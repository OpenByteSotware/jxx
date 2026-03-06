#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
java2cpp17.py

Java → C++17 transpiler (AST-based via `javalang`) with:
  • Imports: java.* → jxx::* (using / using namespace)
  • String policy: preserve Java `String` as C++ `String` (or std::string via flag)
  • byte[]: params & initialized locals by ref (ByteArray&); multi-dim → nested std::vector<ByteArray>
             fields configurable: --bytearray-fields {value|ref|ref-ctor}
  • Generics: class/interface/method templates; type args; wildcards annotated
  • Exceptions: throw new X → throw X(...); try/catch; method throws via macro/comment/off
                multi-catch / untyped → catch(Exception& e) (base configurable)
  • synchronized(expr) → SYNC_MACRO(expr) { ... } (default macro: JXX_SYNCHRONZIED)
  • Enums: simple → enum class + companion meta (values()/valueOf()); rich → class-backed with statics
           calls to Enum.values()/valueOf() rewritten to correct static target
  • Nested types supported (classes, interfaces, enums) with fully-qualified out-of-class definitions
  • **Directory transpile mode**: walk a source tree of .java, emit per-file **.h** (declarations) and **.cpp** (definitions)
        - Headers get **#pragma once** (toggle with --no-pragma-once)
        - Layout mirrors Java package into subfolders under --out/include and --out/src
        - For **templates**, definitions stay in header (standard C++)
        - For **non-templates**, method/ctor bodies move to .cpp with fully-qualified names
  • Header include flags: --string-include, --bytearray-include, --exceptions-include (repeat), --sync-include,
                          generic --include (repeat), and --no-default-includes
  • **CMake generation**: --cmake emits a top-level CMakeLists.txt (library target by default)
        - Options: --cmake-project, --cmake-target, --cmake-kind, --cmake-cxx-standard, --cmake-min

Usage examples:
  # Transpile a single file to stdout (monolithic unit):
  python java2cpp17.py Input.java

  # Transpile a single file into a header/impl pair under an output root:
  python java2cpp17.py Input.java --out ./out --pair

  # Directory mode: mirror packages to include/ and src/ with split headers/impls and CMake
  python java2cpp17.py --dir ./src_java --out ./cpp_out --cmake \
      --string-include "String.hpp" --bytearray-include "ByteArray.hpp" \
      --exceptions-include "Exception.hpp" --exceptions-include "IOException.hpp" \
      --sync-include "Sync.hpp" --checked-exceptions macro --throws-macro JTHROWS \
      --cmake-project TranspiledProj --cmake-target transpiled --cmake-kind static --cmake-cxx-standard 17

Note: Requires `pip install javalang` in the environment where you run it.
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
    "byte": "std::int8_t",
    "short": "short",
    "int": "int",
    "long": "long long",
    "float": "float",
    "double": "double",
    "boolean": "bool",
    "char": "char",
    "void": "void",
}

class Emit:
    def __init__(self) -> None:
        self.lines: List[str] = []
        self.indent_level = 0
    def write(self, line: str = "") -> None:
        if line:
            self.lines.append(("    " * self.indent_level) + line)
        else:
            self.lines.append(line)
    def enter(self) -> None:
        self.indent_level += 1
    def exit(self) -> None:
        self.indent_level = max(0, self.indent_level - 1)
    def text(self) -> str:
        return "\n".join(self.lines)

# ---------- helpers ----------

def _base_type_name_and_dims(t) -> Tuple[str,int]:
    if t is None: return ("void",0)
    if isinstance(t,str): return (t,0)
    name = ".".join(t.name) if isinstance(t.name,(list,tuple)) else t.name
    dims = len(t.dimensions or [])
    return (name,dims)

def _cpp_type_for_byte_dims(total_dims: int) -> str:
    t = "ByteArray"
    for _ in range(max(0,total_dims-1)):
        t = f"std::vector<{t}>"
    return t

class Transpiler:
    def __init__(self,
                 field_mode: str = "value",
                 checked_ex_mode: str = "macro",
                 throws_macro: str = "JTHROWS",
                 string_policy: str = "preserve",
                 sync_macro: str = "JXX_SYNCHRONZIED",
                 exception_base: str = "Exception",
                 enum_valueof_exception: str = "IllegalArgumentException",
                 string_include: Optional[str] = None,
                 bytearray_include: Optional[str] = None,
                 exceptions_includes: Optional[List[str]] = None,
                 sync_include: Optional[str] = None,
                 extra_includes: Optional[List[str]] = None,
                 no_default_includes: bool = False,
                 pragma_once: bool = True) -> None:
        self.field_mode = field_mode
        self.checked_ex_mode = checked_ex_mode
        self.throws_macro = throws_macro
        self.string_policy = string_policy
        self.sync_macro = sync_macro
        self.exception_base = exception_base
        self.enum_valueof_exception = enum_valueof_exception
        self.string_include = string_include
        self.bytearray_include = bytearray_include
        self.exceptions_includes = list(exceptions_includes or [])
        self.sync_include = sync_include
        self.extra_includes = list(extra_includes or [])
        self.no_default_includes = no_default_includes
        self.pragma_once = pragma_once
        self.using_lines: Set[str] = set()
        self.enums_by_java_qual: Dict[str,dict] = {}
        self.enums_by_unqual: Dict[str,dict] = {}

    # ---------- includes ----------
    @staticmethod
    def _norm_inc(s: str) -> str:
        s = (s or "").strip()
        if not s: return s
        if s.startswith('<') or s.startswith('"'): return s
        return f'"{s}"'

    def emit_includes(self, out: Emit, for_header: bool) -> None:
        ordered: List[str] = []
        if self.string_include: ordered.append(self.string_include)
        if self.bytearray_include: ordered.append(self.bytearray_include)
        for exc in self.exceptions_includes: ordered.append(exc)
        if self.sync_include: ordered.append(self.sync_include)
        for inc in self.extra_includes: ordered.append(inc)
        if not self.no_default_includes:
            ordered += ["<iostream>","<vector>","<array>"]
            if self.string_policy == "std": ordered.append("<string>")
        seen = set()
        for inc in ordered:
            op = self._norm_inc(inc)
            if op and op not in seen:
                seen.add(op)
                out.write(f"#include {op}")

    # ---------- imports ----------
    @staticmethod
    def _looks_pkg(parts: List[str], wildcard: bool) -> bool:
        if wildcard: return True
        return not parts or (parts[-1] and parts[-1][0].islower())

    def gather_usings(self, tree) -> None:
        self.using_lines.clear()
        for imp in (tree.imports or []):
            path = imp.path
            if not path or not path.startswith("java."): continue
            rest = path[len("java."):]
            segs = rest.split('.') if rest else []
            if imp.wildcard:
                ns = "::".join(["jxx"] + segs)
                self.using_lines.add(f"using namespace {ns};")
            else:
                cxx = "::".join(["jxx"] + segs) if segs else "jxx"
                if self._looks_pkg(segs, False):
                    self.using_lines.add(f"using namespace {cxx};")
                else:
                    self.using_lines.add(f"using {cxx};")

    # ---------- type mapping & generics ----------
    def _render_type_args(self, args) -> str:
        if not args: return ""
        acc: List[str] = []
        for a in args:
            an = type(a).__name__
            if an == "TypeArgument":
                if getattr(a, 'wildcard', False):
                    if a.pattern_type is not None:
                        b = self._map_ref_or_basic(a.pattern_type)
                        if getattr(a,'kind',None) == 'super': acc.append(f"{b} /* ? super */")
                        else: acc.append(f"{b} /* ? extends */")
                    else:
                        acc.append("/* ? */")
                elif a.type is not None:
                    acc.append(self._map_ref_or_basic(a.type))
                else:
                    acc.append("/* ? */")
            else:
                acc.append(str(a))
        return "<" + ", ".join(acc) + ">"

    def _map_ref_or_basic(self, t) -> str:
        tn = type(t).__name__
        if tn == "BasicType":
            return JAVA_TO_CPP_TYPES.get(t.name, t.name)
        if tn == "ReferenceType":
            raw = t.name.replace('.', '::') if isinstance(t.name,str) else str(t.name)
            if raw == "String": raw = ("String" if self.string_policy == "preserve" else "std::string")
            return f"{raw}{self._render_type_args(getattr(t,'arguments',None))}"
        if isinstance(t,str):
            if t == "String": return ("String" if self.string_policy == "preserve" else "std::string")
            return JAVA_TO_CPP_TYPES.get(t,t)
        return str(t)

    def _map_non_byte(self, t, extra_dims: int = 0) -> str:
        base = self._map_ref_or_basic(t)
        _, d0 = _base_type_name_and_dims(t)
        for _ in range(d0 + (extra_dims or 0)):
            base = f"std::vector<{base}>"
        return base

    def _string_param_t(self) -> str:
        return "const String&" if self.string_policy == "preserve" else "const std::string&"

    def _string_lit(self, s: str) -> str:
        return f"String({s})" if self.string_policy == "preserve" else s

    # ---------- enum registry ----------
    def _is_simple_enum(self, en) -> bool:
        body = list(getattr(en,'body',[]) or [])
        if any(type(x).__name__ in {"FieldDeclaration","MethodDeclaration","ConstructorDeclaration","ClassDeclaration","InterfaceDeclaration","EnumDeclaration"} for x in body):
            return False
        for c in (getattr(en,'constants',[]) or []):
            if getattr(c,'arguments',None) or getattr(c,'class_body',None): return False
        return True

    def _scan_enums(self, types, scope_java: List[str], scope_cxx: List[str]):
        for t in types or []:
            tn = type(t).__name__
            if tn == "EnumDeclaration":
                name = t.name
                jqual = ".".join(scope_java+[name]) if scope_java else name
                cqual = "::".join(scope_cxx+[name]) if scope_cxx else name
                consts = [c.name for c in (t.constants or [])]
                kind = "simple" if self._is_simple_enum(t) else "rich"
                info = {"kind":kind,"java_qual":jqual,"cxx_qual":cqual,"constants":consts}
                self.enums_by_java_qual[jqual] = info
                self.enums_by_unqual.setdefault(name, info)
                self._scan_enums(getattr(t,'body',None), scope_java+[name], scope_cxx+[name])
            elif tn in ("ClassDeclaration","InterfaceDeclaration"):
                name = t.name
                self._scan_enums(getattr(t,'body',None), scope_java+[name], scope_cxx+[name])

    def _lookup_enum(self, qual_or_unqual: str) -> Optional[dict]:
        if qual_or_unqual in self.enums_by_java_qual: return self.enums_by_java_qual[qual_or_unqual]
        return self.enums_by_unqual.get(qual_or_unqual.split('.')[-1])

    # ---------- emit expr ----------
    def emit_expression(self, e) -> str:
        if e is None: return ""
        tn = type(e).__name__
        if tn == "Literal":
            return "nullptr" if e.value is None else e.value
        if tn == "BinaryOperation":
            return f"({self.emit_expression(e.operandl)} {e.operator} {self.emit_expression(e.operandr)})"
        if tn == "UnaryOperation":
            inner = self.emit_expression(e.expression)
            return f"({inner}{e.operator})" if getattr(e,'postfix',False) else f"({e.operator}{inner})"
        if tn == "TernaryExpression":
            return f"({self.emit_expression(e.condition)} ? {self.emit_expression(e.if_true)} : {self.emit_expression(e.if_false)})"
        if tn == "MemberReference":
            q = e.qualifier
            if isinstance(q,str): return f"{q}.{e.member}" if q else e.member
            if q is not None: return f"{self.emit_expression(q)}.{e.member}"
            return e.member
        if tn == "ReferenceType":
            return self._map_non_byte(e)
        if tn == "This": return "this"
        if tn == "ClassReference": return f"typeid({e.type.name})"
        if tn == "MethodInvocation":
            qual = e.qualifier
            qstr = qual if isinstance(qual,str) else (self.emit_expression(qual) if qual is not None else "")
            member = e.member
            args = [self.emit_expression(a) for a in (e.arguments or [])]
            # Enum rewrite
            if qstr and member in {"values","valueOf"}:
                info = self._lookup_enum(qstr)
                if info:
                    cxxq = qstr.replace('.', '::')
                    target = f"{cxxq}__meta::{member}" if info['kind'] == 'simple' else f"{cxxq}::{member}"
                    return f"{target}({', '.join(args)})"
            # System.out
            if ((qstr == "System.out" or qstr.endswith("System.out")) and member in {"print","println"}):
                if not args: return "std::cout << std::endl"
                tail = " << std::endl" if member == "println" else ""
                return f"(std::cout << {' << '.join(args)}{tail})"
            call = f"{qstr}.{member}" if qstr else member
            return f"{call}({', '.join(args)})" if args else f"{call}()"
        if tn == "ClassCreator":
            name = self._map_ref_or_basic(e.type)
            args = [self.emit_expression(a) for a in (e.arguments or [])]
            return f"{name}({', '.join(args)})"
        if tn == "ArrayCreator":
            base = self._map_ref_or_basic(e.type)
            dims = len(e.dimensions or [])
            if dims == 1 and e.dimensions[0] is not None:
                n = self.emit_expression(e.dimensions[0])
                return f"std::vector<{base}>({n})"
            return f"/* TODO: array creation for {base} with {dims} dims */"
        if tn == "ThrowStatement":
            return f"throw {self.emit_expression(e.expression)}"
        if tn == "ArraySelector":
            return f"{self.emit_expression(e.primary)}[{self.emit_expression(e.index)}]"
        if tn == "Assignment":
            l = self.emit_expression(e.expressionl); r = self.emit_expression(e.value); op = e.type if e.type else "="
            return f"({l} {op} {r})"
        if tn == "Cast":
            t = self._map_non_byte(e.type)
            return f"(({t}){self.emit_expression(e.expression)})"
        if tn == "VariableDeclarator":
            init = f" = {self.emit_expression(e.initializer)}" if e.initializer is not None else ""
            return f"{e.name}{init}"
        return str(e)

    # ---------- emit stmt ----------
    def emit_statement(self, s, out: Emit) -> None:
        if s is None: return
        tn = type(s).__name__
        if tn == "ReturnStatement": out.write(f"return {self.emit_expression(s.expression)};"); return
        if tn == "StatementExpression": out.write(f"{self.emit_expression(s.expression)};"); return
        if tn == "LocalVariableDeclaration":
            base, d0 = _base_type_name_and_dims(s.type)
            for d in (s.declarators or []):
                extra = len(getattr(d,'dimensions',[]) or [])
                total = d0 + extra
                name = d.name
                init = self.emit_expression(d.initializer) if d.initializer is not None else None
                if base == 'byte' and total >= 1:
                    cxx = _cpp_type_for_byte_dims(total)
                    if init is not None: out.write(f"{cxx}& {name} = {init};")
                    else: out.write(f"{cxx} {name};  // NOTE: cannot bind reference without initializer")
                else:
                    t = self._map_non_byte(s.type, extra_dims=extra)
                    decl = name + (f" = {init}" if init is not None else "")
                    out.write(f"{t} {decl};")
            return
        if tn == "BlockStatement":
            for st in (s.statements or []): self.emit_statement(st,out); return
        if tn == "IfStatement":
            out.write(f"if ({self.emit_expression(s.condition)}) {{"); out.enter(); self.emit_statement(s.then_statement,out); out.exit(); out.write("}")
            if s.else_statement is not None: out.write("else {"); out.enter(); self.emit_statement(s.else_statement,out); out.exit(); out.write("}")
            return
        if tn == "WhileStatement": out.write(f"while ({self.emit_expression(s.condition)}) {{"); out.enter(); self.emit_statement(s.body,out); out.exit(); out.write("}"); return
        if tn == "ForStatement":
            init = []
            if s.init:
                for i in s.init:
                    if type(i).__name__ == 'LocalVariableDeclaration':
                        tmp = Emit(); self.emit_statement(i,tmp)
                        for ln in tmp.lines:
                            ln=ln.strip();
                            if ln: init.append(ln.rstrip(';'))
                    else:
                        init.append(self.emit_expression(i))
            cond = self.emit_expression(s.condition) if s.condition else ""
            upd = ", ".join(self.emit_expression(u) for u in (s.update or []))
            out.write(f"for ({', '.join(init)}; {cond}; {upd}) {{"); out.enter(); self.emit_statement(s.body,out); out.exit(); out.write("}")
            return
        if tn == "TryStatement":
            out.write("try {"); out.enter(); self.emit_statement(s.block,out); out.exit(); out.write("}")
            for c in (s.catches or []):
                p = c.parameter; tlist = list(getattr(p,'types',[]) or []); name = p.name or 'e'
                if not tlist or len(tlist) > 1:
                    listed = ""
                    if tlist:
                        try: listed = " | ".join(self._map_non_byte(t) for t in tlist)
                        except Exception: listed = " | ".join(str(t) for t in tlist)
                    out.write(f"catch ({self.exception_base}& {name}) {{"); out.enter()
                    if listed: out.write(f"// original: {listed}")
                    self.emit_statement(c.block,out); out.exit(); out.write("}")
                else:
                    ct = self._map_non_byte(tlist[0])
                    out.write(f"catch ({ct}& {name}) {{"); out.enter(); self.emit_statement(c.block,out); out.exit(); out.write("}")
            if s.finally_block is not None:
                out.write("// finally { ... }"); out.write("{"); out.enter(); self.emit_statement(s.finally_block,out); out.exit(); out.write("}")
            return
        if tn == "SynchronizedStatement":
            lock = getattr(s,'lock',None) or getattr(s,'expression',None)
            le = self.emit_expression(lock) if lock is not None else ""
            out.write(f"{self.sync_macro}({le}) {{"); out.enter(); self.emit_statement(s.block,out); out.exit(); out.write("}")
            return
        if tn == "SwitchStatement":
            out.write(f"switch ({self.emit_expression(s.expression)}) {{"); out.enter()
            for case in (s.cases or []):
                if case.case is None: out.write("default:")
                else: out.write(f"case {self.emit_expression(case.case)}:")
                out.enter();
                for st in (case.statements or []): self.emit_statement(st,out)
                out.write("break;"); out.exit()
            out.exit(); out.write("}")
            return
        out.write(f"/* TODO: Unhandled statement: {tn} */")

    # ---------- fields ----------
    def _field_decl_is_byte(self, ftype, decl) -> Tuple[bool,int]:
        base, d0 = _base_type_name_and_dims(ftype)
        extra = len(getattr(decl,'dimensions',[]) or [])
        return (base=='byte' and d0+extra>=1, d0+extra)

    def emit_field(self, f, out: Emit, ref_fields_accum: List[Tuple[str,str]]):
        for d in (f.declarators or []):
            is_byte, total = self._field_decl_is_byte(f.type, d)
            name = d.name
            if is_byte:
                cxx = _cpp_type_for_byte_dims(total)
                if self.field_mode == 'value': out.write(f"{cxx} {name};")
                elif self.field_mode in ('ref','ref-ctor'):
                    out.write(f"{cxx}& {name};");
                    if self.field_mode == 'ref-ctor': ref_fields_accum.append((name,f"{cxx}&"))
                else: out.write(f"{cxx} {name};")
            else:
                t = self._map_non_byte(f.type, extra_dims=len(getattr(d,'dimensions',[]) or []))
                init = f" = {self.emit_expression(d.initializer)}" if d.initializer is not None else ""
                out.write(f"{t} {name}{init};")

    # ---------- templates / throws ----------
    def _render_type_params(self, tparams) -> str:
        if not tparams: return ""
        parts = []
        for tp in tparams:
            bound = ""
            if getattr(tp,'bounds',None): bound = " /* extends " + ", ".join(self._map_ref_or_basic(b) for b in tp.bounds) + " */"
            parts.append(f"typename {tp.name}{bound}")
        return "template<" + ", ".join(parts) + ">"

    def _throws_suffix(self, throws_types) -> Tuple[str,str]:
        if not throws_types: return ("","")
        try: listing = ", ".join(self._map_ref_or_basic(t) for t in throws_types)
        except Exception: listing = ", ".join(str(t) for t in throws_types)
        if self.checked_ex_mode == 'macro': return (f" {self.throws_macro}({listing})","")
        if self.checked_ex_mode == 'comment': return ("", f"  // throws: {listing}")
        return ("","")

    # ---------- method sig + emit (header/source split) ----------
    def _inline_in_header(self, owner_tpl: bool, m) -> bool:
        if owner_tpl: return True
        if getattr(m,'type_parameters',None): return True
        return False

    def _method_signature(self, m, class_name: str, in_interface: bool) -> Tuple[str,bool]:
        is_ctor = (type(m).__name__ == 'ConstructorDeclaration')
        if not is_ctor:
            if m.return_type is not None:
                base, d0 = _base_type_name_and_dims(m.return_type)
                md = len(getattr(m,'dimensions',[]) or [])
                total = d0 + md
                if base=='byte' and total>=1: ret = _cpp_type_for_byte_dims(total)
                else: ret = self._map_non_byte(m.return_type, extra_dims=md)
            else:
                ret = 'void'
        else:
            ret = ''
        name = m.name if not is_ctor else class_name
        params: List[str] = []
        for p in (m.parameters or []):
            b, d0 = _base_type_name_and_dims(p.type)
            total = d0
            if b=='byte' and total>=1: params.append(f"{_cpp_type_for_byte_dims(total)}& {p.name}")
            else: params.append(f"{self._map_non_byte(p.type)} {p.name}")
        sig = (f"{ret} {name}({', '.join(params)})" if not is_ctor else f"{name}({', '.join(params)})")
        return sig, is_ctor

    def emit_method_header_or_body(self, m, out: Emit, class_name: str, in_interface: bool, owner_tpl: bool, emit_body: bool) -> None:
        tpl = self._render_type_params(getattr(m,'type_parameters',None))
        sig, is_ctor = self._method_signature(m, class_name, in_interface)
        suff, throws_comment = self._throws_suffix(getattr(m,'throws',None))
        if in_interface:
            if getattr(m,'body',None) and (owner_tpl or emit_body):
                if tpl: out.write(tpl)
                out.write(f"virtual {sig}{suff} {{"); out.enter()
                for st in m.body: self.emit_statement(st,out)
                out.exit(); out.write("}")
            else:
                decl = f"virtual {sig}{suff} = 0;"
                if throws_comment: decl += throws_comment
                out.write(decl)
            return
        if emit_body:
            if tpl: out.write(tpl)
            out.write(f"{sig}{suff} {{"); out.enter()
            if throws_comment: out.write(throws_comment.strip())
            for st in (m.body or []): self.emit_statement(st,out)
            out.exit(); out.write("}")
        else:
            if tpl: out.write(tpl)
            out.write(f"{sig}{suff};")

    # ---------- enums emit ----------
    def _emit_simple_enum_with_helpers(self, en, out: Emit) -> None:
        name = en.name
        consts = [c.name for c in (en.constants or [])]
        out.write(f"enum class {name} {{ {', '.join(consts)} }};")
        out.write(f"struct {name}__meta {{"); out.enter()
        out.write(f"static inline constexpr std::array<{name}, {len(consts)}> values() {{")
        out.enter(); out.write(f"return {{ {', '.join(f'{name}::{c}' for c in consts)} }};"); out.exit(); out.write("}")
        sp = self._string_param_t()
        out.write(f"static inline {name} valueOf({sp} s) {{"); out.enter()
        for c in consts: out.write(f"if (s == {self._string_lit(f'\"{c}\"')}) return {name}::{c};")
        out.write(f"throw {self.enum_valueof_exception}(\"No enum const {name}.\" /* + s */);")
        out.exit(); out.write("}")
        out.exit(); out.write("};")

    def _emit_rich_enum_header(self, en, out: Emit, emit_bodies_in_header: bool) -> None:
        name = en.name
        bases: List[str] = []
        if getattr(en,'implements',None):
            for i in en.implements: bases.append(self._map_non_byte(i))
        clause = (" : " + ", ".join(f"public {b}" for b in bases)) if bases else ""
        out.write(f"class {name}{clause} {{"); out.enter()
        out.write("public:"); out.enter()
        consts = [c.name for c in (en.constants or [])]
        for c in (en.constants or []):
            args = [self.emit_expression(a) for a in (c.arguments or [])]
            init = f"{name}({', '.join(args)})" if args else f"{name}()"
            out.write(f"inline static const {name} {c.name} = {init};")
            if getattr(c,'class_body',None): out.write(f"// NOTE: per-constant class body for {c.name} not translated")
        if emit_bodies_in_header:
            out.write(f"static inline std::array<{name}, {len(consts)}> values() {{"); out.enter();
            out.write(f"return {{ {', '.join(f'{name}::{c}' for c in consts)} }};"); out.exit(); out.write("}")
            sp = self._string_param_t(); out.write(f"static inline {name} valueOf({sp} s) {{")
            out.enter();
            for c in consts: out.write(f"if (s == {self._string_lit(f'\"{c}\"')}) return {name}::{c};")
            out.write(f"throw {self.enum_valueof_exception}(\"No enum const {name}.\" /* + s */);")
            out.exit(); out.write("}")
        else:
            out.write(f"static std::array<{name}, {len(consts)}> values();")
            out.write(f"static {name} valueOf({self._string_param_t()} s);")
        out.exit()
        def acc(mods:set)->str:
            if 'public' in mods: return 'public'
            if 'protected' in mods: return 'protected'
            return 'private'
        current=None; ref_fields: List[Tuple[str,str]] = []
        for m in (en.body or []):
            mt = type(m).__name__
            desired = 'private' if mt=='ConstructorDeclaration' else acc(set(getattr(m,'modifiers',[]) or []))
            if desired != current:
                if current is not None: out.exit()
                out.write(f"{desired}:"); out.enter(); current=desired
            if mt == 'FieldDeclaration': self.emit_field(m,out,ref_fields)
            elif mt in ('MethodDeclaration','ConstructorDeclaration'):
                self.emit_method_header_or_body(m,out,name,in_interface=False,owner_tpl=False,emit_body=emit_bodies_in_header)
            elif mt in ('ClassDeclaration','InterfaceDeclaration','EnumDeclaration'):
                self.emit_type_header(m,out)
            else:
                out.write(f"/* TODO: Unhandled enum member: {mt} */")
        if current is not None: out.exit()
        out.exit(); out.write("}"); out.write(";")

    # ---------- type header ----------
    def emit_type_header(self, t, out: Emit) -> None:
        tn = type(t).__name__
        if tn == 'EnumDeclaration':
            if self._is_simple_enum(t): self._emit_simple_enum_with_helpers(t,out)
            else: self._emit_rich_enum_header(t,out,emit_bodies_in_header=False)
            return
        if tn not in ('ClassDeclaration','InterfaceDeclaration'):
            out.write(f"/* TODO: Unhandled type: {tn} */"); return
        tpl = self._render_type_params(getattr(t,'type_parameters',None))
        if tpl: out.write(tpl)
        is_iface = (tn == 'InterfaceDeclaration')
        kw = 'struct' if is_iface else 'class'
        name = t.name
        bases: List[str] = []
        if getattr(t,'extends',None):
            ext = t.extends if isinstance(t.extends,list) else [t.extends]
            for b in ext: bases.append(self._map_non_byte(b))
        if getattr(t,'implements',None):
            for i in t.implements: bases.append(self._map_non_byte(i))
        clause = (" : " + ", ".join(f"public {b}" for b in bases)) if bases else ""
        out.write(f"{kw} {name}{clause} {{"); out.enter()
        def acc(mods:set)->str:
            if 'public' in mods: return 'public'
            if 'protected' in mods: return 'protected'
            return 'private'
        current=None; ref_fields: List[Tuple[str,str]] = []
        owner_tpl = bool(getattr(t,'type_parameters',None))
        for m in (t.body or []):
            mt = type(m).__name__
            mods = set(getattr(m,'modifiers',[]) or [])
            desired = acc(mods)
            if desired != current:
                if current is not None: out.exit()
                out.write(f"{desired}:"); out.enter(); current=desired
            if mt == 'FieldDeclaration': self.emit_field(m,out,ref_fields)
            elif mt in ('MethodDeclaration','ConstructorDeclaration'):
                inline_here = self._inline_in_header(owner_tpl, m)
                self.emit_method_header_or_body(m,out,name,in_interface=is_iface,owner_tpl=owner_tpl,emit_body=inline_here)
            elif mt in ('ClassDeclaration','InterfaceDeclaration','EnumDeclaration'):
                self.emit_type_header(m,out)
            else:
                out.write(f"/* TODO: Unhandled member: {mt} */")
        if current is not None: out.exit()
        if not is_iface and self.field_mode == 'ref-ctor' and ref_fields:
            out.write('public:'); out.enter()
            params = ", ".join(f"{typ} {nm}" for (nm,typ) in ref_fields)
            inits = ", ".join(f"{nm}({nm})" for (nm,_) in ref_fields)
            out.write(f"{name}({params}) : {inits} {{}}")
            out.exit()
        out.exit(); out.write('}'); out.write(';')

    # ---------- type source (non-template out-of-class defs; nested supported) ----------
    def _qstack(self, stack: List[str]) -> str:
        return '::'.join(stack)

    def emit_type_source(self, t, out: Emit, ns_stack: List[str], type_stack: List[str]) -> None:
        tn = type(t).__name__
        if tn == 'EnumDeclaration':
            if not self._is_simple_enum(t):
                name = t.name; fq = self._qstack(type_stack+[name]); consts = [c.name for c in (t.constants or [])]
                out.write(f"std::array<{fq}, {len(consts)}> {fq}::values() {{"); out.enter();
                out.write(f"return {{ {', '.join(f'{fq}::{c}' for c in consts)} }};"); out.exit(); out.write('}')
                sp = self._string_param_t();
                out.write(f"{fq} {fq}::valueOf({sp} s) {{"); out.enter()
                for c in consts: out.write(f"if (s == {self._string_lit(f'\"{c}\"')}) return {fq}::{c};")
                out.write(f"throw {self.enum_valueof_exception}(\"No enum const {name}.\" /* + s */);")
                out.exit(); out.write('}')
            # recurse nested
            for m in (t.body or []):
                if type(m).__name__ in ('ClassDeclaration','InterfaceDeclaration','EnumDeclaration'):
                    self.emit_type_source(m, out, ns_stack, type_stack+[t.name])
            return
        if tn not in ('ClassDeclaration','InterfaceDeclaration'): return
        owner_tpl = bool(getattr(t,'type_parameters',None))
        name = t.name; fq = self._qstack(type_stack+[name])
        for m in (t.body or []):
            mt = type(m).__name__
            if mt in ('MethodDeclaration','ConstructorDeclaration'):
                if self._inline_in_header(owner_tpl, m):
                    continue
                sig, is_ctor = self._method_signature(m, name, in_interface=(tn=='InterfaceDeclaration'))
                if is_ctor:
                    qsig = sig.replace(name+'(', fq+'(')
                else:
                    parts = sig.split(' ',1)
                    if len(parts)==2:
                        ret, rest = parts
                        qsig = f"{ret} {fq}::{rest}"
                    else:
                        qsig = f"{fq}::{sig}"
                out.write(qsig + ' {'); out.enter()
                for st in (m.body or []): self.emit_statement(st,out)
                out.exit(); out.write('}')
            elif mt in ('ClassDeclaration','InterfaceDeclaration','EnumDeclaration'):
                self.emit_type_source(m, out, ns_stack, type_stack+[name])

    # ---------- unit drivers ----------
    def transpile_unit(self, java_src: str, mode: str = 'monolith', header_rel_include: Optional[str] = None) -> Tuple[str,str]:
        if javalang is None: raise RuntimeError("javalang is required. pip install javalang")
        tree = javalang.parse.parse(java_src)
        self._scan_enums(tree.types, [], [])
        self.gather_usings(tree)
        ns = tree.package.name.replace('.', '::') if tree.package else ''
        out = Emit()
        out.write('// Generated by java2cpp17 (AST-based)')
        if mode=='header' and self.pragma_once: out.write('#pragma once')
        if mode != 'source':
            self.emit_includes(out, for_header=True)
            out.write('// NOTE: Provide ByteArray/String/Exception/macro headers if not in PCH.')
        else:
            if not header_rel_include: raise ValueError('header_rel_include required for source mode')
            out.write(f"#include {self._norm_inc(header_rel_include)}")
            if self.extra_includes or self.exceptions_includes or self.sync_include or self.bytearray_include or self.string_include:
                self.emit_includes(out, for_header=False)
        out.write('')
        for line in sorted(self.using_lines): out.write(line)
        if self.using_lines: out.write('')
        if ns: out.write(f"namespace {ns} {{"); out.write(''); out.enter()
        if mode in ('monolith','header'):
            for t in (tree.types or []): self.emit_type_header(t,out); out.write('')
        elif mode == 'source':
            for t in (tree.types or []): self.emit_type_source(t,out, ns_stack=(ns.split('::') if ns else []), type_stack=[]); out.write('')
        if ns: out.exit(); out.write(f"}} // namespace {ns}")
        return out.text(), ns

# ---------- path helpers ----------

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
    return os.path.join(full_dir, f"{stem}{ext}")

def rel_include(from_file: str, header_file: str) -> str:
    return os.path.relpath(header_file, start=os.path.dirname(from_file)).replace(os.sep,'/')

# ---------- CMake generation ----------

def generate_cmake(out_root: str,
                    project: str = 'TranspiledProject',
                    target: str = 'transpiled',
                    kind: str = 'static',
                    cxx_standard: str = '17',
                    cmake_min: str = '3.20') -> str:
    kind_map = {
        'static': 'STATIC',
        'shared': 'SHARED',
        'object': 'OBJECT',
        'interface': 'INTERFACE',
    }
    kind_kw = kind_map.get(kind.lower(), 'STATIC')
    include_dir = os.path.join(out_root, 'include').replace('\\','/')
    src_dir = os.path.join(out_root, 'src').replace('\\','/')

    # Gather sources
    cpp_files = []
    for root, dirs, files in os.walk(os.path.join(out_root, 'src')):
        for fn in files:
            if fn.lower().endswith('.cpp'):
                cpp_files.append(os.path.join(root, fn).replace('\\','/'))
    rel_cpp = [os.path.relpath(p, start=out_root).replace('\\','/') for p in cpp_files]

    lines = []
    lines.append(f"cmake_minimum_required(VERSION {cmake_min})")
    lines.append(f"project({project} LANGUAGES CXX)")
    lines.append("")
    lines.append(f"set(CMAKE_CXX_STANDARD {cxx_standard})")
    lines.append("set(CMAKE_CXX_STANDARD_REQUIRED ON)")
    lines.append("set(CMAKE_CXX_EXTENSIONS OFF)")
    lines.append("")
    # Create target
    if kind_kw == 'INTERFACE':
        lines.append(f"add_library({target} INTERFACE)")
        lines.append(f"target_include_directories({target} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)")
    else:
        if rel_cpp:
            lines.append(f"add_library({target} {kind_kw}\n    " + "\n    ".join(rel_cpp) + "\n)")
        else:
            # Fallback: glob in CMake (slower re-configs but safe default)
            lines.append(f"file(GLOB_RECURSE {target}_SRCS CONFIGURE_DEPENDS src/*.cpp)")
            lines.append(f"add_library({target} {kind_kw} ${target}_SRCS)")
        lines.append(f"target_include_directories({target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)")
    lines.append("")
    lines.append("# Optionally:")
    lines.append("# add_executable(app src/main.cpp)")
    lines.append(f"# target_link_libraries(app PRIVATE {target})")
    lines.append("")
    return "\n".join(lines)

# ---------- CLI ----------

def build_arg_parser() -> argparse.ArgumentParser:
    ap = argparse.ArgumentParser(description="Java → C++17 AST transpiler (dir mode, header/source split, pragma once, CMake)")
    ap.add_argument('input', nargs='?', default=None, help='Input Java file (omit when using --dir)')
    ap.add_argument('-o','--output', default=None, help='Output C++ file (monolith) OR output root when --pair/--dir')
    ap.add_argument('--pair', action='store_true', help='For single input, emit header+source pair under --out root')
    ap.add_argument('--dir', default=None, help='Directory to recursively search for .java files (emits header/cpp pairs)')
    ap.add_argument('--out', default=None, help='Output root for --pair/--dir (required for those modes)')

    # Behavior
    ap.add_argument('--bytearray-fields', choices=['value','ref','ref-ctor'], default='value')
    ap.add_argument('--checked-exceptions', choices=['off','macro','comment'], default='macro')
    ap.add_argument('--throws-macro', default='JTHROWS')
    ap.add_argument('--string-policy', choices=['preserve','std'], default='preserve')
    ap.add_argument('--sync-macro', default='JXX_SYNCHRONZIED')
    ap.add_argument('--exception-base', default='Exception')
    ap.add_argument('--enum-valueof-exception', default='IllegalArgumentException')

    # Includes
    ap.add_argument('--string-include', default=None)
    ap.add_argument('--bytearray-include', default=None)
    ap.add_argument('--exceptions-include', action='append', default=[])
    ap.add_argument('--sync-include', default=None)
    ap.add_argument('--include', action='append', default=[])
    ap.add_argument('--no-default-includes', action='store_true')

    # Header options
    ap.add_argument('--no-pragma-once', action='store_true', help='Disable #pragma once in generated headers')

    # CMake generation
    ap.add_argument('--cmake', action='store_true', help='Generate a top-level CMakeLists.txt in --out')
    ap.add_argument('--cmake-project', default='TranspiledProject')
    ap.add_argument('--cmake-target', default='transpiled')
    ap.add_argument('--cmake-kind', choices=['static','shared','object','interface'], default='static')
    ap.add_argument('--cmake-cxx-standard', default='17')
    ap.add_argument('--cmake-min', default='3.20')

    return ap


def make_transpiler(args) -> Transpiler:
    return Transpiler(
        field_mode=args.bytearray_fields,
        checked_ex_mode=args.checked_exceptions,
        throws_macro=args.throws_macro,
        string_policy=args.string_policy,
        sync_macro=args.sync_macro,
        exception_base=args.exception_base,
        enum_valueof_exception=args.enum_valueof_exception,
        string_include=args.string_include,
        bytearray_include=args.bytearray_include,
        exceptions_includes=args.exceptions_include,
        sync_include=args.sync_include,
        extra_includes=args.include,
        no_default_includes=args.no_default_includes,
        pragma_once=(not args.no_pragma_once),
    )


def handle_single_file_monolith(tp: Transpiler, in_path: str, out_path: Optional[str]) -> None:
    with open(in_path,'r',encoding='utf-8') as f: src = f.read()
    text,_ = tp.transpile_unit(src, mode='monolith')
    if out_path:
        with open(out_path,'w',encoding='utf-8') as f: f.write(text)
        print(f"Wrote {out_path}")
    else:
        sys.stdout.write(text)


def handle_single_file_pair(tp: Transpiler, in_path: str, out_root: str) -> Tuple[str,str]:
    with open(in_path,'r',encoding='utf-8') as f: src = f.read()
    pkg = java_package_of(src)
    stem = os.path.splitext(os.path.basename(in_path))[0]
    header_path = path_for_file(out_root, 'include', pkg, stem, '.h')
    source_path = path_for_file(out_root, 'src', pkg, stem, '.cpp')
    text_h,_ = tp.transpile_unit(src, mode='header')
    with open(header_path,'w',encoding='utf-8') as f: f.write(text_h)
    rel = rel_include(source_path, header_path)
    text_cc,_ = tp.transpile_unit(src, mode='source', header_rel_include=rel)
    with open(source_path,'w',encoding='utf-8') as f: f.write(text_cc)
    print(f"Wrote {header_path}\nWrote {source_path}")
    return header_path, source_path


def handle_directory(tp: Transpiler, src_dir: str, out_root: str) -> Tuple[List[str],List[str]]:
    headers: List[str] = []
    sources: List[str] = []
    java_files: List[str] = []
    for root,dirs,files in os.walk(src_dir):
        for fn in files:
            if fn.lower().endswith('.java'):
                java_files.append(os.path.join(root,fn))
    if not java_files:
        print('No .java files found.')
        return headers, sources
    for jpath in java_files:
        try:
            h,cc = handle_single_file_pair(tp, jpath, out_root)
            headers.append(h); sources.append(cc)
        except Exception as ex:
            print(f"[WARN] Failed to transpile {jpath}: {ex}")
    return headers, sources


def main():
    ap = build_arg_parser(); args = ap.parse_args()

    if (args.dir or args.pair) and not args.out:
        ap.error('--out is required when using --dir or --pair')
    if args.input and args.dir:
        ap.error('Provide either a single input file OR --dir, not both.')

    if javalang is None:
        try:
            import javalang as _jl  # noqa
        except Exception:
            print("ERROR: This script requires 'javalang'. Install with: pip install javalang", file=sys.stderr)
            sys.exit(2)

    tp = make_transpiler(args)

    if args.dir:
        headers, sources = handle_directory(tp, args.dir, args.out)
        if args.cmake:
            cmk = generate_cmake(args.out, project=args.cmake_project, target=args.cmake_target,
                                 kind=args.cmake_kind, cxx_standard=args.cmake_cxx_standard, cmake_min=args.cmake_min)
            cmake_path = os.path.join(args.out, 'CMakeLists.txt')
            with open(cmake_path, 'w', encoding='utf-8') as f:
                f.write(cmk)
            print(f"Wrote {cmake_path}")
        return

    if not args.input:
        ap.error('Please provide an input Java file or use --dir.')

    if args.pair:
        handle_single_file_pair(tp, args.input, args.out)
        if args.cmake:
            cmk = generate_cmake(args.out, project=args.cmake_project, target=args.cmake_target,
                                 kind=args.cmake_kind, cxx_standard=args.cmake_cxx_standard, cmake_min=args.cmake_min)
            cmake_path = os.path.join(args.out, 'CMakeLists.txt')
            with open(cmake_path, 'w', encoding='utf-8') as f:
                f.write(cmk)
            print(f"Wrote {cmake_path}")
    else:
        handle_single_file_monolith(tp, args.input, args.output)

if __name__ == '__main__':
    main()
