#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
java2cpp17.py
Java → C++17 transpiler with:
  - Imports: java.* → jxx::*
  - String preserved (or std via --string-policy)
  - byte[] handling (multi-dim; ref params/locals; field modes)
  - Generics: class/interface/method templates; type args; wildcards annotated
  - Exceptions: throw, try/catch; 'throws' via macro/comment/off
  - Untyped/multi-catch → catch(Exception&) (base configurable)
  - synchronized(expr) → JXX_SYNCHRONZIED(expr) { ... } (macro configurable)
  - Enums:
      * Simple → enum class + companion meta struct with values()/valueOf()
      * Rich   → class-backed enum with inline constants + static values()/valueOf()
  - Nested types: classes/interfaces/enums preserved (helpers nest with them)
  - Rewrites: EnumName.values()/valueOf(x) → correct static (helper/class)
  - **Header flags**:
      * --include (repeatable)
      * --string-include, --bytearray-include, --exceptions-include (repeatable), --sync-include
      * --no-default-includes to suppress std headers

Usage:
  python java2cpp17.py Input.java -o Output.cpp \
    [--bytearray-fields {value,ref,ref-ctor}] \
    [--checked-exceptions {off,macro,comment}] \
    [--throws-macro JTHROWS] \
    [--string-policy {preserve,std}] \
    [--sync-macro JXX_SYNCHRONZIED] \
    [--exception-base Exception] \
    [--enum-valueof-exception IllegalArgumentException] \
    [--string-include "String.hpp"] \
    [--bytearray-include "ByteArray.hpp"] \
    [--exceptions-include "Exception.hpp" --exceptions-include "IOException.hpp"] \
    [--sync-include "Sync.hpp"] \
    [--include "More.hpp" --include <optional>] \
    [--no-default-includes]
"""

import argparse
from typing import List, Optional, Set, Tuple, Dict
import javalang

# ------------ Primitive mapping (String handled by policy) ------------
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
    def __init__(self):
        self.lines: List[str] = []
        self.indent_level = 0
    def write(self, line: str = ""):
        if line:
            self.lines.append(("    " * self.indent_level) + line)
        else:
            self.lines.append(line)
    def enter(self):
        self.indent_level += 1
    def exit(self):
        self.indent_level = max(self.indent_level - 1, 0)
    def text(self) -> str:
        return "\n".join(self.lines)

# ------------------------------ Helpers ------------------------------
def _base_type_name_and_dims(t) -> Tuple[str, int]:
    if t is None:
        return ("void", 0)
    if isinstance(t, str):
        return (t, 0)
    name = ".".join(t.name) if isinstance(t.name, (list, tuple)) else t.name
    dims = len(t.dimensions or [])
    return (name, dims)

def _cpp_type_for_byte_dims(total_dims: int) -> str:
    t = "ByteArray"
    for _ in range(max(0, total_dims - 1)):
        t = f"std::vector<{t}>"
    return t

# ------------------------------ Transpiler ------------------------------
class JavaToCppTranspiler:
    def __init__(self,
                 field_mode: str = "value",
                 checked_ex_mode: str = "macro",
                 throws_macro: str = "JTHROWS",
                 string_policy: str = "preserve",
                 sync_macro: str = "JXX_SYNCHRONZIED",
                 exception_base: str = "Exception",
                 enum_valueof_exception: str = "IllegalArgumentException",
                 # include flags:
                 string_include: Optional[str] = None,
                 bytearray_include: Optional[str] = None,
                 exceptions_includes: Optional[List[str]] = None,
                 sync_include: Optional[str] = None,
                 extra_includes: Optional[List[str]] = None,
                 no_default_includes: bool = False):
        self.field_mode = field_mode
        self.checked_ex_mode = checked_ex_mode
        self.throws_macro = throws_macro
        self.string_policy = string_policy
        self.sync_macro = sync_macro
        self.exception_base = exception_base
        self.enum_valueof_exception = enum_valueof_exception

        self.string_include = string_include
        self.bytearray_include = bytearray_include
        self.exceptions_includes = exceptions_includes or []
        self.sync_include = sync_include
        self.extra_includes = extra_includes or []
        self.no_default_includes = no_default_includes

        self.using_lines: Set[str] = set()

        # Enum registry: name → info
        self.enums_by_java_qual: Dict[str, dict] = {}
        self.enums_by_unqual: Dict[str, dict] = {}

    # ---------- Import mapping ----------
    @staticmethod
    def _looks_like_package_or_wildcard(segments: List[str], wildcard: bool) -> bool:
        if wildcard:
            return True
        return not segments or (segments[-1] and segments[-1][0].islower())

    def _gather_import_usings(self, tree):
        self.using_lines.clear()
        for imp in (tree.imports or []):
            path: Optional[str] = imp.path
            if not path or not path.startswith("java."):
                continue
            rest = path[len("java."):]
            segments = rest.split(".") if rest else []
            if imp.wildcard:
                ns = "::".join(["jxx"] + segments)
                self.using_lines.add(f"using namespace {ns};")
                continue
            cxx_path = "::".join(["jxx"] + segments) if segments else "jxx"
            if self._looks_like_package_or_wildcard(segments, wildcard=False):
                self.using_lines.add(f"using namespace {cxx_path};")
            else:
                self.using_lines.add(f"using {cxx_path};")

    # ---------- String policy ----------
    def _render_type_arguments(self, args) -> str:
        if not args:
            return ""
        rendered = []
        for a in args:
            an = type(a).__name__
            if an == "TypeArgument":
                if getattr(a, "wildcard", False):
                    if a.pattern_type is not None:
                        bound = self._map_reference_or_basic(a.pattern_type)
                        if getattr(a, "kind", None) == "super":
                            rendered.append(f"{bound} /* ? super */")
                        else:
                            rendered.append(f"{bound} /* ? extends */")
                    else:
                        rendered.append("/* ? */")
                elif a.type is not None:
                    rendered.append(self._map_reference_or_basic(a.type))
                else:
                    rendered.append("/* ? */")
            else:
                rendered.append(str(a))
        return "<" + ", ".join(rendered) + ">"

    def _map_reference_or_basic(self, t) -> str:
        tn = type(t).__name__
        if tn == "BasicType":
            base = t.name
            return JAVA_TO_CPP_TYPES.get(base, base)
        if tn == "ReferenceType":
            raw_name = t.name.replace(".", "::") if isinstance(t.name, str) else str(t.name)
            if raw_name == "String":
                raw_name = "String" if self.string_policy == "preserve" else "std::string"
            args = self._render_type_arguments(getattr(t, "arguments", None))
            return f"{raw_name}{args}"
        if isinstance(t, str):
            if t == "String":
                return "String" if self.string_policy == "preserve" else "std::string"
            return JAVA_TO_CPP_TYPES.get(t, t)
        return str(t)

    def _map_non_byte_array_type_to_cpp(self, t, extra_dims: int = 0) -> str:
        base_cpp = self._map_reference_or_basic(t)
        _, type_dims = _base_type_name_and_dims(t)
        dims = type_dims + (extra_dims or 0)
        for _ in range(dims):
            base_cpp = f"std::vector<{base_cpp}>"
        return base_cpp

    def _string_param_t(self) -> str:
        return "const String&" if self.string_policy == "preserve" else "const std::string&"

    def _string_literal_ctor(self, s: str) -> str:
        # For preserve policy, compare to String("CONST"); for std, "CONST" is fine
        if self.string_policy == "preserve":
            return f"String({s})"
        return s

    # ---------- Enum scan ----------
    def _is_simple_enum(self, enum_node) -> bool:
        body = list(getattr(enum_node, "body", []) or [])
        has_members = any(type(x).__name__ in {
            "FieldDeclaration","MethodDeclaration","ConstructorDeclaration",
            "ClassDeclaration","InterfaceDeclaration","EnumDeclaration"
        } for x in body)
        if has_members:
            return False
        for c in (getattr(enum_node, "constants", []) or []):
            if getattr(c, "arguments", None):
                return False
            if getattr(c, "class_body", None):
                return False
        return True

    def _scan_types_for_enums(self, types, java_scope: List[str], cxx_scope: List[str]):
        for t in types or []:
            tname = type(t).__name__
            if tname == "EnumDeclaration":
                name = t.name
                java_qual = ".".join(java_scope + [name]) if java_scope else name
                cxx_qual = "::".join(cxx_scope + [name]) if cxx_scope else name
                constants = [c.name for c in (t.constants or [])]
                kind = "simple" if self._is_simple_enum(t) else "rich"
                info = {"kind": kind, "java_qual": java_qual, "cxx_qual": cxx_qual, "constants": constants}
                self.enums_by_java_qual[java_qual] = info
                self.enums_by_unqual.setdefault(name, info)
                # Recurse nested types
                self._scan_types_for_enums(getattr(t, "body", None), java_scope + [name], cxx_scope + [name])
            elif tname in ("ClassDeclaration", "InterfaceDeclaration"):
                name = t.name
                self._scan_types_for_enums(getattr(t, "body", None), java_scope + [name], cxx_scope + [name])

    def _lookup_enum_info(self, java_qual_or_unqual: str) -> Optional[dict]:
        if java_qual_or_unqual in self.enums_by_java_qual:
            return self.enums_by_java_qual[java_qual_or_unqual]
        last = java_qual_or_unqual.split(".")[-1]
        return self.enums_by_unqual.get(last)

    # ---------- Expressions (includes enum call rewrites) ----------
    def emit_expression(self, e) -> str:
        if e is None:
            return ""
        typ = type(e).__name__

        if typ == "Literal":
            v = e.value
            if v is None:
                return "nullptr"
            return v

        if typ == "BinaryOperation":
            left = self.emit_expression(e.operandl)
            right = self.emit_expression(e.operandr)
            op = e.operator
            return f"({left} {op} {right})"

        if typ == "UnaryOperation":
            op = e.operator
            exp = self.emit_expression(e.expression)
            if getattr(e, "postfix", False):
                return f"({exp}{op})"
            else:
                return f"({op}{exp})"

        if typ == "TernaryExpression":
            c = self.emit_expression(e.condition)
            t = self.emit_expression(e.if_true)
            f = self.emit_expression(e.if_false)
            return f"({c} ? {t} : {f})"

        if typ == "MemberReference":
            qual = e.qualifier
            if isinstance(qual, str):
                return f"{qual}.{e.member}" if qual else e.member
            elif qual is not None:
                return f"{self.emit_expression(qual)}.{e.member}"
            return e.member

        if typ == "ReferenceType":
            return self._map_non_byte_array_type_to_cpp(e)

        if typ == "This":
            return "this"

        if typ == "ClassReference":
            return f"typeid({e.type.name})"

        if typ == "MethodInvocation":
            qual = e.qualifier
            member = e.member
            args = [self.emit_expression(a) for a in (e.arguments or [])]
            qual_str = qual if isinstance(qual, str) else (self.emit_expression(qual) if qual is not None else "")

            # Enum .values() / .valueOf() rewrite
            if qual_str and member in {"values", "valueOf"}:
                info = self._lookup_enum_info(qual_str)
                if info:
                    cxx_qual = qual_str.replace(".", "::")
                    target = f"{cxx_qual}__meta::{member}" if info["kind"] == "simple" else f"{cxx_qual}::{member}"
                    return f"{target}({', '.join(args)})"

            # System.out mapping
            if ((qual_str == "System.out" or qual_str.endswith("System.out"))
                and member in {"print", "println"}):
                if not args:
                    return "std::cout << std::endl"
                joiner = " << "
                line_end = " << std::endl" if member == "println" else ""
                return f"(std::cout {joiner} {' << '.join(args)}{line_end})"

            call = f"{qual_str}.{member}" if qual_str else member
            return f"{call}({', '.join(args)})" if args else f"{call}()"

        if typ == "ClassCreator":
            typename = self._map_reference_or_basic(e.type)
            args = [self.emit_expression(a) for a in (e.arguments or [])]
            return f"{typename}({', '.join(args)})"

        if typ == "ArrayCreator":
            base_t = self._map_reference_or_basic(e.type)
            dims = len(e.dimensions or [])
            if dims == 1 and e.dimensions[0] is not None:
                size_expr = self.emit_expression(e.dimensions[0])
                return f"std::vector<{base_t}>({size_expr})"
            return f"/* TODO: array creation for {base_t} with {dims} dims */"

        if typ == "ThrowStatement":
            ex = self.emit_expression(e.expression)
            return f"throw {ex}"

        if typ == "ArraySelector":
            base = self.emit_expression(e.primary)
            index = self.emit_expression(e.index)
            return f"{base}[{index}]"

        if typ == "Assignment":
            l = self.emit_expression(e.expressionl)
            r = self.emit_expression(e.value)
            op = e.type if e.type else "="
            return f"({l} {op} {r})"

        if typ == "Cast":
            t = self._map_non_byte_array_type_to_cpp(e.type)
            exp = self.emit_expression(e.expression)
            return f"(({t}){exp})"

        if typ == "VariableDeclarator":
            name = e.name
            init = ""
            if e.initializer is not None:
                init = f" = {self.emit_expression(e.initializer)}"
            return f"{name}{init}"

        return str(e)

    # ---------- Statements ----------
    def emit_statement(self, s, out: Emit):
        if s is None:
            return
        typ = type(s).__name__

        if typ == "ReturnStatement":
            expr = self.emit_expression(s.expression)
            out.write(f"return {expr};")
            return

        if typ == "StatementExpression":
            expr = self.emit_expression(s.expression)
            out.write(f"{expr};")
            return

        if typ == "LocalVariableDeclaration":
            base, type_dims = _base_type_name_and_dims(s.type)
            for d in (s.declarators or []):
                extra_dims = len(getattr(d, "dimensions", []) or [])
                total_dims = type_dims + extra_dims
                name = d.name
                init_expr = self.emit_expression(d.initializer) if d.initializer is not None else None

                if base == "byte" and total_dims >= 1:
                    cxx = _cpp_type_for_byte_dims(total_dims)
                    if init_expr is not None:
                        out.write(f"{cxx}& {name} = {init_expr};")
                    else:
                        out.write(f"{cxx} {name};  // NOTE: cannot emit reference without initializer")
                else:
                    t = self._map_non_byte_array_type_to_cpp(s.type, extra_dims=extra_dims)
                    decl = name + (f" = {init_expr}" if init_expr is not None else "")
                    out.write(f"{t} {decl};")
            return

        if typ == "BlockStatement":
            if s.statements:
                for st in s.statements:
                    self.emit_statement(st, out)
            return

        if typ == "IfStatement":
            cond = self.emit_expression(s.condition)
            out.write(f"if ({cond}) {{")
            out.enter()
            self.emit_statement(s.then_statement, out)
            out.exit()
            out.write("}")
            if s.else_statement is not None:
                out.write("else {")
                out.enter()
                self.emit_statement(s.else_statement, out)
                out.exit()
                out.write("}")
            return

        if typ == "WhileStatement":
            cond = self.emit_expression(s.condition)
            out.write(f"while ({cond}) {{")
            out.enter()
            self.emit_statement(s.body, out)
            out.exit()
            out.write("}")
            return

        if typ == "ForStatement":
            init = ""
            if s.init:
                init_parts = []
                for i in s.init:
                    if type(i).__name__ == "LocalVariableDeclaration":
                        tmp = Emit()
                        self.emit_statement(i, tmp)
                        init_parts.extend(line.strip().rstrip(";") for line in tmp.lines if line.strip())
                    else:
                        init_parts.append(self.emit_expression(i))
                init = ", ".join(init_parts)
            cond = self.emit_expression(s.condition) if s.condition else ""
            update = ", ".join(self.emit_expression(u) for u in (s.update or []))
            out.write(f"for ({init}; {cond}; {update}) {{")
            out.enter()
            self.emit_statement(s.body, out)
            out.exit()
            out.write("}")
            return

        if typ == "TryStatement":
            out.write("try {")
            out.enter()
            self.emit_statement(s.block, out)
            out.exit()
            out.write("}")
            for catch in (s.catches or []):
                p = catch.parameter
                types_list = list(getattr(p, "types", []) or [])
                name = p.name or "e"
                if not types_list or len(types_list) > 1:
                    comment = ""
                    if types_list:
                        try:
                            listed = " | ".join(self._map_non_byte_array_type_to_cpp(t) for t in types_list)
                        except Exception:
                            listed = " | ".join(str(t) for t in types_list)
                        comment = f"  // original: {listed}"
                    out.write(f"catch ({self.exception_base}& {name}) {{")
                    out.enter()
                    if comment:
                        out.write(comment)
                    if catch.block:
                        for st in catch.block:
                            self.emit_statement(st, out)
                    out.exit()
                    out.write("}")
                else:
                    ct = self._map_non_byte_array_type_to_cpp(types_list[0])
                    out.write(f"catch ({ct}& {name}) {{")
                    out.enter()
                    self.emit_statement(catch.block, out)
                    out.exit()
                    out.write("}")
            if s.finally_block is not None:
                out.write("// finally { ... }")
                out.write("{")
                out.enter()
                self.emit_statement(s.finally_block, out)
                out.exit()
                out.write("}")
            return

        if typ == "SynchronizedStatement":
            lock_node = getattr(s, "lock", None) or getattr(s, "expression", None)
            lock_expr = self.emit_expression(lock_node) if lock_node is not None else ""
            out.write(f"{self.sync_macro}({lock_expr}) {{")
            out.enter()
            self.emit_statement(s.block, out)
            out.exit()
            out.write("}")
            return

        if typ == "SwitchStatement":
            expr = self.emit_expression(s.expression)
            out.write(f"switch ({expr}) {{")
            out.enter()
            for c in (s.cases or []):
                if c.case is None:
                    out.write("default:")
                else:
                    out.write(f"case {self.emit_expression(c.case)}:")
                out.enter()
                for st in c.statements or []:
                    self.emit_statement(st, out)
                out.write("break;")
                out.exit()
            out.exit()
            out.write("}")
            return

        out.write(f"/* TODO: Unhandled statement type: {typ} */")

    # ---------- Fields ----------
    def _field_decl_is_byte_array(self, f_type, declarator) -> Tuple[bool, int]:
        base, type_dims = _base_type_name_and_dims(f_type)
        extra_dims = len(getattr(declarator, "dimensions", []) or [])
        total_dims = type_dims + extra_dims
        return (base == "byte" and total_dims >= 1, total_dims)

    def emit_field(self, f, out: Emit, ref_fields_accum: List[Tuple[str, str]]):
        for d in (f.declarators or []):
            is_byte, total_dims = self._field_decl_is_byte_array(f.type, d)
            name = d.name
            if is_byte:
                cxx = _cpp_type_for_byte_dims(total_dims)
                if self.field_mode == "value":
                    out.write(f"{cxx} {name};")
                elif self.field_mode in ("ref", "ref-ctor"):
                    out.write(f"{cxx}& {name};")
                    if self.field_mode == "ref-ctor":
                        ref_fields_accum.append((name, f"{cxx}&"))
                else:
                    out.write(f"{cxx} {name};")
            else:
                t = self._map_non_byte_array_type_to_cpp(f.type, extra_dims=len(getattr(d, 'dimensions', []) or []))
                init_expr = ""
                if d.initializer is not None:
                    init_expr = f" = {self.emit_expression(d.initializer)}"
                out.write(f"{t} {name}{init_expr};")

    # ---------- Templates / throws ----------
    def _render_type_parameters(self, type_params) -> str:
        if not type_params:
            return ""
        parts = []
        for tp in type_params:
            b = ""
            if getattr(tp, "bounds", None):
                b = " /* extends " + ", ".join(self._map_reference_or_basic(bound) for bound in tp.bounds) + " */"
            parts.append(f"typename {tp.name}{b}")
        return "template<" + ", ".join(parts) + ">"

    def _render_throws_suffix(self, throws_types) -> Tuple[str, str]:
        if not throws_types:
            return ("", "")
        try:
            thr = ", ".join(self._map_reference_or_basic(t) for t in throws_types)
        except Exception:
            thr = ", ".join(str(t) for t in throws_types)
        if self.checked_ex_mode == "macro":
            return (f" {self.throws_macro}({thr})", "")
        if self.checked_ex_mode == "comment":
            return ("", f"  // throws: {thr}")
        return ("", "")

    # ---------- Methods / Ctors ----------
    def emit_method(self, m, out: Emit, class_name: str, in_interface: bool = False):
        is_ctor = (type(m).__name__ == "ConstructorDeclaration")
        method_tpl = self._render_type_parameters(getattr(m, "type_parameters", None))

        # Return type
        if not is_ctor:
            if m.return_type is not None:
                base, type_dims = _base_type_name_and_dims(m.return_type)
                method_dims = len(getattr(m, "dimensions", []) or [])
                total_dims = type_dims + method_dims
                if base == "byte" and total_dims >= 1:
                    ret = _cpp_type_for_byte_dims(total_dims)
                else:
                    ret = self._map_non_byte_array_type_to_cpp(m.return_type, extra_dims=method_dims)
            else:
                ret = "void"
        else:
            ret = ""

        name = m.name if not is_ctor else class_name

        # Params
        params_out: List[str] = []
        for p in (m.parameters or []):
            base, type_dims = _base_type_name_and_dims(p.type)
            total_dims = type_dims
            if base == "byte" and total_dims >= 1:
                cxx = _cpp_type_for_byte_dims(total_dims)
                params_out.append(f"{cxx}& {p.name}")
            else:
                pt = self._map_non_byte_array_type_to_cpp(p.type)
                params_out.append(f"{pt} {p.name}")

        # Throws
        sig_suffix, throws_comment = self._render_throws_suffix(getattr(m, "throws", None))

        if method_tpl:
            out.write(method_tpl)

        header = (f"{ret} {name}({', '.join(params_out)})" if not is_ctor
                  else f"{name}({', '.join(params_out)})")

        if in_interface:
            if getattr(m, "body", None):
                out.write(f"virtual {header}{sig_suffix} {{")
                out.enter()
                for st in m.body:
                    self.emit_statement(st, out)
                out.exit()
                out.write("}")
            else:
                line = f"virtual {header}{sig_suffix} = 0;"
                if throws_comment:
                    line += throws_comment
                out.write(line)
        else:
            line = header + sig_suffix + " {"
            if throws_comment:
                out.write(line)
                out.enter()
                out.write(throws_comment.strip())
            else:
                out.write(line)
                out.enter()
            if getattr(m, "body", None):
                for st in m.body:
                    self.emit_statement(st, out)
            out.exit()
            out.write("}")

    # ---------- Enums ----------
    def _emit_simple_enum_with_helpers(self, t, out: Emit):
        enum_name = t.name
        consts = [c.name for c in (t.constants or [])]
        # 1) enum class
        out.write(f"enum class {enum_name} {{ {', '.join(consts)} }};")
        # 2) companion meta with values() / valueOf()
        out.write(f"struct {enum_name}__meta {{")
        out.enter()
        # values()
        out.write(f"static inline constexpr std::array<{enum_name}, {len(consts)}> values() {{")
        out.enter()
        items = ", ".join(f"{enum_name}::{c}" for c in consts)
        out.write(f"return {{ {items} }};")
        out.exit()
        out.write("}")
        # valueOf(String)
        str_param = self._string_param_t()
        out.write(f"static inline {enum_name} valueOf({str_param} s) {{")
        out.enter()
        for c in consts:
            lit = self._string_literal_ctor(f'\"{c}\"')
            out.write(f"if (s == {lit}) return {enum_name}::{c};")
        out.write(f"throw {self.enum_valueof_exception}(\"No enum const {enum_name}.\" /* + s */);")
        out.exit()
        out.write("}")
        out.exit()
        out.write("};")

    def _emit_rich_enum_class(self, t, out: Emit):
        enum_name = t.name
        bases = []
        if getattr(t, "implements", None):
            for iface in t.implements:
                bases.append(self._map_non_byte_array_type_to_cpp(iface))
        bases_clause = " : " + ", ".join(f"public {b}" for b in bases) if bases else ""
        out.write(f"class {enum_name}{bases_clause} {{")
        out.enter()

        # public: constants
        out.write("public:")
        out.enter()
        for c in (t.constants or []):
            args = []
            if getattr(c, "arguments", None):
                args = [self.emit_expression(a) for a in c.arguments]
            args_str = ", ".join(args)
            init = f"{enum_name}({args_str})" if args else f"{enum_name}()"
            out.write(f"inline static const {enum_name} {c.name} = {init};")
            if getattr(c, "class_body", None):
                out.write(f"// NOTE: per-constant class body for {c.name} not translated; consider manual specialization")
        # values/valueOf
        consts = [c.name for c in (t.constants or [])]
        out.write(f"static inline std::array<{enum_name}, {len(consts)}> values() {{")
        out.enter()
        items = ", ".join(f"{enum_name}::{c}" for c in consts)
        out.write(f"return {{ {items} }};")
        out.exit()
        out.write("}")
        str_param = self._string_param_t()
        out.write(f"static inline {enum_name} valueOf({str_param} s) {{")
        out.enter()
        for c in consts:
            lit = self._string_literal_ctor(f'\"{c}\"')
            out.write(f"if (s == {lit}) return {enum_name}::{c};")
        out.write(f"throw {self.enum_valueof_exception}(\"No enum const {enum_name}.\" /* + s */);")
        out.exit()
        out.write("}")
        out.exit()

        # other members with access control
        def access_of(mods: set) -> str:
            if "public" in mods:
                return "public"
            if "protected" in mods:
                return "protected"
            return "private"

        current_access = None
        ref_fields_accum: List[Tuple[str, str]] = []
        for m in (t.body or []):
            mtype = type(m).__name__
            if mtype == "ConstructorDeclaration":
                desired_access = "private"
            else:
                mods = set(getattr(m, "modifiers", []) or [])
                desired_access = access_of(mods)
            if desired_access != current_access:
                if current_access is not None:
                    out.exit()
                out.write(f"{desired_access}:")
                out.enter()
                current_access = desired_access
            if mtype == "FieldDeclaration":
                self.emit_field(m, out, ref_fields_accum)
            elif mtype in ("MethodDeclaration", "ConstructorDeclaration"):
                self.emit_method(m, out, enum_name, in_interface=False)
            elif mtype in ("ClassDeclaration", "InterfaceDeclaration", "EnumDeclaration"):
                self.emit_type(m, out)
            else:
                out.write(f"/* TODO: Unhandled enum member type: {mtype} */")
        if current_access is not None:
            out.exit()
        out.exit()
        out.write("}")
        out.write(";")

    def emit_enum(self, t, out: Emit):
        info = self._lookup_enum_info(t.name)
        if info and info["kind"] == "simple":
            self._emit_simple_enum_with_helpers(t, out)
        else:
            self._emit_rich_enum_class(t, out)

    # ---------- Types (Class / Interface / Enum) ----------
    def emit_type(self, t, out: Emit):
        tname = type(t).__name__
        if tname == "EnumDeclaration":
            self.emit_enum(t, out)
            return
        if tname not in ("ClassDeclaration", "InterfaceDeclaration"):
            out.write(f"/* TODO: Unhandled type: {tname} */")
            return

        class_tpl = self._render_type_parameters(getattr(t, "type_parameters", None))
        if class_tpl:
            out.write(class_tpl)

        is_interface = (tname == "InterfaceDeclaration")
        class_or_struct = "struct" if is_interface else "class"
        class_name = t.name

        base_list = []
        if getattr(t, "extends", None):
            extends = t.extends if isinstance(t.extends, list) else [t.extends]
            for b in extends:
                base_list.append(self._map_non_byte_array_type_to_cpp(b))
        if getattr(t, "implements", None):
            for iface in t.implements:
                base_list.append(self._map_non_byte_array_type_to_cpp(iface))
        bases = " : " + ", ".join(f"public {b}" for b in base_list) if base_list else ""

        out.write(f"{class_or_struct} {class_name}{bases} {{")
        out.enter()

        def access_of(mods: set) -> str:
            if "public" in mods:
                return "public"
            if "protected" in mods:
                return "protected"
            return "private"

        current_access = None
        ref_fields_accum: List[Tuple[str, str]] = []

        for m in t.body or []:
            mtype = type(m).__name__
            mods = set(getattr(m, "modifiers", []) or [])
            desired_access = access_of(mods)
            if desired_access != current_access:
                if current_access is not None:
                    out.exit()
                out.write(f"{desired_access}:")
                out.enter()
                current_access = desired_access
            if mtype == "FieldDeclaration":
                self.emit_field(m, out, ref_fields_accum)
            elif mtype in ("MethodDeclaration", "ConstructorDeclaration"):
                self.emit_method(m, out, class_name, in_interface=is_interface)
            elif mtype in ("ClassDeclaration", "InterfaceDeclaration", "EnumDeclaration"):
                self.emit_type(m, out)
            else:
                out.write(f"/* TODO: Unhandled member type: {mtype} */")

        if current_access is not None:
            out.exit()

        if not is_interface and self.field_mode == "ref-ctor" and ref_fields_accum:
            out.write("public:")
            out.enter()
            params_sig = ", ".join(f"{typ} {name}" for (name, typ) in ref_fields_accum)
            inits = ", ".join(f"{name}({name})" for (name, _) in ref_fields_accum)
            out.write(f"{class_name}({params_sig}) : {inits} {{}}")
            out.exit()

        out.exit()
        out.write("}")
        out.write(";")

    # ---------- Includes ----------
    @staticmethod
    def _normalize_include_operand(s: str) -> str:
        s = s.strip()
        if not s:
            return s
        if s.startswith("<") or s.startswith("\""):
            return s
        # default to quoted header
        return f"\"{s}\""

    def _emit_includes(self, out: Emit):
        ordered: List[str] = []

        # Feature-specific includes first (if provided)
        if self.string_include:
            ordered.append(self.string_include)
        if self.bytearray_include:
            ordered.append(self.bytearray_include)
        for exc in self.exceptions_includes:
            ordered.append(exc)
        if self.sync_include:
            ordered.append(self.sync_include)
        # Generic extras
        for inc in self.extra_includes:
            ordered.append(inc)

        # Default library includes (unless suppressed)
        if not self.no_default_includes:
            # Always needed by our emitter
            ordered.append("<iostream>")
            ordered.append("<vector>")
            ordered.append("<array>")
            if self.string_policy == "std":
                ordered.append("<string>")

        # Deduplicate while preserving order
        seen = set()
        normalized: List[str] = []
        for inc in ordered:
            op = self._normalize_include_operand(inc)
            if op and op not in seen:
                seen.add(op)
                normalized.append(op)

        for op in normalized:
            out.write(f"#include {op}")

    # ---------- Translation entry ----------
    def transpile(self, java_source: str) -> str:
        tree = javalang.parse.parse(java_source)

        # Pre-scan enums (including nested) so we can rewrite .values/.valueOf
        self._scan_types_for_enums(tree.types, [], [])

        # Imports & usings
        self._gather_import_usings(tree)

        out = Emit()
        out.write("// Generated by java2cpp17 (AST-based)")

        # Includes
        self._emit_includes(out)
        out.write("// NOTE: Provide your ByteArray, String, Exception hierarchy, and synchronization macro if not in PCH.")
        out.write("")

        for line in sorted(self.using_lines):
            out.write(line)
        if self.using_lines:
            out.write("")

        # Package → namespace
        if tree.package:
            ns = tree.package.name.replace(".", "::")
            out.write(f"namespace {ns} {{")
            out.write("")
            out.enter()

        # Types
        for t in (tree.types or []):
            self.emit_type(t, out)
            out.write("")

        if tree.package:
            out.exit()
            out.write(f"}} // namespace {tree.package.name.replace('.', '::')}")

        return out.text()

def main():
    ap = argparse.ArgumentParser(
        description="Java → C++17 AST transpiler (includes, enums with values/valueOf, generics, exceptions, nested types, String preserve, byte arrays, synchronized→macro)"
    )
    ap.add_argument("input", help="Input Java source file")
    ap.add_argument("-o", "--output", help="Output C++ file (default: stdout)")

    # Existing behavior controls
    ap.add_argument("--bytearray-fields",
                    choices=["value", "ref", "ref-ctor"],
                    default="value",
                    help="Emit ByteArray-like fields as value (default), ref, or ref-ctor (adds synthetic ctor).")
    ap.add_argument("--checked-exceptions",
                    choices=["off", "macro", "comment"],
                    default="macro",
                    help="Model Java 'throws' on methods: macro (default), comment, or off.")
    ap.add_argument("--throws-macro", default="JTHROWS",
                    help="Macro name for --checked-exceptions=macro.")
    ap.add_argument("--string-policy",
                    choices=["preserve", "std"],
                    default="preserve",
                    help="How to render Java String: preserve (C++ String) or std (std::string).")
    ap.add_argument("--sync-macro", default="JXX_SYNCHRONZIED",
                    help="Macro name for synchronized blocks.")
    ap.add_argument("--exception-base", default="Exception",
                    help="Base exception type for untyped/multi-catch.")
    ap.add_argument("--enum-valueof-exception", default="IllegalArgumentException",
                    help="Exception type thrown by valueOf when no match.")

    # NEW: include flags
    ap.add_argument("--string-include", default=None,
                    help="Header to include for your String type, e.g., \"String.hpp\"")
    ap.add_argument("--bytearray-include", default=None,
                    help="Header to include for your ByteArray type, e.g., \"ByteArray.hpp\"")
    ap.add_argument("--exceptions-include", action="append", default=[],
                    help="Header(s) for your Exception hierarchy/macros; can repeat.")
    ap.add_argument("--sync-include", default=None,
                    help="Header that defines your synchronization macro, e.g., \"Sync.hpp\"")
    ap.add_argument("--include", action="append", default=[],
                    help="Additional #include operand(s); can repeat. Use <...> or \"...\"; "
                         "if omitted, we wrap in quotes.")
    ap.add_argument("--no-default-includes", action="store_true",
                    help="Do not emit standard library includes (<iostream>, <vector>, <array>, and <string> if needed).")

    args = ap.parse_args()

    with open(args.input, "r", encoding="utf-8") as f:
        src = f.read()

    transpiler = JavaToCppTranspiler(
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
        no_default_includes=args.no_default_includes
    )

    cpp = transpiler.transpile(src)
    if args.output:
        with open(args.output, "w", encoding="utf-8") as f:
            f.write(cpp)
    else:
        print(cpp)

if __name__ == "__main__":
    main()
    