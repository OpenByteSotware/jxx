#!/usr/bin/env python3
## -*- coding: utf-8 -*-
"""
Assumptions / goals:
  • C++ exceptions are thrown by VALUE (catch by const ref).
  • All Java classes implicitly extend Object; in C++ we emit explicit inheritance.
  • Interfaces map to pure-virtual C++ types that DO NOT derive from Object.
  • Objects use pointer semantics: non-array references => jxx::Ptr<RawType>
  • Arrays use jxx::lang::JxxArray<T, Rank> wrapped in jxx::Ptr<...>

New in this version:
  • Array initializers use JxxArray constructors:
      - Rank-1: JxxArray(std::initializer_list<T>)
      - Rank>=2: JxxArray(std::initializer_list<jxx::Ptr<SubArray>>) to support null rows
  • Array field initialization moved out of headers:
      - instance array fields initialized in constructors in .cpp
      - static array fields initialized only in .cpp
  • Synchronized blocks that contain `return` in non-void methods are emitted as:
      return lock->synchronized([&-> Ret { ... });
  • Safe override heuristic: add `override` if:
      - proven in-file by base/interface methods, OR
      - Java method has @Override annotation

Requires: pip install javalang
"""

from __future__ import annotations

import argparse
import os
import sys
from typing import List, Optional, Set, Tuple, Dict, Any

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


def _java_visibility(mods: Set[str]) -> str:
    if "private" in mods:
        return "private"
    if "protected" in mods:
        return "protected"
    if "public" in mods:
        return "public"
    return "public"


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
        instanceof_style: str = "macro",
        downcast_style: str = "macro",
        instanceof_macro: str = "JXX_INSTANCEOF",
        downcast_macro: str = "JXX_CAST",
        instanceof_func: str = "::jxx::lang::instanceof_as",
        downcast_func: str = "::jxx::lang::cast_as",
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

        self.instanceof_style = instanceof_style
        self.downcast_style = downcast_style
        self.instanceof_macro = instanceof_macro
        self.downcast_macro = downcast_macro
        self.instanceof_func = instanceof_func
        self.downcast_func = downcast_func

        self._tmp_counter = 0

        # conditional include flags
        self._needs_functional = False
        self._needs_cassert = False
        self._needs_object_include = False
        self._needs_initializer_list = False

        self._deferred_instance_inits: Dict[str, List[Tuple[str, str]]] = {}
        self._deferred_ctor_initlist: Dict[str, List[Tuple[str, str]]] = {}

        # object include path in your tree
        self.object_include = "lang/jxx.lang.Object.h"

        # catch var / reference var tracking for '.' member access
        self._dotstack: List[Set[str]] = []

        # current method return type tracking (for synchronized-return parity)
        self._retstack: List[str] = []

        # override detection (in-file + @Override)
        self._type_table: Dict[str, Dict[str, Any]] = {}
        self._override_keys: Dict[str, Set[Tuple[str, Tuple[str, ...]]]] = {}

        # deferred instance array field initializations: class -> [(field, expr)]
        self._deferred_instance_inits: Dict[str, List[Tuple[str, str]]] = {}

        # primitive map
        self.primitive_map: Dict[str, str] = dict(JAVA_TO_CPP_TYPES)
        for item in (primitive_map or []):
            if item and "=" in item:
                k, v = item.split("=", 1)
                k = k.strip()
                v = v.strip()
                if k and v:
                    self.primitive_map[k] = v

        self.using_lines: Set[str] = set()
        self.import_header_includes: List[str] = []
        self.include_written: Set[str] = set()

        self._static_field_defs: List[Tuple[str, str, str, str]] = []

        # per-method symbol stack for instance/static heuristics
        self._symstack: List[Dict[str, str]] = []

    # ---------------- includes ----------------
    def _unwrap_node(self, n):
        # javalang occasionally wraps nodes in tuples (e.g., (pos, node))
        if isinstance(n, tuple) and len(n) >= 2:
            cand = n[1]
            # best-effort: accept the second element as the node
            return cand
        return n

    @staticmethod
    def _norm_inc(s: str) -> str:
        s = (s or "").strip()
        if not s:
            return s
        if s.startswith("<") or s.startswith('"'):
            return s
        return f'"{s}"'

    def _emit_include_operand(self, out: Emit, operand: str) -> None:
        op = self._norm_inc(operand)
        if op and op not in self.include_written:
            self.include_written.add(op)
            out.write(f"#include {op}")

    def _scan_needs(self, tree) -> None:
        self._needs_functional = False
        self._needs_cassert = False
        self._needs_object_include = False
        self._needs_initializer_list = False

        for _, node in tree:
            nn = type(node).__name__

            if nn == "TryStatement" and getattr(node, "finally_block", None) is not None:
                self._needs_functional = True

            if nn == "AssertStatement":
                self._needs_cassert = True

            if nn == "ArrayInitializer":
                self._needs_initializer_list = True

            if nn == "ArrayCreator" and getattr(node, "initializer", None) is not None:
                self._needs_initializer_list = True

            if nn == "ClassDeclaration":
                ext = getattr(node, "extends", None)
                if ext is None:
                    self._needs_object_include = True
                else:
                    ext_name = ".".join(ext.name) if isinstance(getattr(ext, "name", None), (list, tuple)) else getattr(ext, "name", "")
                    if ext_name.split(".")[-1] == "Object":
                        self._needs_object_include = True

            if self._needs_functional and self._needs_cassert and self._needs_object_include and self._needs_initializer_list:
                return

    def emit_includes(self, out: Emit) -> None:
        ordered: List[str] = []

        if self._needs_object_include:
            ordered.append(self.object_include)

        if self.string_include:
            ordered.append(self.string_include)
        if self.bytearray_include:
            ordered.append(self.bytearray_include)
        ordered.extend(self.extra_includes)

        need_memory = (self.instanceof_style == "dynamic" or self.downcast_style == "dynamic")

        if not self.no_default_includes:
            ordered.extend(["<vector>", "<array>"])
            if need_memory:
                ordered.append("<memory>")
            if self._needs_functional:
                ordered.append("<functional>")
            if self._needs_initializer_list:
                ordered.append("<initializer_list>")
            if self.string_policy == "std":
                ordered.append("<string>")
            if self._needs_cassert:
                ordered.append("<cassert>")

        seen = set()
        for inc in ordered:
            op = self._norm_inc(inc)
            if op and op not in seen:
                seen.add(op)
                self._emit_include_operand(out, op)

    # ---------------- imports -> usings + include hints ----------------
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
            if not path or not path.startswith("java."):
                continue
            rest = path[len("java.") :]
            segs = rest.split(".") if rest else []

            if imp.wildcard:
                ns = "::".join(["jxx"] + segs)
                self.using_lines.add(f"using namespace {ns};")
                self.import_header_includes.append(f"jxx.{'.'.join(segs)}.h" if segs else "jxx.h")
            else:
                cxx = "::".join(["jxx"] + segs) if segs else "jxx"
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

    # ---------------- symbol / dot / return stacks ----------------
    def _ret_push(self, ret: str) -> None:
        self._retstack.append(ret)

    def _ret_pop(self) -> None:
        if self._retstack:
            self._retstack.pop()

    def _ret_current(self) -> str:
        return self._retstack[-1] if self._retstack else "void"

    def _sym_push(self) -> None:
        self._symstack.append({})
        self._dotstack.append(set())

    def _sym_pop(self) -> None:
        if self._symstack:
            self._symstack.pop()
        if self._dotstack:
            self._dotstack.pop()

    def _sym_set(self, name: str, ctype: str) -> None:
        if self._symstack:
            self._symstack[-1][name] = ctype

    def _sym_get(self, name: str) -> Optional[str]:
        for scope in reversed(self._symstack):
            if name in scope:
                return scope[name]
        return None

    def _dot_add(self, name: str) -> None:
        if self._dotstack:
            self._dotstack[-1].add(name)

    def _dot_has(self, name: str) -> bool:
        for scope in reversed(self._dotstack):
            if name in scope:
                return True
        return False

    # ---------------- utility helpers ----------------
    def _fresh_tmp(self, prefix: str = "__tmp") -> str:
        self._tmp_counter += 1
        return f"{prefix}{self._tmp_counter}"

    def _contains_return(self, node) -> bool:
        if node is None:
            return False
        tn = type(node).__name__
        if tn == "ReturnStatement":
            return True
        for attr in ("statements", "block", "then_statement", "else_statement", "body"):
            v = getattr(node, attr, None)
            if isinstance(v, list):
                for x in v:
                    if self._contains_return(x):
                        return True
            elif v is not None:
                if self._contains_return(v):
                    return True
        return False

    # ---------------- type mapping ----------------
    @staticmethod
    def _simple_name(name: str) -> str:
        return name.split(".")[-1]

    def _is_java_class_ref(self, t) -> bool:
        return type(t).__name__ == "ReferenceType" and self._simple_name(str(t.name)) == "Class"

    def _raw_reference_name(self, t) -> str:
        raw = t.name.replace(".", "::") if isinstance(t.name, str) else str(t.name)
        if raw == "String":
            return "String" if self.string_policy == "preserve" else "std::string"
        return raw

    def _map_ref_or_basic(self, t) -> str:
        tn = type(t).__name__
        if tn == "BasicType":
            return self.primitive_map.get(t.name, t.name)
        if tn == "ReferenceType":
            if self._is_java_class_ref(t):
                return "jxx::Ptr<jxx::lang::ClassAny>"
            raw = self._raw_reference_name(t)
            return f"jxx::Ptr<{raw}>"
        if isinstance(t, str):
            if t == "String":
                raw = "String" if self.string_policy == "preserve" else "std::string"
                return f"jxx::Ptr<{raw}>"
            return self.primitive_map.get(t, t)
        return str(t)

    def _array_type(self, elem_cpp: str, rank: int) -> str:
        return f"jxx::Ptr<jxx::lang::JxxArray<{elem_cpp}, {rank}>>"

    def _array_elem_type(self, t) -> str:
        tn = type(t).__name__
        if tn == "BasicType":
            return self.primitive_map.get(t.name, t.name)
        if tn == "ReferenceType":
            if self._is_java_class_ref(t):
                return "jxx::lang::ClassAny"
            raw = self._raw_reference_name(t)
            return f"jxx::Ptr<{raw}>"
        return f"jxx::Ptr<{self._simple_name(str(getattr(t, 'name', t)))}>"

    def _map_type(self, t, extra_dims: int = 0) -> str:
        base_name, d0 = _base_type_name_and_dims(t)
        rank = d0 + (extra_dims or 0)

        if self._simple_name(base_name) == "byte" and rank == 1:
            return "jxx::Ptr<ByteArray>"

        if type(t).__name__ == "ReferenceType" and self._simple_name(base_name) == "Class":
            if rank == 0:
                return "jxx::Ptr<jxx::lang::ClassAny>"
            return f"jxx::Ptr<jxx::lang::JxxArray<jxx::lang::ClassAny, {rank}>>"

        if rank > 0:
            elem = self._array_elem_type(t)
            return self._array_type(elem, rank)

        return self._map_ref_or_basic(t)

    # ---------------- instanceof / downcast ----------------
    def _raw_type_for_cast(self, t) -> str:
        tn = type(t).__name__
        if tn == "BasicType":
            return self.primitive_map.get(t.name, t.name)
        if tn == "ReferenceType":
            base = self._simple_name(str(t.name))
            if base == "Class":
                return "jxx::lang::ClassAny"
            return self._raw_reference_name(t)
        return str(t)

    def _emit_instanceof(self, expr_cpp: str, type_raw: str) -> str:
        if self.instanceof_style == "macro":
            return f"{self.instanceof_macro}({type_raw}, {expr_cpp})"
        if self.instanceof_style == "func":
            return f"{self.instanceof_func}<{type_raw}>({expr_cpp})"
        return f"static_cast<bool>(std::dynamic_pointer_cast<{type_raw}>({expr_cpp}))"

    def _emit_downcast(self, expr_cpp: str, type_raw: str, is_primitive_target: bool) -> str:
        if is_primitive_target:
            return f"(({type_raw}){expr_cpp})"
        if self.downcast_style == "macro":
            return f"{self.downcast_macro}({type_raw}, {expr_cpp})"
        if self.downcast_style == "func":
            return f"{self.downcast_func}<{type_raw}>({expr_cpp})"
        return (
            #f"([&]() -> jxx::Ptr<{type_raw}> {{ "
            f"([&]{pe_raw}> {{ "
            f"auto __o = {expr_cpp}; "
            f"if (!__o) return jxx::Ptr<{type_raw}>{{}}; "
            f"auto __t = std::dynamic_pointer_cast<{type_raw}>(__o); "
            f"if (__t) return __t; "
            f"throw jxx::lang::ClassCastException(jxx::lang::String(\"ClassCastException: incompatible cast\")); "
            f"}})()"
        )

    # ---------------- member access heuristic (static vs instance vs dot-vars) ----------------
    def _qual_is_static(self, q: str) -> bool:
        if not q:
            return False
        head = q.split(".")[0].split("::")[0].split("->")[0]
        if head in ("this", "super"):
            return False
        if self._sym_get(head) is not None or self._dot_has(head):
            return False
        return bool(head) and head[0].isupper()

    def _format_qual(self, q: str) -> Tuple[str, str]:
        if not q:
            return ("", "")
        if "::" in q:
            return (q, "::")
        if "->" in q:
            return (q, "->")

        head = q.split(".")[0].split("::")[0].split("->")[0]
        if self._dot_has(head):
            return (q, ".")

        if self._qual_is_static(q):
            return (q.replace(".", "::"), "::")

        return (q.replace(".", "->"), "->")

    # ---------------- new-expression helpers (allocation style 1) ----------------
    def _emit_new(self, raw_cpp: str, args: List[str]) -> str:
        if self.new_macro_style == "template":
            return f"{self.new_macro}<{raw_cpp}>({', '.join(args)})" if args else f"{self.new_macro}<{raw_cpp}>()"
        return f"{self.new_macro}({raw_cpp})({', '.join(args)})" if args else f"{self.new_macro}({raw_cpp})()"

    def _emit_array_init_expr(self, elem_cpp: str, rank: int, init_node) -> str:
        """
        Build JxxArray initializer expression using your JxxArray constructors.
        - Rank-1: JxxArray(std::initializer_list<T>)
        - Rank>=2: JxxArray(std::initializer_list<jxx::Ptr<SubArray>>) so null rows are supported.
        """
        arr_raw = f"jxx::lang::JxxArray<{elem_cpp}, {rank}>"
        vals = getattr(init_node, "initializers", None) or []

        if rank == 1:
            inner = ", ".join(self.emit_expression(v) for v in vals)
            return self._emit_new(arr_raw, [f"std::initializer_list<{elem_cpp}>{{{inner}}}"])

        sub_raw = f"jxx::lang::JxxArray<{elem_cpp}, {rank-1}>"
        row_exprs: List[str] = []
        for v in vals:
            vn = type(v).__name__
            if vn == "Literal" and getattr(v, "value", None) == "null":
                row_exprs.append("nullptr")
                continue
            if vn == "ArrayInitializer":
                row_exprs.append(self._emit_array_init_expr(elem_cpp, rank - 1, v))
                continue
            row_exprs.append("nullptr /* TODO: non-initializer row */")

        inner = ", ".join(row_exprs)
        return self._emit_new(arr_raw, [f"std::initializer_list<jxx::Ptr<{sub_raw}>>{{{inner}}}"])

    # ---------------- expressions ----------------
    def emit_expression(self, e) -> str:
        if e is None:
            return ""
        tn = type(e).__name__

        if tn == "TernaryExpression":
            cond = self.emit_expression(e.condition)
            tval = self.emit_expression(e.if_true)
            fval = self.emit_expression(e.if_false)
            return f"(({cond}) ? ({tval}) : ({fval}))"

        if tn == 'Literal':
            # javalang represents Java null as the string "null"
            if e.value is None or e.value == 'null':
                return 'nullptr'
            return e.value

        if tn == "This":
            return "this"

        if tn == 'MemberReference':
            q = e.qualifier
            qstr = q if isinstance(q, str) else (self.emit_expression(q) if q is not None else '')
            fq, sep = self._format_qual(qstr)
            base = f"{fq}{sep}{e.member}" if fq else e.member

            for op in (getattr(e, 'prefix_operators', None) or []):
                base = op + base
            for op in (getattr(e, 'postfix_operators', None) or []):
                base = base + op

            return base

        if tn == "MethodInvocation":
            qual = e.qualifier
            qstr = qual if isinstance(qual, str) else (self.emit_expression(qual) if qual is not None else "")
            fq, sep = self._format_qual(qstr)
            args = [self.emit_expression(a) for a in (e.arguments or [])]
            head = f"{fq}{sep}{e.member}" if fq else e.member
            return f"{head}({', '.join(args)})" if args else f"{head}()"

        if tn == "Primary":
            return self._emit_primary(e)

        if tn == 'Cast':
            # Handle casts with array dimensions (e.g. byte[]), not just primitive/basic.
            dims = len(getattr(e.type, 'dimensions', None) or [])
            if dims > 0:
                # Special case: byte[] -> ByteArray
                base = None
                if type(e.type).__name__ == 'BasicType':
                    base = e.type.name
                else:
                    base = self._simple_name(str(getattr(e.type, 'name', '')))

                expr_cpp = self.emit_expression(e.expression)

                if base == 'byte' and dims == 1:
                    # Cast to Ptr<ByteArray> => raw type is ByteArray for your JXX_CAST macro/helper
                    return self._emit_downcast(expr_cpp, "ByteArray", is_primitive_target=False)

                # Generic array cast: cast to raw JxxArray<Elem,Rank>
                elem_cpp = self._array_elem_type(e.type)
                raw_arr = f"jxx::lang::JxxArray<{elem_cpp}, {dims}>"
                return self._emit_downcast(expr_cpp, raw_arr, is_primitive_target=False)

            # Non-array cast (primitive or object)
            type_raw = self._raw_type_for_cast(e.type)
            is_prim = (type(e.type).__name__ == 'BasicType')
            return self._emit_downcast(self.emit_expression(e.expression), type_raw, is_prim)

        if tn == "BinaryOperation":
            if getattr(e, "operator", None) == "instanceof":
                left = self.emit_expression(e.operandl)
                type_raw = self._raw_type_for_cast(e.operandr)
                return self._emit_instanceof(left, type_raw)
            return f"({self.emit_expression(e.operandl)} {e.operator} {self.emit_expression(e.operandr)})"

        if tn == "ClassCreator":
            name = self._raw_reference_name(e.type) if type(e.type).__name__ == "ReferenceType" else str(e.type)
            args = [self.emit_expression(a) for a in (e.arguments or [])]
            return self._emit_new(name, args)

        if tn == "Assignment":
            l = self.emit_expression(e.expressionl)
            r = self.emit_expression(e.value)
            op = e.type if e.type else "="
            return f"({l} {op} {r})"

        if tn == "ArrayCreator":
            dims = getattr(e, "dimensions", None) or []
            init = getattr(e, "initializer", None)

            # Rank detection: dims list or (type.dimensions) or init implies rank 1
            rank = len(dims)
            t_dims = getattr(e.type, "dimensions", None) or []
            rank = max(rank, len(t_dims))
            if init is not None and rank == 0:
                rank = 1

            elem_cpp = self._array_elem_type(e.type)

            # Special case byte[] -> Ptr<ByteArray>
            base_name = self._simple_name(str(getattr(e.type, "name", "")))
            if type(e.type).__name__ == "BasicType":
                base_name = e.type.name
            if base_name == "byte" and rank == 1:
                if init is not None:
                    # ByteArray initializer_list not defined in your aliases; fall back to length + assigns not supported here
                    return "nullptr /* TODO: ByteArray initializer */"
                n0 = self.emit_expression(dims[0]) if dims else "0"
                return self._emit_new("ByteArray", [n0])

            # initializer present: new T[]{...}
            if init is not None:
                return self._emit_array_init_expr(elem_cpp, rank, init)

            # allocate by dimensions
            arr_raw = f"jxx::lang::JxxArray<{elem_cpp}, {rank}>"
            if rank == 1:
                n0 = self.emit_expression(dims[0])
                return self._emit_new(arr_raw, [n0])

            dim_list = ", ".join(self.emit_expression(d) for d in dims)
            return self._emit_new(arr_raw, [f"std::array<std::uint32_t, {rank}>{{{dim_list}}}"])

        # ArrayInitializer should be handled where declared type is known (fields/locals). Fallback:
        if tn == "ArrayInitializer":
            return "nullptr /* TODO: ArrayInitializer needs declared type */"

        return str(e)

    def _emit_primary(self, p) -> str:
        base = ""
        if getattr(p, "qualifier", None):
            base = p.qualifier
        elif getattr(p, "member", None):
            base = p.member
        elif getattr(p, "value", None):
            base = p.value
        elif getattr(p, "expression", None) is not None:
            base = self.emit_expression(p.expression)
        base = base or ""

        for op in (getattr(p, "prefix_operators", None) or []):
            base = op + base

        selectors = list(getattr(p, "selectors", None) or [])
        if not selectors:
            for op in (getattr(p, "postfix_operators", None) or []):
                base = base + op
            return base

        first_sep = "::" if self._qual_is_static(base) else "->"
        instance_chain = (first_sep == "->")
        cur = base.replace(".", "::") if first_sep == "::" else base.replace(".", "->")

        for sel in selectors:
            st = type(sel).__name__
            if st == "MethodInvocation":
                args = [self.emit_expression(a) for a in (sel.arguments or [])]
                sep = "->" if instance_chain else first_sep
                cur = f"{cur}{sep}{sel.member}({', '.join(args)})" if args else f"{cur}{sep}{sel.member}()"
                instance_chain = True
            elif st == "MemberReference":
                sep = "->" if instance_chain else first_sep
                cur = f"{cur}{sep}{sel.member}"
                instance_chain = True
            elif st == "ArraySelector":
                idx = self.emit_expression(sel.index)
                cur = f"(*({cur}))[{idx}]"
                instance_chain = True
            else:
                sep = "->" if instance_chain else first_sep
                cur = f"{cur}{sep}{str(sel)}"
                instance_chain = True

        for op in (getattr(p, "postfix_operators", None) or []):
            cur = cur + op
        return cur

    # ---------------- assert statement ----------------
    def _emit_assert_statement(self, s, out: Emit) -> None:
        cond_cpp = self.emit_expression(getattr(s, "condition", None))
        msg_node = getattr(s, "value", None) or getattr(s, "message", None)

        if msg_node is None:
            out.write(f"assert({cond_cpp});")
            return

        if type(msg_node).__name__ == "Literal" and isinstance(getattr(msg_node, "value", None), str):
            msg_cpp = self.emit_expression(msg_node)
            out.write(f"assert(({cond_cpp}) && {msg_cpp});")
            return

        out.write(f"assert({cond_cpp}); /* Java assert message expr omitted (non-literal) */")

    # ---------------- synchronized statement ----------------
    def _emit_synchronized_statement(self, s, out: Emit) -> None:
        lock_expr = getattr(s, "lock", None) or getattr(s, "expression", None)
        block = getattr(s, "block", None) or []
        tmp = self._fresh_tmp("__sync_lock_")

        out.write("{")
        out.enter()
        self._sym_push()

        out.write(f"auto {tmp} = {self.emit_expression(lock_expr)};")
        out.write(f"if (!{tmp}) throw jxx::lang::NullPointerException(jxx::lang::String(\"synchronized on null\"));")

        method_ret = self._ret_current()
        block_has_return = any(self._contains_return(st) for st in block)

        # If synchronized block contains return inside non-void method, return synchronized() result (Java parity)
        if method_ret != "void" and block_has_return:
            out.write(f"return {tmp}->synchronized([&-> {method_ret} {{")
            out.enter()
            self._sym_push()
            for st in block:
                self.emit_statement(st, out)
            self._sym_pop()
            out.exit()
            out.write("});")
        else:
            out.write(f"{tmp}->synchronized([&{{")
            out.enter()
            self._sym_push()
            for st in block:
                self.emit_statement(st, out)
            self._sym_pop()
            out.exit()
            out.write("});")

        self._sym_pop()
        out.exit()
        out.write("}")

    # ---------------- statements ----------------
    def _emit_do_statement(self, s, out: Emit) -> None:
        cond = self.emit_expression(getattr(s, "condition", None))
        body = getattr(s, "body", None) or getattr(s, "statement", None)
        out.write("do {")
        out.enter(); self._sym_push()
        if body is not None:
            self.emit_statement(body, out)
        self._sym_pop(); out.exit()
        out.write(f"}} while ({cond});")

    def emit_statement(self, s, out: Emit) -> None:
        if s is None:
            return
        tn = type(s).__name__

        # wrapper Statement nodes (some javalang versions)
        if tn == "Statement":
            inner = getattr(s, "statement", None)
            if inner is not None:
                self.emit_statement(inner, out)
                return
            inner_expr = getattr(s, "expression", None)
            if inner_expr is not None:
                out.write(f"{self.emit_expression(inner_expr)};")
                return
            inner_stmts = getattr(s, "statements", None)
            if inner_stmts:
                for st in inner_stmts:
                    self.emit_statement(st, out)
                return
            out.write("/* TODO: Empty/unknown wrapper Statement */")
            return

        if tn == "AssertStatement":
            self._emit_assert_statement(s, out)
            return

        if tn == "SynchronizedStatement":
            self._emit_synchronized_statement(s, out)
            return

        if tn == "DoStatement":
            self._emit_do_statement(s, out)
            return

        if tn == "BlockStatement":
            inner = getattr(s, "statements", None)
            if inner:
                for st in inner:
                    self.emit_statement(st, out)
            else:
                decl = getattr(s, "declaration", None)
                if decl is not None:
                    self.emit_statement(decl, out)
            return

        if tn == "ReturnStatement":
            if s.expression is None:
                out.write("return;")
            else:
                out.write(f"return {self.emit_expression(s.expression)};")
            return

        if tn == "StatementExpression":
            out.write(f"{self.emit_expression(s.expression)};")
            return

        if tn == "LocalVariableDeclaration":
            tcpp = self._map_type(s.type)
            mods = set(getattr(s, 'modifiers', []) or [])
            is_final = ('final' in mods)

            # Raw name for object construction (only used when we auto-init a final ref)
            raw = None
            if type(s.type).__name__ == 'ReferenceType':
                # Use raw type name, NOT Ptr<...>
                raw = self._raw_reference_name(s.type)

            for decl in s.declarators:
                self._sym_set(decl.name, tcpp)
                init = decl.initializer

                if init is None:
                    # If it's "final <RefType> x;" with no initializer, synthesize default new
                    # => const Ptr<RefType> x = JXX_NEW<RefType>();
                    if is_final and raw is not None:
                        expr = self._emit_new(raw, []) if hasattr(self, '_emit_new') else (
                            f"{self.new_macro}<{raw}>()" if self.new_macro_style == 'template'
                            else f"{self.new_macro}({raw})()"
                        )
                        out.write(f"const {tcpp} {decl.name} = {expr};")
                    else:
                        # Not final (or not a reference type): keep uninitialized declaration
                        out.write(f"{tcpp} {decl.name};")
                else:
                    # If final and there is an initializer, make it const (Java final == immutable binding)
                    if is_final:
                        out.write(f"const {tcpp} {decl.name} = {self.emit_expression(init)};")
                    else:
                        out.write(f"{tcpp} {decl.name} = {self.emit_expression(init)};")

            return

        if tn == "IfStatement":
            cond = self.emit_expression(s.condition)
            out.write(f"if ({cond}) {{")
            out.enter(); self._sym_push()
            self.emit_statement(s.then_statement, out)
            self._sym_pop(); out.exit()
            if s.else_statement is not None:
                out.write("} else {")
                out.enter(); self._sym_push()
                self.emit_statement(s.else_statement, out)
                self._sym_pop(); out.exit()
            out.write("}")
            return

        if tn == "WhileStatement":
            cond = self.emit_expression(s.condition)
            out.write(f"while ({cond}) {{")
            out.enter(); self._sym_push()
            self.emit_statement(s.body, out)
            self._sym_pop(); out.exit()
            out.write("}")
            return

        if tn == "ForStatement":
            self._emit_for_statement(s, out)
            return

        if tn == "TryStatement":
            self._emit_try_statement(s, out)
            return

        if tn == "SwitchStatement":
            self._emit_switch_statement(s, out)
            return

        if tn == "BreakStatement":
            out.write("break;")
            return

        if tn == "ContinueStatement":
            out.write("continue;")
            return

        if tn == "ThrowStatement":
            self._emit_throw_statement(s, out)
            return

        out.write(f"/* TODO: Unhandled statement: {tn} */")

    def _emit_throw_statement(self, s, out: Emit) -> None:
        expr = s.expression
        et = type(expr).__name__

        if et == "ClassCreator":
            raw = self._raw_reference_name(expr.type) if type(expr.type).__name__ == "ReferenceType" else str(expr.type)
            args = [self.emit_expression(a) for a in (expr.arguments or [])]
            ctor = f"{raw}({', '.join(args)})" if args else f"{raw}()"
            out.write(f"throw {ctor};")
            return

        e_cpp = self.emit_expression(expr)

        if et in ("MemberReference", "Identifier"):
            name = getattr(expr, "member", None) or getattr(expr, "name", None) or ""
            ctype = self._sym_get(name) if name else None
            if ctype and ctype.startswith("jxx::Ptr<"):
                warn = ""
                for bt in ("jxx::lang::Throwable", "jxx::lang::Exception", "jxx::lang::RuntimeException"):
                    if bt in ctype:
                        warn = " /* WARNING: potential slicing (Ptr to base exception) */"
                        break
                out.write(f"throw *({e_cpp});{warn}")
                return

        out.write(f"throw {e_cpp};")

    def _emit_for_statement(self, s, out: Emit) -> None:
        control = s.control
        ct = type(control).__name__

        # Enhanced for: for (T x : iterable)
        if ct == 'EnhancedForControl':
            var = control.var
            vtype = getattr(var, 'type', None)
            name = getattr(var, 'name', None)

            if name is None:
                decls = getattr(var, 'declarators', None) or []
                if decls:
                    name = getattr(decls[0], 'name', None)

            if vtype is None or name is None:
                out.write("/* TODO: Unhandled enhanced-for variable shape */")
                out.write("for (/*?*/; /*?*/; /*?*/) {")
                out.enter();
                self._sym_push()
                self.emit_statement(s.body, out)
                self._sym_pop();
                out.exit()
                out.write("}")
                return

            tcpp = self._map_type(vtype)
            self._sym_set(name, tcpp)

            iterable = self.emit_expression(control.iterable)

            out.write(f"for ({tcpp} {name} : *({iterable})) {{")
            out.enter();
            self._sym_push()
            self.emit_statement(s.body, out)
            self._sym_pop();
            out.exit()
            out.write("}")
            return

        if ct == 'ForControl':
            # Normalize init/update: javalang may return a single node, not a list
            init_nodes = getattr(control, 'init', None)
            if init_nodes is None:
                init_nodes = []
            elif not isinstance(init_nodes, list):
                init_nodes = [init_nodes]

            update_nodes = getattr(control, 'update', None)
            if update_nodes is None:
                update_nodes = []
            elif not isinstance(update_nodes, list):
                update_nodes = [update_nodes]

            # --- init ---
            init_parts: List[str] = []
            for init in init_nodes:
                init = self._unwrap_node(init)
                it = type(init).__name__

                if it in ('LocalVariableDeclaration', 'VariableDeclaration'):
                    tcpp = self._map_type(init.type)
                    for d in (init.declarators or []):
                        self._sym_set(d.name, tcpp)
                        if d.initializer is None:
                            init_parts.append(f"{tcpp} {d.name}")
                        else:
                            init_parts.append(f"{tcpp} {d.name} = {self.emit_expression(d.initializer)}")
                else:
                    init_parts.append(self.emit_expression(init))

            init_str = ", ".join(init_parts)

            # --- condition ---
            cond = getattr(control, 'condition', None)
            cond_str = self.emit_expression(cond) if cond is not None else ""

            # --- update ---
            upd_parts: List[str] = []
            for u in update_nodes:
                u = self._unwrap_node(u)
                upd_parts.append(self.emit_expression(u))
            upd_str = ", ".join(upd_parts)

            out.write(f"for ({init_str}; {cond_str}; {upd_str}) {{")
            out.enter();
            self._sym_push()
            self.emit_statement(s.body, out)
            self._sym_pop();
            out.exit()
            out.write("}")
            return

        out.write(f"/* TODO: Unhandled for-control: {ct} */")

    def _emit_try_statement(self, s, out: Emit) -> None:
        finally_block = getattr(s, "finally_block", None)
        if finally_block is not None:
            self._needs_functional = True
            out.write("struct __JxxFinallyGuard { std::function<void()> f; ~__JxxFinallyGuard(){ if(f) f(); } };")
            out.write("__JxxFinallyGuard __finally{[&")
            out.enter(); self._sym_push()
            if isinstance(finally_block, list):
                for st in finally_block:
                    self.emit_statement(st, out)
            else:
                self.emit_statement(finally_block, out)
            self._sym_pop(); out.exit()
            out.write("}};")
            out.write("")

        out.write("try {")
        out.enter(); self._sym_push()

        blk = getattr(s, "block", None)
        if blk is not None:
            if isinstance(blk, list):
                for st in blk:
                    self.emit_statement(st, out)
            else:
                self.emit_statement(blk, out)

        self._sym_pop(); out.exit()
        out.write("}")

        for cc in (s.catches or []):
            param = cc.parameter
            name = param.name
            types = getattr(param, "types", None) or [param.type]
            for ty in types:
                raw = self._raw_reference_name(ty) if type(ty).__name__ == "ReferenceType" else str(ty)
                out.write(f"catch (const {raw}& {name}) {{")
                out.enter(); self._sym_push()
                self._sym_set(name, raw)
                self._dot_add(name)  # dot-access for catch refs

                cblk = getattr(cc, "block", None)
                if cblk is not None:
                    if isinstance(cblk, list):
                        for st in cblk:
                            self.emit_statement(st, out)
                    else:
                        self.emit_statement(cblk, out)

                self._sym_pop(); out.exit()
                out.write("}")

    def _emit_switch_statement(self, s, out: Emit) -> None:
        expr = self.emit_expression(s.expression)
        out.write(f"switch ({expr}) {{")
        out.enter(); self._sym_push()
        for case in (s.cases or []):
            if case.case is None:
                out.write("default:")
            else:
                labels = case.case if isinstance(case.case, list) else [case.case]
                for lab in labels:
                    out.write(f"case {self.emit_expression(lab)}:")
            out.enter(); self._sym_push()
            for st in (case.statements or []):
                self.emit_statement(st, out)
            self._sym_pop(); out.exit()
        self._sym_pop(); out.exit()
        out.write("}")

    # ---------------- fields (with deferral for array initializers) ----------------
    def _emit_field_decls(self, out: Emit, cls_name: str, field_node) -> None:
        mods = set(field_node.modifiers or [])
        is_static = "static" in mods
        is_final = "final" in mods

        tcpp = self._map_type(field_node.type)

        # IMPORTANT:
        # For fields, Java `final` on pointer should ideally be modeled as const member,
        # but const members MUST be initialized in ctor init-list.
        # We'll support that via _deferred_ctor_initlist (see below).
        if is_final:
            tcpp = f"const {tcpp}"

        for decl in field_node.declarators:
            name = decl.name
            init = decl.initializer

            # ---- static fields: header declares, cpp defines ----
            if is_static:
                out.write(f"static {tcpp} {name};")
                if init is not None:
                    self._static_field_defs.append((cls_name, tcpp, name, self.emit_expression(init)))
                continue

            # ---- instance fields ----
            is_ptr = tcpp.startswith("jxx::Ptr<") or tcpp.startswith("const jxx::Ptr<")

            if init is None:
                # Your policy: all object fields set to nullptr in header
                if is_ptr:
                    out.write(f"{tcpp} {name} = nullptr;")
                else:
                    out.write(f"{tcpp} {name};")
                continue

            init_cpp = self.emit_expression(init)

            # Java null => keep nullptr, no deferred init
            if init_cpp == "nullptr":
                if is_ptr:
                    out.write(f"{tcpp} {name} = nullptr;")
                else:
                    out.write(f"{tcpp} {name} = nullptr;")  # rare but consistent
                continue

            # Defer ALL pointer initializers to .cpp (your rule)
            if is_ptr:
                out.write(f"{tcpp} {name} = nullptr;")

                if is_final:
                    # const member: MUST be initialized in ctor init-list
                    self._deferred_ctor_initlist.setdefault(cls_name, []).append((name, init_cpp))
                else:
                    # non-const: assign in ctor body
                    self._deferred_instance_inits.setdefault(cls_name, []).append((name, init_cpp))
            else:
                # Non-pointer fields can stay inline
                out.write(f"{tcpp} {name} = {init_cpp};")

    # ---------------- method signatures + override detection ----------------
    def _method_signature(self, m, class_name: str) -> Tuple[str, bool]:
        is_ctor = (type(m).__name__ == "ConstructorDeclaration")

        if not is_ctor:
            if m.return_type is not None:
                md = len(getattr(m, "dimensions", []) or [])
                ret = self._map_type(m.return_type, extra_dims=md)
            else:
                ret = "void"
        else:
            ret = ""

        name = m.name if not is_ctor else class_name
        params: List[str] = []
        for p in (m.parameters or []):
            ptype = self._map_type(p.type)
            params.append(f"{ptype} {p.name}")

        sig = f"{ret} {name}({', '.join(params)})" if not is_ctor else f"{name}({', '.join(params)})"
        return sig, is_ctor

    def _method_key(self, m) -> Optional[Tuple[str, Tuple[str, ...]]]:
        if type(m).__name__ != "MethodDeclaration":
            return None
        name = m.name
        params = tuple(self._map_type(p.type) for p in (m.parameters or []))
        return (name, params)

    def _has_override_annotation(self, m) -> bool:
        ann = getattr(m, "annotations", None) or []
        for a in ann:
            n = a.name[-1] if isinstance(a.name, (list, tuple)) else a.name
            if n == "Override":
                return True
        return False

    def _is_abstract_method(self, m) -> bool:
        mods = set(getattr(m, "modifiers", []) or [])
        return ("abstract" in mods) or (getattr(m, "body", None) is None)

    def _raw_inherit_name(self, ref_type) -> str:
        raw = ref_type.name.replace(".", "::") if isinstance(ref_type.name, str) else str(ref_type.name)
        return raw

    def _build_type_table(self, tree) -> None:
        self._type_table = {}
        self._override_keys = {}

        for t in (tree.types or []):
            tt = type(t).__name__
            if tt not in ("ClassDeclaration", "InterfaceDeclaration"):
                continue

            kind = "interface" if tt == "InterfaceDeclaration" else "class"
            name = t.name

            ext = getattr(t, "extends", None)
            if kind == "interface":
                if isinstance(ext, list):
                    extends = [self._raw_inherit_name(x) for x in ext]
                elif ext is None:
                    extends = []
                else:
                    extends = [self._raw_inherit_name(ext)]
            else:
                extends = self._raw_inherit_name(ext) if ext is not None else None

            implements = []
            if kind == "class":
                implements = [self._raw_inherit_name(x) for x in (getattr(t, "implements", None) or [])]

            methods = set()
            for member in (t.body or []):
                if type(member).__name__ == "MethodDeclaration":
                    k = self._method_key(member)
                    if k:
                        methods.add(k)

            self._type_table[name] = {"kind": kind, "extends": extends, "implements": implements, "methods": methods}

        def gather_inherited(type_name: str, seen: Set[str]) -> Set[Tuple[str, Tuple[str, ...]]]:
            if type_name in seen:
                return set()
            seen.add(type_name)

            info = self._type_table.get(type_name)
            if not info:
                return set()

            inherited = set()

            if info["kind"] == "class":
                base = info["extends"]
                if isinstance(base, str) and base in self._type_table:
                    inherited |= self._type_table[base]["methods"]
                    inherited |= gather_inherited(base, seen)

                for iface in info.get("implements", []):
                    if iface in self._type_table:
                        inherited |= self._type_table[iface]["methods"]
                        inherited |= gather_inherited(iface, seen)
            else:
                for parent in (info.get("extends", []) or []):
                    if parent in self._type_table:
                        inherited |= self._type_table[parent]["methods"]
                        inherited |= gather_inherited(parent, seen)

            return inherited

        for name, info in self._type_table.items():
            if info["kind"] != "class":
                continue
            inherited = gather_inherited(name, set())
            self._override_keys[name] = info["methods"] & inherited

    # ---------------- method definitions (with synchronized method support + ctor array init injection) ----------------
    def _emit_method_definition(self, out: Emit, cls_name: str, m) -> None:
        mt = type(m).__name__
        if mt not in ("MethodDeclaration", "ConstructorDeclaration"):
            return

        sig, is_ctor = self._method_signature(m, cls_name)

        if is_ctor:
            for fname, fexpr in self._deferred_instance_inits.get(cls_name, []):
                out.write(f"this->{fname} = {fexpr};")

            initlist_items = self._deferred_ctor_initlist.get(cls_name, [])
            initlist = ""
            if initlist_items:
                initlist = " : " + ", ".join(f"{fname}({fexpr})" for fname, fexpr in initlist_items)

            params = sig[len(cls_name) :]
            head = f"{cls_name}::{cls_name}{params}"
            body = getattr(m, "body", None)

            out.write(f"{head}{initlist} {{")
            out.enter()
            self._sym_push()
            self._ret_push("void")

            for p in (m.parameters or []):
                self._sym_set(p.name, self._map_type(p.type))

            # Inject deferred instance array inits at start of every ctor
            for fname, fexpr in self._deferred_instance_inits.get(cls_name, []):
                out.write(f"this->{fname} = {fexpr};")

            if body:
                for st in body:
                    self.emit_statement(st, out)

            self._ret_pop()
            self._sym_pop()
            out.exit()
            out.write("}")
            out.write("")
            return

        # Normal method
        ret_cpp, rest = sig.split(" ", 1)
        name = m.name
        params = rest[len(name) :]
        head = f"{ret_cpp} {cls_name}::{name}{params}"

        mods = set(getattr(m, "modifiers", []) or [])
        is_sync = ("synchronized" in mods)
        is_static = ("static" in mods)

        body = getattr(m, "body", None)

        if not is_sync:
            out.write(f"{head} {{")
            out.enter()
            self._sym_push()
            self._ret_push(ret_cpp)

            for p in (m.parameters or []):
                self._sym_set(p.name, self._map_type(p.type))

            if body:
                for st in body:
                    self.emit_statement(st, out)

            self._ret_pop()
            self._sym_pop()
            out.exit()
            out.write("}")
            out.write("")
            return

        # synchronized method wrapper
        lock_expr = f"{cls_name}::__jxx_class_lock" if is_static else "this"

        out.write(f"{head} {{")
        out.enter()
        self._sym_push()
        self._ret_push(ret_cpp)

        for p in (m.parameters or []):
            self._sym_set(p.name, self._map_type(p.type))

        if ret_cpp == "void":
            out.write(f"{lock_expr}->synchronized(& {{")
            out.enter(); self._sym_push()
            if body:
                for st in body:
                    self.emit_statement(st, out)
            self._sym_pop(); out.exit()
            out.write("});")
            out.write("return;")
        else:
            out.write(f"return {lock_expr}->synchronized([&-> {ret_cpp} {{")
            out.enter(); self._sym_push()
            if body:
                for st in body:
                    self.emit_statement(st, out)
            self._sym_pop(); out.exit()
            out.write("});")

        self._ret_pop()
        self._sym_pop()
        out.exit()
        out.write("}")
        out.write("")

    # ---------------- unit emission ----------------
    def _compute_guard_from_path(self, header_path: str) -> str:
        up = header_path.replace("\\", "/").upper()
        macro = "".join(ch if ch.isalnum() else "_" for ch in up)
        if not macro.endswith("_H") and not macro.endswith("_HPP"):
            macro += "_H"
        return macro

    def transpile_unit(
        self,
        java_src: str,
        mode: str = "monolith",
        header_rel_include: Optional[str] = None,
        header_guard_name: Optional[str] = None,
    ) -> str:
        if javalang is None:
            raise RuntimeError("javalang is required. pip install javalang")

        tree = javalang.parse.parse(java_src)

        # Reset per-unit collections (important because header/source are separate passes)
        self._static_field_defs = []
        self._deferred_instance_inits = {}

        self._scan_needs(tree)
        self.gather_usings(tree)
        self._build_type_table(tree)

        ns = tree.package.name.replace(".", "::") if tree.package else ""

        out = Emit()
        out.write("// Generated by java_to_cpp (AST-based)")
        self.include_written = set()

        guard_to_use = None
        if mode == "header":
            if self.header_guards:
                guard_to_use = header_guard_name or self.guard_name or "GENERATED_HEADER_GUARD_H"
                out.write(f"#ifndef {guard_to_use}")
                out.write(f"#define {guard_to_use}")
            elif self.pragma_once:
                out.write("#pragma once")

        if mode != "source":
            for inc in sorted(set(self.import_header_includes)):
                self._emit_include_operand(out, inc)
            self.emit_includes(out)
            out.write("// NOTE: For instanceof/cast macro/func styles, include your jxx.lang.Cast.h.")
        else:
            if not header_rel_include:
                raise ValueError("header_rel_include required for source mode")
            self._emit_include_operand(out, header_rel_include)
            self.emit_includes(out)

        out.write("")
        for line in sorted(self.using_lines):
            out.write(line)
        if self.using_lines:
            out.write("")

        if ns:
            out.write(f"namespace {ns} {{")
            out.write("")
            out.enter()

        # ---------- HEADER ----------
        if mode in ("monolith", "header"):
            for t in (tree.types or []):
                tt = type(t).__name__
                if tt not in ("ClassDeclaration", "InterfaceDeclaration"):
                    continue

                is_iface = (tt == "InterfaceDeclaration")

                if is_iface:
                    # Interface: pure virtual, no Object base
                    out.write(f"struct {t.name} {{")
                    out.enter()
                    out.write("public:")
                    out.enter()
                    out.write(f"virtual ~{t.name}() = default;")
                    for member in (t.body or []):
                        if type(member).__name__ == "MethodDeclaration":
                            sig, _ = self._method_signature(member, t.name)
                            out.write(f"virtual {sig} = 0;")
                    out.exit()
                    out.exit()
                    out.write("};")
                    out.write("")
                    continue

                # Class: extends + implements (multiple inheritance). Default extends Object.
                ext = getattr(t, "extends", None)
                impls = getattr(t, "implements", None) or []
                bases: List[str] = []
                if ext is None:
                    bases.append("jxx::lang::Object")
                else:
                    bases.append(self._raw_inherit_name(ext))
                for iface in impls:
                    bases.append(self._raw_inherit_name(iface))

                base_clause = " : " + ", ".join(f"public {b}" for b in bases) if bases else ""
                out.write(f"class {t.name}{base_clause} {{")
                out.enter()

                # Determine if this class needs a class-level lock for static synchronized methods
                needs_class_lock = False
                for member in (t.body or []):
                    if type(member).__name__ == "MethodDeclaration":
                        mods = set(getattr(member, "modifiers", []) or [])
                        if "synchronized" in mods and "static" in mods:
                            needs_class_lock = True
                            break

                # Collect fields during header pass to populate deferred array inits too
                # (so we can synthesize default ctor declaration if needed)
                for member in (t.body or []):
                    if type(member).__name__ == "FieldDeclaration":
                        self._emit_field_decls(Emit(), t.name, member)  # dummy to populate deferred inits

                has_ctor = any(type(member).__name__ == "ConstructorDeclaration" for member in (t.body or []))
                needs_default_ctor = (not has_ctor) and bool(self._deferred_instance_inits.get(t.name))

                # Group members by visibility
                buckets = {"public": [], "protected": [], "private": []}
                for member in (t.body or []):
                    mods = set(getattr(member, "modifiers", []) or [])
                    vis = _java_visibility(mods)
                    buckets[vis].append(member)

                for vis in ("public", "protected", "private"):
                    members = buckets[vis]

                    if vis == "private" and needs_class_lock:
                        init = self._emit_new("jxx::lang::Object", [])
                        out.write(f"inline static jxx::Ptr<jxx::lang::Object> __jxx_class_lock = {init};")

                    if not members and not (vis == "public" and needs_default_ctor):
                        continue

                    out.write(f"{vis}:")
                    out.enter()

                    if vis == "public" and needs_default_ctor:
                        out.write(f"{t.name}();")

                    for member in members:
                        mt = type(member).__name__
                        if mt == "FieldDeclaration":
                            self._emit_field_decls(out, t.name, member)
                        elif mt in ("MethodDeclaration", "ConstructorDeclaration"):
                            sig, is_ctor2 = self._method_signature(member, t.name)
                            if type(member).__name__ == "ConstructorDeclaration":
                                out.write(sig + ";")
                                continue

                            mk = self._method_key(member)
                            overrides = (mk in self._override_keys.get(t.name, set())) or self._has_override_annotation(member)
                            is_abs = self._is_abstract_method(member)

                            if is_abs:
                                if overrides:
                                    out.write(f"virtual {sig} override = 0;")
                                else:
                                    out.write(f"virtual {sig} = 0;")
                            else:
                                if overrides:
                                    out.write(f"virtual {sig} override;")
                                else:
                                    out.write(sig + ";")

                    out.exit()

                out.exit()
                out.write("};")
                out.write("")

        # ---------- SOURCE ----------
        if mode == "source":
            # Re-collect static field defs and deferred array inits in source pass
            self._static_field_defs = []
            self._deferred_instance_inits = {}

            for t in (tree.types or []):
                if type(t).__name__ not in ("ClassDeclaration", "InterfaceDeclaration"):
                    continue
                for member in (t.body or []):
                    if type(member).__name__ == "FieldDeclaration":
                        self._emit_field_decls(Emit(), t.name, member)

            # Emit static field definitions
            for cls_name, tcpp, name, init_expr in self._static_field_defs:
                out.write(f"{tcpp} {cls_name}::{name} = {init_expr};")
            if self._static_field_defs:
                out.write("")

            # Synthesize default ctor definitions if needed (deferred array inits exist but no ctors)
            for t in (tree.types or []):
                if type(t).__name__ != "ClassDeclaration":
                    continue
                cname = t.name
                has_ctor = any(type(m).__name__ == "ConstructorDeclaration" for m in (t.body or []))
                if (not has_ctor) and self._deferred_instance_inits.get(cname):
                    out.write(f"{cname}::{cname}() {{")
                    out.enter(); self._sym_push(); self._ret_push("void")
                    for fname, fexpr in self._deferred_instance_inits.get(cname, []):
                        out.write(f"this->{fname} = {fexpr};")
                    self._ret_pop(); self._sym_pop(); out.exit()
                    out.write("}")
                    out.write("")

            # Emit method/ctor bodies
            for t in (tree.types or []):
                if type(t).__name__ != "ClassDeclaration":
                    continue
                for member in (t.body or []):
                    if type(member).__name__ in ("MethodDeclaration", "ConstructorDeclaration"):
                        self._emit_method_definition(out, t.name, member)

        if ns:
            out.exit()
            out.write(f"}} // namespace {ns}")

        if mode == "header" and guard_to_use:
            out.write(f"#endif // {guard_to_use}")

        return out.text()


# ---------- CLI / IO ----------

def java_package_of(src: str) -> str:
    try:
        tree = javalang.parse.parse(src)
    except Exception:
        return ""
    return tree.package.name if tree.package else ""


def path_for_file(out_root: str, subdir: str, pkg: str, stem: str, ext: str) -> str:
    pkg_path = pkg.replace(".", os.sep) if pkg else ""
    full_dir = os.path.join(out_root, subdir, pkg_path)
    os.makedirs(full_dir, exist_ok=True)
    return os.path.join(full_dir, f"{stem}{ext}")


def rel_include(from_file: str, header_file: str) -> str:
    return os.path.relpath(header_file, start=os.path.dirname(from_file)).replace(os.sep, "/")


def generate_cmake(
    out_root: str,
    project: str = "TranspiledProject",
    target: str = "transpiled",
    kind: str = "static",
    cxx_standard: str = "17",
    cmake_min: str = "3.20",
) -> str:
    kind_map = {"static": "STATIC", "shared": "SHARED", "object": "OBJECT", "interface": "INTERFACE"}
    kind_kw = kind_map.get(kind.lower(), "STATIC")

    cpp_files: List[str] = []
    for root, _, files in os.walk(os.path.join(out_root, "src")):
        for fn in files:
            if fn.lower().endswith(".cpp"):
                cpp_files.append(os.path.join(root, fn).replace("\\", "/"))
    rel_cpp = [os.path.relpath(p, start=out_root).replace("\\", "/") for p in cpp_files]

    L: List[str] = []
    L.append(f"cmake_minimum_required(VERSION {cmake_min})")
    L.append(f"project({project} LANGUAGES CXX)")
    L.append("")
    L.append(f"set(CMAKE_CXX_STANDARD {cxx_standard})")
    L.append("set(CMAKE_CXX_STANDARD_REQUIRED ON)")
    L.append("set(CMAKE_CXX_EXTENSIONS OFF)")
    L.append("")

    if kind_kw == "INTERFACE":
        L.append(f"add_library({target} INTERFACE)")
        L.append(f"target_include_directories({target} INTERFACE ${{CMAKE_CURRENT_SOURCE_DIR}}/include)")
    else:
        if rel_cpp:
            L.append(f"add_library({target} {kind_kw}\n    " + "\n    ".join(rel_cpp) + "\n)")
        else:
            L.append(f"file(GLOB_RECURSE {target}_SRCS CONFIGURE_DEPENDS src/*.cpp)")
            L.append(f"add_library({target} {kind_kw} ${{{target}_SRCS}})")
        L.append(f"target_include_directories({target} PUBLIC ${{CMAKE_CURRENT_SOURCE_DIR}}/include)")

    L.append("")
    L.append("# add_executable(app src/main.cpp)")
    L.append(f"# target_link_libraries(app PRIVATE {target})")
    L.append("")
    return "\n".join(L)


def build_arg_parser() -> argparse.ArgumentParser:
    ap = argparse.ArgumentParser(description="Java → C++17 transpiler (value exceptions + arrays + interfaces + override)")

    ap.add_argument("input", nargs="?", default=None, help="Input Java file (omit when using --dir)")
    ap.add_argument("-o", "--output", default=None, help="Output C++ file (monolith) OR output root when --pair/--dir")
    ap.add_argument("--pair", action="store_true", help="For single input, emit header+source pair under --out root")
    ap.add_argument("--dir", default=None, help="Directory to recursively search for .java files")
    ap.add_argument("--out", default=None, help="Output root for --pair/--dir")

    ap.add_argument("--primitive-map", action="append", default=[], help="Override primitive mapping, e.g. int=jint")

    ap.add_argument("--string-policy", choices=["preserve", "std"], default="preserve")
    ap.add_argument("--string-include", default=None)
    ap.add_argument("--bytearray-include", default=None)
    ap.add_argument("--include", action="append", default=[])
    ap.add_argument("--no-default-includes", action="store_true")

    ap.add_argument("--header-guards", action="store_true")
    ap.add_argument("--guard-name", default=None)
    ap.add_argument("--no-pragma-once", action="store_true")

    ap.add_argument("--new-macro-style", choices=["template", "curried"], default="template")
    ap.add_argument("--new-macro", default="JXX_NEW")

    ap.add_argument("--instanceof-style", choices=["macro", "func", "dynamic"], default="macro")
    ap.add_argument("--downcast-style", choices=["macro", "func", "dynamic"], default="macro")
    ap.add_argument("--instanceof-macro", default="JXX_INSTANCEOF")
    ap.add_argument("--downcast-macro", default="JXX_CAST")
    ap.add_argument("--instanceof-func", default="::jxx::lang::instanceof_as")
    ap.add_argument("--downcast-func", default="::jxx::lang::cast_as")

    ap.add_argument("--cmake", action="store_true")
    ap.add_argument("--cmake-project", default="TranspiledProject")
    ap.add_argument("--cmake-target", default="transpiled")
    ap.add_argument("--cmake-kind", choices=["static", "shared", "object", "interface"], default="static")
    ap.add_argument("--cmake-cxx-standard", default="17")
    ap.add_argument("--cmake-min", default="3.20")

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
        instanceof_style=args.instanceof_style,
        downcast_style=args.downcast_style,
        instanceof_macro=args.instanceof_macro,
        downcast_macro=args.downcast_macro,
        instanceof_func=args.instanceof_func,
        downcast_func=args.downcast_func,
    )


def handle_single_file_monolith(tp: Transpiler, in_path: str, out_path: Optional[str]) -> None:
    with open(in_path, "r", encoding="utf-8") as f:
        src = f.read()
    text = tp.transpile_unit(src, mode="monolith")
    if out_path:
        with open(out_path, "w", encoding="utf-8") as f:
            f.write(text)
        print(f"Wrote {out_path}")
    else:
        sys.stdout.write(text)


def handle_single_file_pair(tp: Transpiler, in_path: str, out_root: str) -> Tuple[str, str]:
    with open(in_path, "r", encoding="utf-8") as f:
        src = f.read()
    pkg = java_package_of(src)
    stem = os.path.splitext(os.path.basename(in_path))[0]

    header_path = path_for_file(out_root, "include", pkg, stem, ".h")
    source_path = path_for_file(out_root, "src", pkg, stem, ".cpp")

    guard = tp.guard_name or (tp._compute_guard_from_path(header_path) if tp.header_guards else None)

    text_h = tp.transpile_unit(src, mode="header", header_guard_name=guard)
    with open(header_path, "w", encoding="utf-8") as f:
        f.write(text_h)

    rel = rel_include(source_path, header_path)
    text_cc = tp.transpile_unit(src, mode="source", header_rel_include=rel)
    with open(source_path, "w", encoding="utf-8") as f:
        f.write(text_cc)

    print(f"Wrote {header_path}\nWrote {source_path}")
    return header_path, source_path


def handle_directory(tp: Transpiler, src_dir: str, out_root: str) -> None:
    for root, _, files in os.walk(src_dir):
        for fn in files:
            if fn.lower().endswith(".java"):
                handle_single_file_pair(tp, os.path.join(root, fn), out_root)


def main() -> None:
    ap = build_arg_parser()
    args = ap.parse_args()

    if (args.dir or args.pair) and not args.out:
        ap.error("--out is required when using --dir or --pair")

    if javalang is None:
        print("ERROR: This script requires 'javalang'. Install with: pip install javalang", file=sys.stderr)
        sys.exit(2)

    tp = make_transpiler(args)

    if args.dir:
        handle_directory(tp, args.dir, args.out)
        if args.cmake:
            cmk = generate_cmake(
                args.out,
                project=args.cmake_project,
                target=args.cmake_target,
                kind=args.cmake_kind,
                cxx_standard=args.cmake_cxx_standard,
                cmake_min=args.cmake_min,
            )
            cmake_path = os.path.join(args.out, "CMakeLists.txt")
            with open(cmake_path, "w", encoding="utf-8") as f:
                f.write(cmk)
            print(f"Wrote {cmake_path}")
        return

    if not args.input:
        ap.error("Please provide an input Java file or use --dir.")

    if args.pair:
        handle_single_file_pair(tp, args.input, args.out)
        return

    handle_single_file_monolith(tp, args.input, args.output)


if __name__ == "__main__":
    main()


