#!/usr/bin/env python3
"""Compile Wayland XML into compact, inspectable Caustic protocol metadata."""
import argparse
import re
import xml.etree.ElementTree as ET
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
WIRE = {'int': 'i', 'uint': 'u', 'fixed': 'f', 'string': 's',
        'object': 'o', 'new_id': 'n', 'array': 'a', 'fd': 'h'}


def ident(value):
    out = re.sub(r'[^A-Za-z0-9_]', '_', value)
    if out and out[0].isdigit():
        out = '_' + out
    if out in {'type', 'fn', 'use', 'as', 'let', 'with', 'struct', 'return',
               'if', 'else', 'while', 'for', 'break', 'continue', 'cast',
               'sizeof', 'call', 'extern', 'only', 'imut', 'mut'}:
        out = 'wl_' + out
    return out


def parse(paths):
    protocols = []
    seen = {}
    for path in paths:
        root = ET.parse(path).getroot()
        pname = root.attrib['name']
        protocol = {'name': pname, 'interfaces': []}
        for element in root.findall('interface'):
            name = element.attrib['name']
            if name in seen:
                raise ValueError(f'duplicate interface {name}: {seen[name]} and {path}')
            iface = {'name': name, 'version': int(element.attrib['version']),
                     'requests': [], 'events': [], 'enums': []}
            for direction in ('request', 'event'):
                for opcode, msg in enumerate(element.findall(direction)):
                    since = int(msg.attrib.get('since', '1'))
                    signature = ('%d:' % since if since > 1 else '')
                    argtypes, interfaces = [], []
                    for arg in msg.findall('arg'):
                        kind = arg.attrib['type']
                        if kind not in WIRE:
                            raise ValueError(f'{path}: unknown argument type {kind}')
                        nullable = arg.attrib.get('allow-null', arg.attrib.get('nullable', 'false')) == 'true'
                        if nullable:
                            signature += '?'
                        signature += WIRE[kind]
                        argtypes.append(kind)
                        interfaces.append(arg.attrib.get('interface', ''))
                    # wl_registry.bind is the one core request whose new id
                    # carries its interface name and version on the wire.
                    if name == 'wl_registry' and direction == 'request' and msg.attrib['name'] == 'bind':
                        signature = 'usun'
                        argtypes = ['uint', 'string', 'uint', 'new_id']
                        interfaces = ['', '', '', '']
                    iface[direction + 's'].append({
                        'name': msg.attrib['name'], 'opcode': opcode, 'since': since,
                        'signature': signature, 'types': ','.join(interfaces),
                        'arg_count': len(argtypes),
                        'destructor': msg.attrib.get('type') == 'destructor'})
            for enum in element.findall('enum'):
                entries = []
                for entry in enum.findall('entry'):
                    val = entry.attrib.get('value', '0')
                    value = int(val, 16 if val.startswith('0x') else 10)
                    entries.append((entry.attrib['name'], value))
                iface['enums'].append((enum.attrib['name'], entries))
            protocol['interfaces'].append(iface)
            seen[name] = str(path)
        protocols.append(protocol)
    return protocols


def emit(protocols, out):
    interfaces = [iface for proto in protocols for iface in proto['interfaces']]
    lines = ['// Generated from the Wayland protocol XML files; do not edit.',
             'struct Interface {', '    id as i64;', '    version as i64;',
             '    request_count as i64;', '    event_count as i64;', '    name as *u8;', '}',
             'struct Message {', '    opcode as i64;', '    since as i64;',
             '    arg_count as i64;', '    destructor as i64;',
             '    signature as *u8;', '    types as *u8;', '    name as *u8;', '}', '']
    for ident_id, iface in enumerate(interfaces):
        prefix = ident(iface['name'])
        lines += [f'let is i64 as IFACE_{prefix.upper()} with imut = {ident_id};',
                  f'fn _interface_{ident_id}() as Interface {{',
                  '    let is Interface as d;', f'    d.id = {ident_id};',
                  f'    d.version = {iface["version"]};',
                  f'    d.request_count = {len(iface["requests"])};',
                  f'    d.event_count = {len(iface["events"])};',
                  f'    d.name = "{iface["name"]}";', '    return d;', '}', '']
        for direction in ('requests', 'events'):
            for msg in iface[direction]:
                slot = ident(msg['name'])
                lines += [f'fn _{prefix}_{direction}_{slot}() as Message {{',
                          '    let is Message as m;',
                          f'    m.opcode = {msg["opcode"]};',
                          f'    m.since = {msg["since"]};',
                          f'    m.arg_count = {msg["arg_count"]};',
                          f'    m.destructor = {int(msg["destructor"])};',
                          f'    m.signature = "{msg["signature"]}";',
                          f'    m.types = "{msg["types"]}";',
                          f'    m.name = "{msg["name"]}";', '    return m;', '}', '']
            messages = iface[direction]
            fun = f'_{prefix}_{direction}_at'
            lines += [f'fn {fun}(opcode as i64) as Message {{',
                      '    let is Message as m;', '    m.opcode = 0 - 1;']
            for msg in messages:
                lines.append(f'    if (opcode == {msg["opcode"]}) {{ return _{prefix}_{direction}_{ident(msg["name"])}(); }}')
            lines += ['    return m;', '}', '']
    lines += ['fn interface(id as i64) as Interface {',
              '    let is Interface as d;', '    d.id = 0 - 1;']
    for ident_id, _ in enumerate(interfaces):
        lines.append(f'    if (id == {ident_id}) {{ return _interface_{ident_id}(); }}')
    lines += ['    return d;', '}', '']
    for direction in ('requests', 'events'):
        fun = 'request_message' if direction == 'requests' else 'event_message'
        lines += [f'fn {fun}(iface as i64, opcode as i64) as Message {{',
                  '    let is Message as m;', '    m.opcode = 0 - 1;']
        for ident_id, iface in enumerate(interfaces):
            lines.append(f'    if (iface == {ident_id}) {{ return _{ident(iface["name"])}_{direction}_at(opcode); }}')
        lines += ['    return m;', '}', '']
    for iface in interfaces:
        for enum_name, entries in iface['enums']:
            enum_prefix = (ident(iface['name']) + '_' + ident(enum_name)).upper()
            for entry, value in entries:
                lines.append(f'let is i64 as {enum_prefix}_{ident(entry).upper()} with imut = {value};')
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text('\n'.join(lines) + '\n')
    # Deterministic inventory for independent protoc XML comparison and docs.
    inventory = []
    for iface_id, iface in enumerate(interfaces):
        inventory.append(f'I {iface_id} {iface["name"]} {iface["version"]} {len(iface["requests"])} {len(iface["events"])}')
        for d in ('requests', 'events'):
            for m in iface[d]:
                inventory.append(f'M {iface["name"]} {d[:-1]} {m["opcode"]} {m["since"]} {m["name"]} {m["signature"]} {m["types"]} {int(m["destructor"])}')
    out.with_name('protocol.txt').write_text('\n'.join(inventory) + '\n')


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--out', type=Path, default=ROOT / 'protocol.cst')
    ap.add_argument('xml', nargs='+', type=Path)
    args = ap.parse_args()
    emit(parse(args.xml), args.out)


if __name__ == '__main__':
    main()
