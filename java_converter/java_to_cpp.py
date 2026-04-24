#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""java_to_cpp.py

Java → C++17 AST-based transpiler (javalang) with project-specific rules.

Array mapping (ALL occurrences: fields/locals/params/returns):
  • Any Java array type T[] maps to:
        jxx::Ptr<jxx::lang::JxxArray<Elem, Rank>>
    where Rank is the number of Java array dimensions.

Reference element semantics (updated):
  • For arrays of reference types (e.g., Foo[], String[][]), element type is wrapped:
        Elem = jxx::Ptr<Foo>   (or jxx::Ptr<String>)
    Therefore:
        Foo[]    → jxx::Ptr<jxx::lang::JxxArray<jxx::Ptr<Foo>, 1>>
        Foo[][]  → jxx::Ptr<jxx::lang::JxxArray<jxx::Ptr<Foo>, 2>>

Primitive element semantics:
  • For arrays of primitives, element type is by-value j*:
        int[] → jxx::Ptr<jxx::lang::JxxArray<jint, 1>>

Byte array mapping (requested):
  • Java byte[] (Rank==1) maps to:
        jxx::Ptr<ByteArray>
    for parameters/returns/fields/locals.

  • Java byte[][] and higher (Rank>=2) maps to:
        jxx::Ptr<jxx::lang::JxxArray<jbyte, Rank>>

Class mapping (ALL occurrences):
  • Class / Class<?> / Class<T> (Rank==0) → jxx::Ptr<jxx::lang::ClassAny>
  • Class[] / Class[][] / ...            → jxx::Ptr<jxx::lang::JxxArray<jxx::lang::ClassAny, Rank>>

Other rules:
  • Primitive mapping defaults: int→jint, boolean→jbool, short→jshort, ... (override via --primitive-map)
  • new Foo(a) → JXX_NEW<Foo>(a) (configurable)
  • java.* import-derived includes: import java.io.* → #include "jxx.io.h"; import java.util.Map → #include "jxx.util.Map.h"

Runtime expectations:
  • namespace jxx { template<class T> using Ptr = std::shared_ptr<T>; }
  • namespace jxx::lang { class ClassAny; template<class T, int Rank> class JxxArray; }
  • ByteArray is your byte[] wrapper type (NOT a pointer). We wrap it as jxx::Ptr<ByteArray>.

Requires: pip install javalang
"""

from __future__ import annotations

import argparse
import os
import sys
from typing import List, Optional, Set, Tuple, Dict

try:
    import javalang
except Exception:
    javalang = None

JAVA_TO_CPP_TYPES = {
    "byte": "jbyte",
    "short": "jshort",
    "int": "jint",
    "long": "jlong",
    "float": "jfloat",
    "double": "jdouble",
    "boolean": "jbool",
    "char": "jchar",
    "void": "void",
}

class Emit:
    def __init__(self) -> None:
        self.lines: List[str] = []
        self.indent_level = 0

    def write(self, line: str = "") -> None:
        self.lines.append(("    " * self.indent_level) + line if line else line)

    def enter(self) -> None:
        self.indent_level += 1

    def exit(self) -> None:
        self.indent_level = max(0, self.indent_level - 1)

    def text(self) -> str:
        return "\n".join(self.lines)


def _base_type_name_and_dims(t) -> Tuple[str, int]:
    if t is None:
        return ("void", 0)
    if isinstance(t, str):
        return (t, 0)
    name = ".".join(t.name) if isinstance(t.name, (list, tuple)) else t.name
    dims = len(t.dimensions or [])
    return (name, dims)


class Transpiler:
    def __init__(
        self,
        string_policy: str = "preserve",
        string_include: Optional[str] = None,
        bytearray_include: Optional[str] = None,
        extra_includes: Optional[List[str]] = None,
        no_default_includes: bool = False,
        pragma_once: bool = True,
        header_guards: bool = False,
        guard_name: Optional[str] = None,
        new_macro_style: str = "template",
        new_macro: str = "JXX_NEW",
        primitive_map: Optional[List[str]] = None,
    ) -> None:
        self.string_policy = string_policy
        self.string_include = string_include
        self.bytearray_include = bytearray_include
        self.extra_includes = list(extra_includes or [])
        self.no_default_includes = no_default_includes
        self.pragma_once = pragma_once
        self.header_guards = header_guards
        self.guard_name = guard_name
        self.new_macro_style = new_macro_style
        self.new_macro = new_macro

        self.primitive_map: Dict[str, str] = dict(JAVA_TO_CPP_TYPES)
        for item in (primitive_map or []):
            if item and '=' in item:
                k, v = item.split('=', 1)
                k = k.strip(); v = v.strip()
                if k and v:
                    self.primitive_map[k] = v

        self.using_lines: Set[str] = set()
        self.import_header_includes: List[str] = []
        self.include_written: Set[str] = set()

    # ---------- includes ----------
    @staticmethod
    def _norm_inc(s: str) -> str:
        s = (s or "").strip()
        if not s:
            return s
        if s.startswith('<') or s.startswith('"'):
            return s
        return f'"{s}"'

    def _emit_include_operand(self, out: Emit, operand: str) -> None:
        op = self._norm_inc(operand)
        if op and op not in self.include_written:
            self.include_written.add(op)
            out.write(f"#include {op}")

    def emit_includes(self, out: Emit) -> None:
        ordered: List[str] = []
        if self.string_include:
            ordered.append(self.string_include)
        if self.bytearray_include:
            ordered.append(self.bytearray_include)
        ordered.extend(self.extra_includes)
        if not self.no_default_includes:
            ordered.extend(["<vector>", "<array>"])
            if self.string_policy == 'std':
                ordered.append('<string>')

        seen = set()
        for inc in ordered:
            op = self._norm_inc(inc)
            if op and op not in seen:
                seen.add(op)
                self._emit_include_operand(out, op)

    # ---------- imports → usings + import-derived includes ----------
    @staticmethod
    def _looks_pkg(parts: List[str], wildcard: bool) -> bool:
        if wildcard:
            return True
        return (not parts) or (parts[-1] and parts[-1][0].islower())

    def gather_usings(self, tree) -> None:
        self.using_lines.clear()
        self.import_header_includes = []
        for imp in (tree.imports or []):
            path = imp.path
            if not path or not path.startswith('java.'):
                continue
            rest = path[len('java.'):]
            segs = rest.split('.') if rest else []

            if imp.wildcard:
                ns = '::'.join(['jxx'] + segs)
                self.using_lines.add(f"using namespace {ns};")
                self.import_header_includes.append(f"jxx.{'.'.join(segs)}.h" if segs else 'jxx.h')
            else:
                cxx = '::'.join(['jxx'] + segs) if segs else 'jxx'
                if self._looks_pkg(segs, False):
                    self.using_lines.add(f"using namespace {cxx};")
                    if segs:
                        self.import_header_includes.append(f"jxx.{'.'.join(segs)}.h")
                else:
                    self.using_lines.add(f"using {cxx};")
                    if segs:
                        *pkg, typ = segs
                        inc = f"jxx.{'.'.join(pkg)}.{typ}.h" if pkg else f"jxx.{typ}.h"
                        self.import_header_includes.append(inc)

    # ---------- type mapping ----------
    @staticmethod
    def _simple_name(name: str) -> str:
        return name.split('.')[-1]

    def _is_java_class_ref(self, t) -> bool:
        return type(t).__name__ == 'ReferenceType' and self._simple_name(str(t.name)) == 'Class'

    def _map_ref_or_basic(self, t) -> str:
        tn = type(t).__name__
        if tn == 'BasicType':
            return self.primitive_map.get(t.name, t.name)
        if tn == 'ReferenceType':
            if self._is_java_class_ref(t):
                return 'jxx::Ptr<jxx::lang::ClassAny>'
            raw = t.name.replace('.', '::') if isinstance(t.name, str) else str(t.name)
            if raw == 'String':
                raw = 'String' if self.string_policy == 'preserve' else 'std::string'
            return raw
        if isinstance(t, str):
            if t == 'String':
                return 'String' if self.string_policy == 'preserve' else 'std::string'
            return self.primitive_map.get(t, t)
        return str(t)

    def _array_type(self, elem_cpp: str, rank: int) -> str:
        return f'jxx::Ptr<jxx::lang::JxxArray<{elem_cpp}, {rank}>>'

    def _array_elem_type(self, t) -> str:
        """Element type used inside JxxArray<Elem,Rank>.

        Java semantics:
          • primitive[] stores primitives by value
          • reference[] stores references

        Therefore:
          • BasicType => mapped primitive (e.g., jint)
          • ReferenceType (non-Class) => jxx::Ptr<CppType>
          • Class => ClassAny by value inside array, because the array itself is a reference container
            and ClassAny is the erased Class<?> representation.
        """
        tn = type(t).__name__
        if tn == 'BasicType':
            return self.primitive_map.get(t.name, t.name)
        if tn == 'ReferenceType':
            if self._is_java_class_ref(t):
                return 'jxx::lang::ClassAny'
            base = self._map_ref_or_basic(t)  # e.g., Foo, String
            return f'jxx::Ptr<{base}>'
        # Fallback
        base = self._map_ref_or_basic(t)
        return f'jxx::Ptr<{base}>'

    def _map_type(self, t, extra_dims: int = 0) -> str:
        base_name, d0 = _base_type_name_and_dims(t)
        rank = d0 + (extra_dims or 0)

        # byte[] (rank==1) maps to Ptr<ByteArray>
        if self._simple_name(base_name) == 'byte' and rank == 1:
            return 'jxx::Ptr<ByteArray>'

        # Class[] uses JxxArray<ClassAny, rank>
        if type(t).__name__ == 'ReferenceType' and self._simple_name(base_name) == 'Class':
            if rank == 0:
                return 'jxx::Ptr<jxx::lang::ClassAny>'
            return f'jxx::Ptr<jxx::lang::JxxArray<jxx::lang::ClassAny, {rank}>>'

        if rank > 0:
            elem = self._array_elem_type(t)
            return self._array_type(elem, rank)

        # Non-array types: primitives by value, references by value (unchanged outside arrays)
        return self._map_ref_or_basic(t)

    # ---------- expressions (minimal) ----------
    def emit_expression(self, e) -> str:
        if e is None:
            return ''
        tn = type(e).__name__
        if tn == 'Literal':
            return 'nullptr' if e.value is None else e.value
        if tn == 'ClassCreator':
            name = self._map_ref_or_basic(e.type)
            args = [self.emit_expression(a) for a in (e.arguments or [])]
            if self.new_macro_style == 'template':
                return f"{self.new_macro}<{name}>({', '.join(args)})" if args else f"{self.new_macro}<{name}>()"
            return f"{self.new_macro}({name})({', '.join(args)})" if args else f"{self.new_macro}({name})()"
        if tn == 'MethodInvocation':
            qual = e.qualifier
            qstr = qual if isinstance(qual, str) else (self.emit_expression(qual) if qual is not None else '')
            args = [self.emit_expression(a) for a in (e.arguments or [])]
            call = f"{qstr}.{e.member}" if qstr else e.member
            return f"{call}({', '.join(args)})" if args else f"{call}()"
        return str(e)

    def emit_statement(self, s, out: Emit) -> None:
        if s is None:
            return
        tn = type(s).__name__
        if tn == 'ReturnStatement':
            out.write(f"return {self.emit_expression(s.expression)};")
            return
        if tn == 'StatementExpression':
            out.write(f"{self.emit_expression(s.expression)};")
            return
        if tn == 'BlockStatement':
            for st in (s.statements or []):
                self.emit_statement(st, out)
            return
        out.write(f"/* TODO: Unhandled statement: {tn} */")

    # ---------- signatures ----------
    def _method_signature(self, m, class_name: str) -> Tuple[str, bool]:
        is_ctor = (type(m).__name__ == 'ConstructorDeclaration')

        if not is_ctor:
            if m.return_type is not None:
                md = len(getattr(m, 'dimensions', []) or [])
                ret = self._map_type(m.return_type, extra_dims=md)
            else:
                ret = 'void'
        else:
            ret = ''

        name = m.name if not is_ctor else class_name
        params: List[str] = []
        for p in (m.parameters or []):
            params.append(f"{self._map_type(p.type)} {p.name}")

        sig = f"{ret} {name}({', '.join(params)})" if not is_ctor else f"{name}({', '.join(params)})"
        return sig, is_ctor

    # ---------- unit emission ----------
    def _compute_guard_from_path(self, header_path: str) -> str:
        up = header_path.replace('\\', '/').upper()
        macro = ''.join(ch if ch.isalnum() else '_' for ch in up)
        if not macro.endswith('_H') and not macro.endswith('_HPP'):
            macro += '_H'
        return macro

    def transpile_unit(self, java_src: str, mode: str = 'monolith', header_rel_include: Optional[str] = None,
                      header_guard_name: Optional[str] = None) -> str:
        if javalang is None:
            raise RuntimeError('javalang is required. pip install javalang')
        tree = javalang.parse.parse(java_src)
        self.gather_usings(tree)

        ns = tree.package.name.replace('.', '::') if tree.package else ''

        out = Emit()
        out.write('// Generated by java_to_cpp (AST-based)')
        self.include_written = set()

        guard_to_use = None
        if mode == 'header':
            if self.header_guards:
                guard_to_use = header_guard_name or self.guard_name or 'GENERATED_HEADER_GUARD_H'
                out.write(f'#ifndef {guard_to_use}')
                out.write(f'#define {guard_to_use}')
            elif self.pragma_once:
                out.write('#pragma once')

        if mode != 'source':
            for inc in sorted(set(self.import_header_includes)):
                self._emit_include_operand(out, inc)
            self.emit_includes(out)
            out.write('// NOTE: Ensure jxx::Ptr, jxx::lang::JxxArray<T,Rank>, ByteArray, and ClassAny are available.')
        else:
            if not header_rel_include:
                raise ValueError('header_rel_include required for source mode')
            self._emit_include_operand(out, header_rel_include)
            self.emit_includes(out)

        out.write('')
        for line in sorted(self.using_lines):
            out.write(line)
        if self.using_lines:
            out.write('')

        if ns:
            out.write(f'namespace {ns} {{')
            out.write('')
            out.enter()

        for t in (tree.types or []):
            if type(t).__name__ not in ('ClassDeclaration', 'InterfaceDeclaration'):
                continue
            is_iface = (type(t).__name__ == 'InterfaceDeclaration')
            kw = 'struct' if is_iface else 'class'
            out.write(f'{kw} {t.name} {{')
            out.enter(); out.write('public:'); out.enter()
            for m in (t.body or []):
                if type(m).__name__ in ('MethodDeclaration', 'ConstructorDeclaration'):
                    sig, _ = self._method_signature(m, t.name)
                    out.write(sig + ';')
            out.exit(); out.exit(); out.write('};')
            out.write('')

        if ns:
            out.exit(); out.write(f'}} // namespace {ns}')

        if mode == 'header' and guard_to_use:
            out.write(f'#endif // {guard_to_use}')

        return out.text()


# ---------- CLI / IO ----------

def java_package_of(src: str) -> str:
    try:
        tree = javalang.parse.parse(src)
    except Exception:
        return ''
    return tree.package.name if tree.package else ''


def path_for_file(out_root: str, subdir: str, pkg: str, stem: str, ext: str) -> str:
    pkg_path = pkg.replace('.', os.sep) if pkg else ''
    full_dir = os.path.join(out_root, subdir, pkg_path)
    os.makedirs(full_dir, exist_ok=True)
    return os.path.join(full_dir, f'{stem}{ext}')


def rel_include(from_file: str, header_file: str) -> str:
    return os.path.relpath(header_file, start=os.path.dirname(from_file)).replace(os.sep, '/')


def generate_cmake(out_root: str,
                   project: str = 'TranspiledProject',
                   target: str = 'transpiled',
                   kind: str = 'static',
                   cxx_standard: str = '17',
                   cmake_min: str = '3.20') -> str:
    kind_map = {'static': 'STATIC', 'shared': 'SHARED', 'object': 'OBJECT', 'interface': 'INTERFACE'}
    kind_kw = kind_map.get(kind.lower(), 'STATIC')

    cpp_files: List[str] = []
    for root, _, files in os.walk(os.path.join(out_root, 'src')):
        for fn in files:
            if fn.lower().endswith('.cpp'):
                cpp_files.append(os.path.join(root, fn).replace('\\', '/'))
    rel_cpp = [os.path.relpath(p, start=out_root).replace('\\', '/') for p in cpp_files]

    L: List[str] = []
    L.append(f'cmake_minimum_required(VERSION {cmake_min})')
    L.append(f'project({project} LANGUAGES CXX)')
    L.append('')
    L.append(f'set(CMAKE_CXX_STANDARD {cxx_standard})')
    L.append('set(CMAKE_CXX_STANDARD_REQUIRED ON)')
    L.append('set(CMAKE_CXX_EXTENSIONS OFF)')
    L.append('')

    if kind_kw == 'INTERFACE':
        L.append(f'add_library({target} INTERFACE)')
        L.append(f'target_include_directories({target} INTERFACE ${{CMAKE_CURRENT_SOURCE_DIR}}/include)')
    else:
        if rel_cpp:
            L.append(f'add_library({target} {kind_kw}\n    ' + '\n    '.join(rel_cpp) + '\n)')
        else:
            L.append(f'file(GLOB_RECURSE {target}_SRCS CONFIGURE_DEPENDS src/*.cpp)')
            L.append(f'add_library({target} {kind_kw} ${{{target}_SRCS}})')
        L.append(f'target_include_directories({target} PUBLIC ${{CMAKE_CURRENT_SOURCE_DIR}}/include)')

    L.append('')
    L.append('# add_executable(app src/main.cpp)')
    L.append(f'# target_link_libraries(app PRIVATE {target})')
    L.append('')
    return '\n'.join(L)


def build_arg_parser() -> argparse.ArgumentParser:
    ap = argparse.ArgumentParser(description='Java → C++17 transpiler (object arrays use Ptr<T> elements)')

    ap.add_argument('input', nargs='?', default=None, help='Input Java file (omit when using --dir)')
    ap.add_argument('-o', '--output', default=None, help='Output C++ file (monolith) OR output root when --pair/--dir')
    ap.add_argument('--pair', action='store_true', help='For single input, emit header+source pair under --out root')
    ap.add_argument('--dir', default=None, help='Directory to recursively search for .java files')
    ap.add_argument('--out', default=None, help='Output root for --pair/--dir')

    ap.add_argument('--primitive-map', action='append', default=[], help='Override primitive mapping, e.g. int=jint')

    ap.add_argument('--string-policy', choices=['preserve', 'std'], default='preserve')
    ap.add_argument('--string-include', default=None)
    ap.add_argument('--bytearray-include', default=None)
    ap.add_argument('--include', action='append', default=[])
    ap.add_argument('--no-default-includes', action='store_true')

    ap.add_argument('--header-guards', action='store_true')
    ap.add_argument('--guard-name', default=None)
    ap.add_argument('--no-pragma-once', action='store_true')

    ap.add_argument('--new-macro-style', choices=['template', 'curried'], default='template')
    ap.add_argument('--new-macro', default='JXX_NEW')

    ap.add_argument('--cmake', action='store_true')
    ap.add_argument('--cmake-project', default='TranspiledProject')
    ap.add_argument('--cmake-target', default='transpiled')
    ap.add_argument('--cmake-kind', choices=['static', 'shared', 'object', 'interface'], default='static')
    ap.add_argument('--cmake-cxx-standard', default='17')
    ap.add_argument('--cmake-min', default='3.20')

    return ap


def make_transpiler(args) -> Transpiler:
    return Transpiler(
        string_policy=args.string_policy,
        string_include=args.string_include,
        bytearray_include=args.bytearray_include,
        extra_includes=args.include,
        no_default_includes=args.no_default_includes,
        pragma_once=(not args.no_pragma_once),
        header_guards=args.header_guards,
        guard_name=args.guard_name,
        new_macro_style=args.new_macro_style,
        new_macro=args.new_macro,
        primitive_map=args.primitive_map,
    )


def handle_single_file_monolith(tp: Transpiler, in_path: str, out_path: Optional[str]) -> None:
    with open(in_path, 'r', encoding='utf-8') as f:
        src = f.read()
    text = tp.transpile_unit(src, mode='monolith')
    if out_path:
        with open(out_path, 'w', encoding='utf-8') as f:
            f.write(text)
        print(f'Wrote {out_path}')
    else:
        sys.stdout.write(text)


def handle_single_file_pair(tp: Transpiler, in_path: str, out_root: str) -> Tuple[str, str]:
    with open(in_path, 'r', encoding='utf-8') as f:
        src = f.read()
    pkg = java_package_of(src)
    stem = os.path.splitext(os.path.basename(in_path))[0]

    header_path = path_for_file(out_root, 'include', pkg, stem, '.h')
    source_path = path_for_file(out_root, 'src', pkg, stem, '.cpp')

    guard = tp.guard_name or (tp._compute_guard_from_path(header_path) if tp.header_guards else None)

    text_h = tp.transpile_unit(src, mode='header', header_guard_name=guard)
    with open(header_path, 'w', encoding='utf-8') as f:
        f.write(text_h)

    rel = rel_include(source_path, header_path)
    text_cc = tp.transpile_unit(src, mode='source', header_rel_include=rel)
    with open(source_path, 'w', encoding='utf-8') as f:
        f.write(text_cc)

    print(f'Wrote {header_path}\nWrote {source_path}')
    return header_path, source_path


def handle_directory(tp: Transpiler, src_dir: str, out_root: str) -> None:
    for root, _, files in os.walk(src_dir):
        for fn in files:
            if fn.lower().endswith('.java'):
                handle_single_file_pair(tp, os.path.join(root, fn), out_root)


def main() -> None:
    ap = build_arg_parser()
    args = ap.parse_args()

    if (args.dir or args.pair) and not args.out:
        ap.error('--out is required when using --dir or --pair')

    if javalang is None:
        print("ERROR: This script requires 'javalang'. Install with: pip install javalang", file=sys.stderr)
        sys.exit(2)

    tp = make_transpiler(args)

    if args.dir:
        handle_directory(tp, args.dir, args.out)
        if args.cmake:
            cmk = generate_cmake(args.out, project=args.cmake_project, target=args.cmake_target,
                                 kind=args.cmake_kind, cxx_standard=args.cmake_cxx_standard, cmake_min=args.cmake_min)
            cmake_path = os.path.join(args.out, 'CMakeLists.txt')
            with open(cmake_path, 'w', encoding='utf-8') as f:
                f.write(cmk)
            print(f'Wrote {cmake_path}')
        return

    if not args.input:
        ap.error('Please provide an input Java file or use --dir.')

    if args.pair:
        handle_single_file_pair(tp, args.input, args.out)
        return

    handle_single_file_monolith(tp, args.input, args.output)


if __name__ == '__main__':
    main()
