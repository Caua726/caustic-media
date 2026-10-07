#!/usr/bin/env python3
"""Generate the Win64 SDK subset and COM slots from clang's typed C AST.

The manifest selects APIs, never their declarations or offsets. Clang computes
LLP64 types, anonymous unions, inherited COM slots and macro values; mingw-gcc
independently checks every exposed record and field in the generated test.
"""
import argparse
import ctypes
import json
from pathlib import Path
import re
import subprocess

from clang import cindex

ROOT = Path(__file__).resolve().parents[1]
CK = cindex.CursorKind
TK = cindex.TypeKind
SIGNED = {TK.CHAR_S, TK.SCHAR, TK.SHORT, TK.INT, TK.LONG, TK.LONGLONG}
UNSIGNED = {TK.BOOL, TK.CHAR_U, TK.UCHAR, TK.USHORT, TK.UINT, TK.ULONG, TK.ULONGLONG, TK.WCHAR}


def integer_value(cursor):
    # Not all distro versions of clang.cindex expose the evaluation API.
    lib = cindex.conf.lib
    lib.clang_Cursor_Evaluate.argtypes = [cindex.Cursor]
    lib.clang_Cursor_Evaluate.restype = ctypes.c_void_p
    lib.clang_EvalResult_getAsLongLong.argtypes = [ctypes.c_void_p]
    lib.clang_EvalResult_getAsLongLong.restype = ctypes.c_longlong
    lib.clang_EvalResult_dispose.argtypes = [ctypes.c_void_p]
    result = lib.clang_Cursor_Evaluate(cursor)
    if not result:
        raise ValueError(f'constant not evaluated: {cursor.spelling}')
    try:
        return lib.clang_EvalResult_getAsLongLong(result)
    finally:
        lib.clang_EvalResult_dispose(result)


def identifier(name):
    name = re.sub(r"[^A-Za-z0-9_]", "_", name)
    if name in {'type', 'fn', 'use', 'as', 'let', 'with', 'struct', 'return',
                'if', 'else', 'while', 'for', 'break', 'continue', 'cast',
                'sizeof', 'call', 'extern', 'only', 'imut', 'mut'}:
        return 'c_' + name
    return name


class Generator:
    def __init__(self, manifest, headers, clang):
        self.manifest = manifest
        includes = '#include <initguid.h>\n' + ''.join(
            f'#include <{h}>\n' for h in manifest['headers'])
        constants = ''.join(f'const long long cm_{n} = (long long)({n});\n'
                            for n in manifest['constants'])
        resource = subprocess.check_output([clang, '-print-resource-dir'], text=True).strip()
        self.tu = cindex.Index.create().parse('caustic_sdk.c', args=[
            '--target=x86_64-w64-windows-gnu', '-isystem', headers,
            '-resource-dir', resource, '-D_WIN32_WINNT=0x0A00', '-DCINTERFACE',
            '-DCOBJMACROS', '-DUNICODE', '-D_UNICODE'],
            unsaved_files=[('caustic_sdk.c', includes + constants)])
        errors = [str(d) for d in self.tu.diagnostics if d.severity >= 3]
        if errors:
            raise ValueError('\n'.join(errors))
        self.decls = {}
        for c in self.tu.cursor.get_children():
            if c.spelling and (c.spelling not in self.decls or c.is_definition()):
                self.decls[c.spelling] = c
        selected = manifest['records'] + manifest['interfaces'] + [
            n + 'Vtbl' for n in manifest['interfaces']]
        self.preferred = {
            self.decls[n].type.get_canonical().get_declaration().get_usr(): n
            for n in selected}
        self.names = {}
        self.records = []
        self.visiting = set()
        self.functions = []
        self.interfaces = []
        self.checks = []
        for n in manifest['records']:
            self.record(self.decls[n].type, n)
        for n in manifest['interfaces']:
            self.record(self.decls[n + 'Vtbl'].type, n + 'Vtbl')
            self.record(self.decls[n].type, n)

    def record(self, t, name=None):
        t = t.get_canonical()
        key = t.get_declaration().get_usr()
        if key in self.names:
            return self.names[key]
        name = self.preferred.get(key, name or identifier(t.get_declaration().spelling))
        if not name or t.get_size() < 0:
            raise ValueError(f'incomplete record: {t.spelling}')
        self.names[key] = name
        self.visiting.add(key)
        # Unions and records with bitfields cannot be declared field by field
        # in Caustic. Their aligned storage remains fully usable via pointers.
        fields = list(t.get_fields())
        opaque = t.get_declaration().kind == CK.UNION_DECL or any(f.is_bitfield() for f in fields)
        if opaque:
            align = t.get_align()
            size = t.get_size()
            if align not in (1, 2, 4, 8) or size % align:
                raise ValueError(f'unsupported alignment: {t.spelling}: {size}/{align}')
            body = [f'    storage as [{size // align}]u{align * 8};']
        else:
            body, at, pad = [], 0, 0
            for i, f in enumerate(fields):
                off = f.get_field_offsetof() // 8
                if off < at or f.get_field_offsetof() % 8:
                    raise ValueError(f'overlapping field: {name}.{f.spelling}')
                if off > at:
                    body.append(f'    _pad{pad} as [{off - at}]u8;')
                    pad += 1
                anonymous = not f.spelling or f.is_anonymous()
                fn = f'_anon{i}' if anonymous else identifier(f.spelling)
                ft = self.type(f.type, field=True, hint=name + '_' + fn)
                body.append(f'    {fn} as {ft};')
                if not anonymous:
                    self.checks.append((name, f.spelling, fn, off))
                at = off + f.type.get_size()
            if at < t.get_size():
                body.append(f'    _pad{pad} as [{t.get_size() - at}]u8;')
        self.records.append((name, t.get_size(), body))
        self.visiting.remove(key)
        return name

    def type(self, t, field=False, hint=None):
        raw = t.spelling
        c = t.get_canonical()
        if c.kind == TK.VOID:
            return 'void'
        if c.kind in SIGNED | UNSIGNED:
            return ('i' if c.kind in SIGNED else 'u') + str(c.get_size() * 8)
        if c.kind == TK.ENUM:
            return self.type(c.get_declaration().enum_type)
        if c.kind == TK.FLOAT:
            return 'f32'
        if c.kind == TK.DOUBLE:
            return 'f64'
        if c.kind == TK.CONSTANTARRAY:
            return f'[{c.element_count}]' + self.type(c.element_type, field=True, hint=hint)
        if c.kind == TK.POINTER:
            p = c.get_pointee().get_canonical()
            # STRICT handles point at opaque C records. Treat them as integer
            # handles, not an imaginary dereferenceable Caustic type.
            handle = raw.startswith('H') and raw.isupper()
            if handle or (p.kind == TK.RECORD and
                          (p.get_size() <= 0 or p.get_declaration().spelling.endswith('__'))):
                return 'i64'
            if p.kind in (TK.FUNCTIONPROTO, TK.FUNCTIONNOPROTO, TK.VOID):
                return '*u8'
            if p.kind == TK.RECORD:
                key = p.get_declaration().get_usr()
                return '*' + self.names[key] if key in self.names else '*u8'
            return '*' + self.type(p)
        if c.kind == TK.RECORD:
            if not field:
                # Win64 passes records of 1/2/4/8 bytes as an integer, all
                # others by a pointer. A COM caller packs those slots below.
                size = c.get_size()
                return f'u{size * 8}' if size in (1, 2, 4, 8) else '*u8'
            return self.record(c, hint)
        raise ValueError(f'unsupported type: {raw} ({c.kind})')

    def emit(self, out):
        out.mkdir(parents=True, exist_ok=True)
        banner = '// Generated by window/win32/tools/generate.py; do not edit.\n'
        types = [banner]
        for name, size, body in self.records:
            types += [f'// {size} bytes in the Win64 ABI.', f'struct {name} {{', *body, '}', '']
        # Constants are expressions in mingw's headers, including casts and
        # negated flags. clang evaluates them without a handwritten parser.
        for n in self.manifest['constants']:
            v = integer_value(self.decls['cm_' + n])
            lit = str(v) if v >= 0 else f'0 - {-v}'
            types.append(f'let is i64 as {n} with imut = {lit};')
        (out / 'types.cst').write_text('\n'.join(types) + '\n')
        guids = [banner, 'use "types.cst" as t;', '']
        for name in self.manifest.get('guids', []):
            init = next(c for c in self.decls[name].get_children()
                        if c.kind == CK.INIT_LIST_EXPR)
            fields = list(init.get_children())
            words = [integer_value(c) for c in fields[:3]]
            octets = [integer_value(c) for c in fields[3].get_children()]
            if len(octets) != 8:
                raise ValueError(f'invalid GUID: {name}')
            guids += [f'fn {name}() as t.GUID {{', '    let is t.GUID as g;']
            for field, value in zip(('Data1', 'Data2', 'Data3'), words):
                guids.append(f'    g.{field} = {value};')
            for i, value in enumerate(octets):
                guids.append(f'    g.Data4[{i}] = {value};')
            guids += ['    return g;', '}', '']
        (out / 'guids.cst').write_text('\n'.join(guids) + '\n')
        symbols = []
        for dll, names in self.manifest['libraries'].items():
            lines = [banner, 'use "types.cst" as t;', '']
            for name in names:
                decl = self.decls[name]
                args = [(identifier(a.spelling) or f'a{i}', self.type(a.type))
                        for i, a in enumerate(decl.get_arguments())]
                ret = self.type(decl.result_type)
                def qualify(ty):
                    # t.Record even underneath a pointer or an array.
                    return re.sub(r'\b([A-Za-z_]\w*)\b',
                                  lambda m: 't.' + m[0] if m[0] in self.names.values() else m[0], ty)
                sig = ', '.join(f'{n} as {qualify(ty)}' for n, ty in args)
                lines.append(f'extern "{dll}.dll" fn {name}({sig}) as {qualify(ret)};')
                symbols.append(f'{dll}.dll {name}')
            (out / (dll + '.cst')).write_text('\n'.join(lines) + '\n')
        (out / 'symbols.txt').write_text('\n'.join(symbols) + '\n')
        com = [banner, 'use "types.cst" as t;', 'use "../abi.cst" as abi;', '']
        for iface in self.manifest['interfaces']:
            table = self.decls[iface + 'Vtbl'].type.get_canonical()
            for slot, field in enumerate(table.get_fields()):
                fn = field.type.get_canonical().get_pointee()
                args = [qualify(self.type(a)) for a in fn.argument_types()]
                ret = qualify(self.type(fn.get_result()))
                name = iface + '_' + field.spelling
                com.append(f'let is i64 as {name}_SLOT with imut = {slot};')
                com.append(f'fn {name}(' + ', '.join(f'a{i} as {ty}' for i, ty in enumerate(args)) + f') as {ret} {{')
                com.append('    let is [16]u64 as slots;')
                for i, ty in enumerate(args):
                    if ty in ('f64', 'f32'):
                        com.append(f'    *cast(*{ty}, &slots[{i}]) = a{i};')
                    else:
                        com.append(f'    slots[{i}] = cast(u64, a{i});')
                com.append(f'    let is *u8 as proc = *cast(**u8, cast(i64, *cast(**u8, a0)) + {slot * 8});')
                call = f'abi.invoke(proc, cast(*u64, &slots), {len(args)})'
                com.append(f'    return cast({ret}, {call});' if ret != 'void' else f'    {call};')
                com += ['}', '']
        (out / 'com.cst').write_text('\n'.join(com) + '\n')
        # The C compiler is a separate oracle, not another rendering of the
        # offsets clang gave us. No test asserts generated source text.
        c = [*(f'#include <{h}>' for h in self.manifest['headers']), '#include <stddef.h>', '#include <stdio.h>', 'int main(void) {']
        test = [banner, 'use "bind/types.cst" as t;', 'use "std/io.cst" as io;', 'let is i64 as failures with mut = 0;',
                'fn check(name as *u8, got as i64, want as i64) as void {',
                '    if (got != want) { io.printf("FAIL %%s: %%d, expected %%d\\n", name, got, want); failures = failures + 1; }', '}']
        cases = {}
        # Only named SDK records are valid names in independently compiled C.
        selected = set(self.manifest['records']) | {n + 'Vtbl' for n in self.manifest['interfaces']} | set(self.manifest['interfaces'])
        for name, size, _ in self.records:
            if name not in selected:
                continue
            c.append(f'    _Static_assert(sizeof({name}) == {size}, "{name}");')
            c.append(f'    printf("{name} %zu\\n", sizeof({name}));')
            cases[name] = [f'fn layout_{name}() as void {{',
                           f'    check("{name}", sizeof(t.{name}), {size});']
            if any(record == name for record, *_ in self.checks):
                cases[name].append(f'    let is t.{name} as v_{name};')
        for name, cfield, field, off in self.checks:
            if name not in cases:
                continue
            c.append(f'    _Static_assert(offsetof({name}, {cfield}) == {off}, "{name}.{cfield}");')
            cases[name].append(f'    check("{name}.{field}", cast(i64, &v_{name}.{field}) - cast(i64, &v_{name}), {off});')
        c += ['    return 0;', '}']
        for lines in cases.values():
            test += lines + ['}', '']
        test.append('fn main() as i32 {')
        test += [f'    layout_{name}();' for name in cases]
        test += ['    if (failures != 0) { return 1; }', '    io.printf("win32 layout: all checks passed\\n");', '    return 0;', '}']
        (ROOT / 'tools/layout.c').write_text('\n'.join(c) + '\n')
        (ROOT / 'layout_test.cst').write_text('\n'.join(test) + '\n')


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--manifest', type=Path, default=ROOT / 'tools/sdk.json')
    ap.add_argument('--headers', default='/usr/x86_64-w64-mingw32/include')
    ap.add_argument('--clang', default='clang')
    ap.add_argument('--out', type=Path, default=ROOT / 'bind')
    ns = ap.parse_args()
    Generator(json.loads(ns.manifest.read_text()), ns.headers, ns.clang).emit(ns.out)


if __name__ == '__main__':
    main()
