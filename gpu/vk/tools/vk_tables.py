#!/usr/bin/env python3
# gpu/vk/tools/vk_tables.py — the registry, turned into tables we can check against.
#
# This does NOT emit Caustic. bind/ is written by hand, for the same reason
# window/x11/bind/ is: the transcription is where the understanding happens, and
# a generator that emits code nobody reads produces a binding nobody can debug.
#
# What it emits is the reference the hand-written code is checked against, which
# is the same split window/x11/tools/x11_layout.c already uses — a program asks
# the authority, the answer is committed, and a test asserts the code matches it.
#
# Four outputs:
#
#   symbols.txt   every command, with its dispatch level and where it comes from.
#                 check_symbols.sh compares this against what bind/ declares.
#   enums.txt     every enumerant with its resolved value, including the ones
#                 the registry only describes as arithmetic on an extension
#                 number. check_constants.sh compares this against bind/.
#   structs.txt   every struct and union with its members, so a struct nobody
#                 transcribed is visible rather than merely absent.
#   vk_layout.c   a C program that prints sizeof and offsetof for every struct.
#                 Compiled and run by hand; its output is committed as
#                 layout.txt and asserted by vk_layout_test.cst.
#
# The registry is the one Khronos publishes and every language's bindings are
# generated from. It ships with the Vulkan SDK and with Mesa:
#
#   python3 vk_tables.py [/usr/share/vulkan/registry/vk.xml]
#
# Regenerating is a deliberate act with a visible diff, not something that
# happens under a system update — same reason x11_layout.c is committed rather
# than run by the build.

import os
import sys

# defusedxml when it is installed, the stdlib parser when it is not. vk.xml is a
# file the Vulkan package put on this machine and the path is typed by whoever
# runs this, so neither entity expansion nor external fetches are a live threat
# here — but the hardened parser costs nothing when it is present, and a hard
# dependency on it would mean a developer cannot regenerate the tables without
# pip. This script is run by hand; its OUTPUT is what CI consumes.
try:
    from defusedxml import ElementTree as ET
except ImportError:
    import xml.etree.ElementTree as ET

DEFAULT_XML = "/usr/share/vulkan/registry/vk.xml"
OUT_DIR = os.path.dirname(os.path.abspath(__file__))

# The API this binding targets. vk.xml also describes Vulkan SC, whose commands
# and structs differ; everything tagged for it alone is not ours.
API = "vulkan"

# --- Dispatch level ---
#
# A command's level is decided by the type of its first parameter, and it is not
# cosmetic: a device-level command fetched through vkGetInstanceProcAddr still
# works, but reaches the driver through a loader trampoline that dispatches on
# the handle at every call. Fetched through vkGetDeviceProcAddr it is direct.
# That difference is invisible until it is in every vkCmdDraw.
DEVICE_FIRST_PARAM = ("VkDevice", "VkQueue", "VkCommandBuffer")

# Resolvable before there is an instance, so they come from the one extern
# rather than from either load function.
LOADER_COMMANDS = (
    "vkGetInstanceProcAddr",
    "vkCreateInstance",
    "vkEnumerateInstanceVersion",
    "vkEnumerateInstanceLayerProperties",
    "vkEnumerateInstanceExtensionProperties",
)


def api_ok(el):
    """vk.xml tags anything Vulkan-SC-only with an api attribute. No attribute
    means it belongs to every API, which is the common case."""
    a = el.get("api")
    return a is None or API in a.split(",")


def supported(ext):
    """An extension's supported attribute is a comma-separated list, and it must
    be split rather than searched: "vulkan" is a substring of "vulkansc", so a
    plain `in` quietly accepts every Vulkan SC extension as ours."""
    return API in (ext.get("supported") or "").split(",")


def core_name(feature):
    """Since 1.4 the registry splits each core version into profile features —
    VK_BASE_VERSION_1_0, VK_COMPUTE_VERSION_1_0, VK_GRAPHICS_VERSION_1_0 — that
    together make up VK_VERSION_1_0. The split matters to someone building a
    profile-limited driver and to nobody transcribing a binding, so it collapses
    back to the version everyone actually says."""
    for tag in ("BASE_", "COMPUTE_", "GRAPHICS_"):
        if feature.startswith("VK_" + tag):
            return "VK_" + feature[len("VK_" + tag):]
    return feature


class Registry:
    def __init__(self, path):
        self.root = ET.parse(path).getroot()
        self.types = {}
        for t in self.root.find("types"):
            name = t.get("name")
            if name is None:
                n = t.find("name")
                name = n.text if n is not None else None
            if name and api_ok(t):
                self.types.setdefault(name, t)
        self.commands = {}
        for c in self.root.find("commands"):
            if not api_ok(c):
                continue
            if c.get("alias"):
                self.commands[c.get("name")] = c
            else:
                self.commands[c.find("proto/name").text] = c

    def resolve(self, cmd):
        """Follow an alias to the command that actually carries a prototype."""
        seen = 0
        while cmd is not None and cmd.get("alias") and seen < 8:
            cmd = self.commands.get(cmd.get("alias"))
            seen += 1
        return cmd

    def level(self, name):
        if name in LOADER_COMMANDS:
            return "loader"
        c = self.resolve(self.commands.get(name))
        if c is None:
            return "instance"
        p = c.find("param")
        if p is None:
            return "instance"
        t = p.find("type")
        if t is not None and t.text in DEVICE_FIRST_PARAM:
            return "device"
        return "instance"

    def reachable(self):
        """Commands and types a program can actually name: core, plus every
        extension the registry still supports for this API. The 222 disabled
        extensions are in the file but are not part of Vulkan."""
        cmds = {}
        for f in self.root.findall("feature"):
            if not api_ok(f):
                continue
            for req in f.findall("require"):
                if not api_ok(req):
                    continue
                for c in req.findall("command"):
                    cmds.setdefault(c.get("name"), core_name(f.get("name")))
        for e in self.root.findall("extensions/extension"):
            if not supported(e):
                continue
            for req in e.findall("require"):
                if not api_ok(req):
                    continue
                for c in req.findall("command"):
                    cmds.setdefault(c.get("name"), e.get("name"))
        return cmds


# --- Enumerant values ---
#
# Most enumerants carry their value literally. The ones an extension adds do
# not: the registry gives an offset and lets the reader compute
#
#   1000000000 + (extension_number - 1) * 1000 + offset
#
# negated when dir="-", which is how VkResult's extension error codes stay
# negative. Getting this wrong produces a constant that compiles, links, and
# makes the driver reject a call for a reason nothing explains — so it is
# computed here once rather than by hand 330 times.
EXT_BASE = 1000000000
EXT_BLOCK = 1000


def enum_value(el, ext_number):
    if el.get("alias"):
        return None
    v = el.get("value")
    if v is not None:
        return v.strip()
    bitpos = el.get("bitpos")
    if bitpos is not None:
        return "0x%X" % (1 << int(bitpos))
    offset = el.get("offset")
    if offset is not None:
        n = el.get("extnumber") or ext_number
        if n is None:
            return None
        val = EXT_BASE + (int(n) - 1) * EXT_BLOCK + int(offset)
        if el.get("dir") == "-":
            val = -val
        return str(val)
    return None


def collect_enums(reg):
    """Every enumerant, grouped by the enum it belongs to. Values arrive from
    three places — the enums block, a feature's require, and an extension's
    require — and the last two are where the arithmetic lives."""
    groups = {}
    width = {}

    for enums in reg.root.findall("enums"):
        gname = enums.get("name")
        if gname is None:
            continue
        width[gname] = int(enums.get("bitwidth") or 32)
        bucket = groups.setdefault(gname, {})
        for e in enums.findall("enum"):
            if not api_ok(e):
                continue
            v = enum_value(e, None)
            if v is not None:
                bucket[e.get("name")] = v

    def sweep(container, ext_number):
        for req in container.findall("require"):
            if not api_ok(req):
                continue
            for e in req.findall("enum"):
                if not api_ok(e):
                    continue
                g = e.get("extends")
                if g is None:
                    # A bare constant like VK_MAX_EXTENSION_NAME_SIZE. Still
                    # ours to declare, so it gets a group of its own.
                    v = e.get("value")
                    if v is not None and e.get("alias") is None:
                        groups.setdefault("VK_API_CONSTANTS", {})[e.get("name")] = v.strip()
                    continue
                v = enum_value(e, ext_number)
                if v is not None:
                    groups.setdefault(g, {})[e.get("name")] = v

    for f in reg.root.findall("feature"):
        if api_ok(f):
            sweep(f, None)
    for e in reg.root.findall("extensions/extension"):
        if supported(e):
            sweep(e, e.get("number"))

    return groups, width


# --- Struct members ---
#
# The member's C type as written, because that is what a transcription has to
# reproduce and what vk_layout.c has to declare. Pointer-ness and array length
# live in the text around the name, not in attributes.
def member_text(m):
    parts = []
    for node in m.iter():
        if node.tag == "comment":
            continue
        if node is not m and node.text:
            parts.append(node.text)
        if node is not m and node.tail:
            parts.append(node.tail)
    if m.text:
        parts.insert(0, m.text)
    return " ".join("".join(parts).split())


def struct_members(t):
    out = []
    for m in t.findall("member"):
        if not api_ok(m):
            continue
        name = m.find("name")
        out.append((name.text if name is not None else "?", member_text(m)))
    return out


# --- vk_layout.c ---
#
# Caustic structs are packed and C structs are not, so every struct in bind/
# carries explicit _padN fields wherever the C ABI would have aligned. Measured
# on this registry: 1219 of the 1342 structs need at least one, because nearly
# every Vulkan struct opens with sType (4 bytes) followed by pNext (a pointer),
# and that pair alone forces one.
#
# A pad that is missing is not a compile error and not a crash. It is a field
# read at the wrong offset — the same silent failure window/x11 paid for once
# already. So the numbers are not counted by hand: this asks the C compiler that
# built this machine's Vulkan headers, and its output is what vk_layout_test.cst
# asserts against.
#
# Only the platform-independent surface is emitted. Structs behind Android,
# Win32, Fuchsia, QNX, Metal and friends need headers this machine does not
# have; Xlib is the exception, because VK_KHR_xlib_surface is the one platform
# extension this library actually uses.
PLATFORMS_KEPT = (None, "xlib")

LAYOUT_C_HEAD = """\
/* gpu/vk/tools/vk_layout.c — where the offsets come from.
 *
 * GENERATED by vk_tables.py. Do not edit; regenerate.
 *
 * Caustic structs are packed and C structs are not, so every struct in
 * gpu/vk/bind/ carries explicit _padN fields wherever the C ABI would have
 * inserted padding. Those pads are load-bearing: drop the one between sType
 * and pNext and every field after it lands four bytes early, the driver reads
 * garbage, and nothing says a word.
 *
 * This program asks the C compiler that built this machine's Vulkan headers,
 * and its output is the reference bind/ is written from and vk_layout_test.cst
 * asserts against.
 *
 * It is committed rather than run by the build for two reasons: CI then needs
 * no C compiler and no Vulkan headers, and re-deriving the layout becomes a
 * deliberate act with a visible diff instead of something that changes
 * silently under a system update.
 *
 *   cc -o vk_layout vk_layout.c && ./vk_layout > layout.txt
 */
#define VK_USE_PLATFORM_XLIB_KHR
#include <stdio.h>
#include <stddef.h>
#include <vulkan/vulkan.h>

#define S(t)    printf("size %-56s %zu\\n", #t, sizeof(t))
#define F(t, f) printf("off  %-56s %-40s %zu\\n", #t, #f, offsetof(t, f))

int main(void) {
    printf("# gpu/vk/tools/vk_layout.c output — x86_64 linux, Vulkan %s\\n", VERSION_STR);
    printf("# regenerate: cc -o vk_layout vk_layout.c && ./vk_layout > layout.txt\\n");
"""


def emit_layout_c(reg, path, version):
    """Emit the C program whose output becomes layout.txt."""
    # Which platform, if any, each type is gated behind.
    #
    # A type with no gate at all is NOT kept. The registry also describes Vulkan
    # SC and 222 disabled extensions, and their structs are in the file but not
    # in this machine's headers — defaulting to keep emits references to types
    # the compiler has never heard of.
    gate = {}
    for e in reg.root.findall("extensions/extension"):
        if not supported(e):
            continue
        plat = e.get("platform")
        beta = e.get("provisional") == "true"
        for req in e.findall("require"):
            if not api_ok(req):
                continue
            for t in req.findall("type"):
                # First gate wins, and the core sweep below overrides it: a type
                # promoted to core is reachable without naming the extension.
                gate.setdefault(t.get("name"), (plat, beta))
    for f in reg.root.findall("feature"):
        if not api_ok(f):
            continue
        for req in f.findall("require"):
            if not api_ok(req):
                continue
            for t in req.findall("type"):
                gate[t.get("name")] = (None, False)

    lines = [LAYOUT_C_HEAD.replace("VERSION_STR", '"%s"' % version)]
    kept = skipped = 0
    for name in sorted(reg.types):
        t = reg.types[name]
        if t.get("category") not in ("struct", "union") or t.get("alias"):
            continue
        if name not in gate:
            skipped += 1
            continue
        plat, beta = gate[name]
        if plat not in PLATFORMS_KEPT or beta:
            skipped += 1
            continue
        members = struct_members(t)
        # offsetof cannot name a bitfield. The 27 that exist are all in NV
        # cluster acceleration-structure extensions; the struct's size is still
        # worth asserting, so only the members are dropped.
        bitfield = any(":" in txt.split(mname, 1)[-1] for mname, txt in members)
        lines.append("")
        lines.append("    S(%s);" % name)
        if not bitfield:
            for mname, _ in members:
                lines.append("    F(%s, %s);" % (name, mname))
        kept += 1
    lines.append("")
    lines.append("    return 0;")
    lines.append("}")

    with open(path, "w") as f:
        f.write("\n".join(lines) + "\n")
    print("  %-14s %5d structs  (%d fora: plataforma ou provisoria)"
          % (os.path.basename(path), kept, skipped))


def write(path, lines):
    with open(path, "w") as f:
        f.write("\n".join(lines) + "\n")
    print("  %-14s %5d linhas" % (os.path.basename(path), len(lines)))


def main():
    xml = sys.argv[1] if len(sys.argv) > 1 else DEFAULT_XML
    if not os.path.exists(xml):
        sys.stderr.write(
            "vk_tables: %s nao existe.\n"
            "O registry vem com o SDK do Vulkan e com o Mesa; passe o caminho\n"
            "como argumento se ele estiver em outro lugar.\n" % xml
        )
        return 1

    reg = Registry(xml)

    # The patch level, and the newest core version the registry describes.
    # Both go in the console line so a stale table is visible when regenerating.
    # There are two VK_HEADER_VERSION defines in the file — one for Vulkan and
    # one for Vulkan SC — so the api filter is what picks the right one, and
    # taking the last match silently reports the SC patch level instead.
    patch = "?"
    for t in reg.root.find("types"):
        n = t.find("name")
        if (t.get("category") == "define" and api_ok(t)
                and n is not None and n.text == "VK_HEADER_VERSION"):
            patch = (n.tail or "").strip()
            break
    cores = sorted(
        core_name(f.get("name"))
        for f in reg.root.findall("feature")
        if api_ok(f) and core_name(f.get("name")).startswith("VK_VERSION_")
    )
    version = "%s.%s" % (cores[-1].replace("VK_VERSION_", "").replace("_", "."), patch)

    header = [
        "# gerado por gpu/vk/tools/vk_tables.py a partir de %s" % xml,
        "# regerar: python3 gpu/vk/tools/vk_tables.py",
    ]

    print("vk_tables: lendo %s" % xml)

    # --- symbols.txt ---
    reach = reg.reachable()
    rows = []
    for name in sorted(reach):
        rows.append("%-9s %-52s %s" % (reg.level(name), name, reach[name]))
    counts = {}
    for name in reach:
        counts[reg.level(name)] = counts.get(reg.level(name), 0) + 1
    write(
        os.path.join(OUT_DIR, "symbols.txt"),
        header
        + [
            "# um comando por linha: nivel de despacho, nome, de onde vem.",
            "#",
            "# O nivel decide por qual funcao o ponteiro e preenchido. loader vem",
            "# do unico extern; instance vem de vk.load_instance; device vem de",
            "# vk.load_device, e buscar um device-level pelo caminho de instance",
            "# custa um trampolim do loader em toda chamada.",
            "#",
            "# total %d: %d loader, %d instance, %d device"
            % (
                len(reach),
                counts.get("loader", 0),
                counts.get("instance", 0),
                counts.get("device", 0),
            ),
        ]
        + rows,
    )

    # --- enums.txt ---
    groups, width = collect_enums(reg)
    rows = []
    total = 0
    for g in sorted(groups):
        if not groups[g]:
            continue
        bits = width.get(g, 32)
        rows.append("")
        rows.append("# %s%s" % (g, "  (64 bits)" if bits == 64 else ""))
        for n in sorted(groups[g]):
            rows.append("%-4d %-56s %s" % (bits, n, groups[g][n]))
            total += 1
    write(
        os.path.join(OUT_DIR, "enums.txt"),
        header
        + [
            "# largura em bits, nome, valor. Agrupado por enum, com o grupo em",
            "# comentario porque o binding declara constantes soltas e nao enums.",
            "#",
            "# Os valores de extensao ja vem calculados: o registry so descreve a",
            "# aritmetica (1000000000 + (numero - 1) * 1000 + offset, negado se",
            "# dir=-), e errar isso produz uma constante que compila e que faz o",
            "# driver recusar a chamada sem explicar por que.",
            "#",
            "# %d constantes em %d grupos" % (total, len([g for g in groups if groups[g]])),
        ]
        + rows,
    )

    # --- structs.txt ---
    rows = []
    nstruct = nunion = 0
    for name in sorted(reg.types):
        t = reg.types[name]
        cat = t.get("category")
        if cat not in ("struct", "union"):
            continue
        if t.get("alias"):
            rows.append("alias  %-52s = %s" % (name, t.get("alias")))
            continue
        if cat == "union":
            nunion += 1
        else:
            nstruct += 1
        rows.append("")
        rows.append("%s %s" % (cat, name))
        for mname, mtext in struct_members(t):
            rows.append("    %-32s %s" % (mname, mtext))
    write(
        os.path.join(OUT_DIR, "structs.txt"),
        header
        + [
            "# toda struct e uniao, com seus membros como o C os declara.",
            "#",
            "# Serve para ver o que ainda nao foi transcrito: uma struct ausente",
            "# do bind/ e invisivel por si so, e so aparece quando alguem precisa",
            "# dela. Aqui ela esta listada desde o primeiro dia.",
            "#",
            "# %d structs, %d unioes" % (nstruct, nunion),
        ]
        + rows,
    )

    # --- vk_layout.c ---
    emit_layout_c(reg, os.path.join(OUT_DIR, "vk_layout.c"), version)

    print("vk_tables: ok  (Vulkan %s)" % version)
    print("           layout.txt e um passo a mao:")
    print("           cd gpu/vk/tools && cc -o vk_layout vk_layout.c && ./vk_layout > layout.txt")
    return 0


if __name__ == "__main__":
    sys.exit(main())
