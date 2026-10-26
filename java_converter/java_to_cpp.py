#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Java 8 to JXX C++17 translator.

Focused modernization of the existing AST translator:
* reference returns/fields/locals use ::jxx::Ptr<T>
* reference parameters use const ::jxx::Ptr<T>&
* primitives are fully qualified
* classes use ClassBase; interfaces use InterfaceBase
* package include paths and javax -> ext mapping
* arrays use JxxArray; no ByteArray special case
* allocations/casts use ::jxx::NEW and ::jxx::CAST
* unsupported reflection/annotation/instrument/invoke packages are diagnosed

Requires: pip install javalang
"""
from __future__ import annotations
import argparse, os, re, sys
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, List, Optional, Sequence, Set, Tuple
try:
    import javalang
except Exception:
    javalang=None

PRIMITIVES={
 'byte':'::jxx::lang::jbyte','short':'::jxx::lang::jshort',
 'int':'::jxx::lang::jint','long':'::jxx::lang::jlong',
 'float':'::jxx::lang::jfloat','double':'::jxx::lang::jdouble',
 'boolean':'::jxx::lang::jbool','char':'::jxx::lang::jchar','void':'void'}
UNSUPPORTED=('java.lang.annotation','java.lang.instrument','java.lang.invoke')
# Names supplied by the implicit language package or by the converter's
# standard support includes must not be resolved through an on-demand import.
DEFAULT_LANGUAGE_TYPES={'Object','String','StringBuilder','StringBuffer','System','JxxArray','Class','Integer','Character','Boolean','Byte','Short','Long','Float','Double','Throwable','Exception','RuntimeException','Error'}
SDK_TYPES_BY_PACKAGE={
    'java.util': {
        'Collection','List','ArrayList','LinkedList','Set','HashSet',
        'LinkedHashSet','SortedSet','TreeSet','Map','HashMap','LinkedHashMap',
        'SortedMap','TreeMap','Queue','Deque','ArrayDeque','Iterator',
        'ListIterator','Enumeration','Comparator','Optional','Scanner',
        'StringTokenizer','Vector','Stack','IntSummaryStatistics',
        'LongSummaryStatistics','DoubleSummaryStatistics','IntStream',
        'LongStream','DoubleStream','Stream',
    },
    'java.math': {'BigInteger','BigDecimal','MathContext','RoundingMode'},
}
JAVA_UTIL_TYPES=SDK_TYPES_BY_PACKAGE['java.util']

SPECIAL_TYPES={
    'java.lang.reflect.InvocationTargetException':'java.lang.InvocationTargetException',
    'java.util.Map.Entry':'java.util.MapEntry',
}
METHOD_RESULT_TYPES={
    # java.lang.String stream-producing methods. String.h may only forward
    # declare IntStream, but fluent calls such as chars().distinct() require
    # the complete type in the generated translation unit.
    'chars':'java.util.IntStream',
    'codePoints':'java.util.IntStream',
}

STATIC_ACCESSORS={
    'java.util.concurrent.TimeUnit':{
        'NANOSECONDS','MICROSECONDS','MILLISECONDS','SECONDS','MINUTES','HOURS','DAYS'},
    'javax.xml.xpath.XPathConstants':{'NUMBER','STRING','BOOLEAN','NODESET','NODE'},
    'javax.xml.transform.OutputKeys':{
        'METHOD','VERSION','ENCODING','OMIT_XML_DECLARATION','STANDALONE',
        'DOCTYPE_PUBLIC','DOCTYPE_SYSTEM','CDATA_SECTION_ELEMENTS','INDENT','MEDIA_TYPE'},
}

@dataclass(frozen=True)
class CType:
    raw:str
    primitive:bool=False
    void:bool=False
    def value(self)->str:
        return self.raw if self.primitive or self.void else f'::jxx::Ptr<{self.raw}>'
    def param(self)->str:
        v=self.value(); return v if self.primitive or self.void else f'const {v}&'

class Emit:
    def __init__(self): self.lines=[]; self.n=0
    def w(self,s=''): self.lines.append(('    '*self.n+s) if s else '')
    def enter(self): self.n+=1
    def exit(self): self.n=max(0,self.n-1)
    def text(self): return '\n'.join(self.lines)+'\n'

class Translator:
    def __init__(self):
        self.imports:Dict[str,str]={}; self.wildcard_imports:List[str]=[]
        self.includes:Set[str]=set(); self.diag:List[str]=[]
        self.package=''; self.current_class=''; self.current_return='void'
        self.known_types:Set[str]=set(); self.static_fields:Set[str]=set()
        self.main_owner_names:Set[str]=set()
        self.main_entries:Dict[str,str]={}
        self.generated_files:List[Path]=[]
        self.type_parameters:Set[str]=set()
        self.declared_type_parameters:Set[str]=set()
        self.generic_class_parameters:Dict[str,List[str]]={}
        self.generic_constructor_parameters:Dict[str,Dict[int,str]]={}
        self.source_class_bases:Dict[str,str]={}
        self.source_constructor_parameter_types:Dict[str,List[object]]={}
        self.source_field_types:Dict[str,Dict[str,object]]={}
        self.local_variable_types:Dict[str,object]={}
        self.source_type_index:Dict[str,Tuple[str,str]]={}

    @staticmethod
    def simple(n): return str(n).split('.')[-1]

    @staticmethod
    def name_of(t):
        if t is None:
            return ""

        if isinstance(t, str):
            return t

        name = getattr(t, "name", None)

        if name is None:
            return str(t)

        if isinstance(name, str):
            return name

        if isinstance(name, (list, tuple)):
            return ".".join(
                Translator.name_of(x)
                for x in name
            )

        return str(name)
    def wildcard_sdk_type(self,name:str)->Optional[str]:
        matches=[package for package in self.wildcard_imports
                 if name in SDK_TYPES_BY_PACKAGE.get(package,set())]
        if len(matches)==1:
            return f'{matches[0]}.{name}'
        if len(matches)>1:
            self.diag.append(
                f'ambiguous wildcard SDK type {name}: ' + ', '.join(matches))
        return None

    def qmap(self,name:str)->Tuple[str,Optional[str]]:
        # Resolve implicit language-package types before considering wildcard
        # imports. Otherwise import java.util.* incorrectly maps String to
        # ::jxx::util::String and emits util/jxx.util.String.h.
        if name in DEFAULT_LANGUAGE_TYPES:
            if name=='String': name='java.lang.String'
            elif name=='Object': name='java.lang.Object'
            elif name=='System': name='java.lang.System'
            elif name=='Class': name='java.lang.ClassAny'
            elif name=='JxxArray': return '::jxx::JxxArray',None
            else: name=f'java.lang.{name}'
        elif name in self.imports:
            imported_name=self.imports[name]
            if imported_name in self.source_type_index:
                qualified,include=self.source_type_index[imported_name]
                return qualified,include
            name=imported_name
        elif any(name in SDK_TYPES_BY_PACKAGE.get(package,set())
                 for package in self.wildcard_imports):
            # Resolve known SDK classes from all imported wildcard packages,
            # even when the source imports more than one package on demand.
            wildcard_name=self.wildcard_sdk_type(name)
            if wildcard_name is not None:
                name=wildcard_name
        elif name in self.source_type_index and name not in self.known_types:
            qualified,include=self.source_type_index[name]
            return qualified,include
        elif ('.' not in name and name not in self.known_types and
              name not in self.type_parameters and
              name not in self.declared_type_parameters and
              len(self.wildcard_imports) == 1):
            # Resolve an unqualified type through the single on-demand import.
            # ref_raw() then records the concrete JXX class header.
            name=f'{self.wildcard_imports[0]}.{name}'
        name=SPECIAL_TYPES.get(name,name)
        if name=='String': name='java.lang.String'
        if name=='Object': name='java.lang.Object'
        if name=='Class': name='java.lang.ClassAny'
        p=name.split('.')
        if len(p)==1:
            return p[0],None
        typ=p[-1]; pkg=p[:-1]
        if pkg[0]=='java': mapped=pkg[1:]; cpp=['jxx']+mapped; prefix='jxx'
        elif pkg[0]=='javax': mapped=['ext']+pkg[1:]; cpp=['jxx']+mapped; prefix='jxx'
        else: mapped=pkg; cpp=['jxx']+pkg; prefix='jxx'
        raw='::'+'::'.join(cpp+[typ])
        dotted='.'.join(mapped); folder='/'.join(mapped)
        fn=f'{prefix}.{dotted}.{typ}.h'
        return raw,(f'{folder}/{fn}' if folder else fn)

    def qualified_static_owner(self, qualifier:str)->str:
        parts=qualifier.split('.')
        root=parts[0]
        mapped=self.qmap(self.imports.get(root,root))[0]
        if len(parts)>1:
            # The root denotes a type, so its first member uses static access.
            # Any remaining dotted components are object fields held through
            # JXX shared pointers and therefore use pointer member access.
            mapped+='::'+parts[1]
            for field in parts[2:]:
                mapped+=f'->{field}'
        return mapped

    def ref_raw(self,t)->str:
        name=self.name_of(t); raw,inc=self.qmap(name)
        if inc: self.includes.add(inc)
        args=[]
        declared_arguments=getattr(t,'arguments',None) or []
        for a in declared_arguments:
            at=getattr(a,'type',None)
            if at is None: args.append('::jxx::Ptr<::jxx::lang::Object>')
            else: args.append(self.resolve(at).value())
        if not args:
            generic_parameters=self.generic_class_parameters.get(name,[])
            # A member generic such as node<T> may appear with a diamond or as
            # a raw source spelling inside its generic owner. When every class
            # parameter is active in the current template context, preserve
            # that specialization instead of emitting the invalid C++ node.
            if (generic_parameters and
                    all(parameter in self.type_parameters
                        for parameter in generic_parameters)):
                args=list(generic_parameters)
        if args: raw += '<'+', '.join(args)+'>'
        return raw

    def resolve(self,t,extra_dims=0)->CType:
        if t is None: return CType('void',void=True)
        tn=type(t).__name__; name=self.name_of(t)
        dims=len(getattr(t,'dimensions',None) or [])+extra_dims
        # A method or class type parameter remains the C++ template argument,
        # while any source array dimensions are still applied around it.
        if name in self.type_parameters:
            base=CType(name,primitive=True)
        elif tn=='BasicType' or name in PRIMITIVES:
            base=CType(PRIMITIVES.get(name,name),primitive=True,void=(name=='void'))
        else: base=CType(self.ref_raw(t))
        if not dims: return base
        elem=base.value() if not base.primitive else base.raw
        return CType(
            f'::jxx::Ptr<::jxx::JxxArray<{elem}, {dims}U>>',
            primitive=True)

    @staticmethod
    def detect_main_entries(source: str) -> Dict[str,str]:
        """Return {containing_class: parameter_name} for source entry methods."""
        declaration=re.compile(r'\bclass\s+([A-Za-z_$][\w$]*)[^{}]*\{')
        main_signature=re.compile(
            r'\bpublic\s+(?:(?:final|synchronized|strictfp)\s+)*static\s+'
            r'(?:(?:final|synchronized|strictfp)\s+)*void\s+main\s*\('
            r'\s*String\s*(?:'
            r'\[\s*\]\s*(?P<before>[A-Za-z_$][\w$]*)|'
            r'(?P<after>[A-Za-z_$][\w$]*)\s*\[\s*\]|'
            r'\.\.\.\s*(?P<varargs>[A-Za-z_$][\w$]*))'
            r'\s*\)')
        entries:Dict[str,str]={}
        for match in declaration.finditer(source):
            depth=1
            index=match.end()
            while index < len(source) and depth:
                if source[index]=='{': depth+=1
                elif source[index]=='}': depth-=1
                index+=1
            body=source[match.end():max(match.end(),index-1)]
            entry=main_signature.search(body)
            if entry:
                entries[match.group(1)]=(entry.group('before') or
                                         entry.group('after') or
                                         entry.group('varargs'))
        return entries

    @staticmethod
    def detect_main_owners(source: str) -> Set[str]:
        return set(Translator.detect_main_entries(source))

    def gather(self,tree):
        self.package=tree.package.name if tree.package else ''
        # Collect every type declared by this source, including member/local
        # classes, before wildcard imports are used to resolve references.
        # These names live in the generated source/header and must never create
        # SDK includes such as util/jxx.util.node.h.
        self.known_types.update(t.name for t in tree.types or [])
        for declaration in self.class_declarations(tree):
            self.known_types.add(declaration.name)
            class_parameters=self.type_parameter_names(declaration)
            self.declared_type_parameters.update(class_parameters)
            if class_parameters:
                self.generic_class_parameters[declaration.name]=class_parameters
            source_base=getattr(declaration,'extends',None)
            if source_base is not None:
                self.source_class_bases[declaration.name]=self.name_of(source_base)
            fields=self.source_field_types.setdefault(declaration.name,{})
            for member in getattr(declaration,'body',None) or []:
                if type(member).__name__=='FieldDeclaration':
                    for declarator in member.declarators or []:
                        fields[declarator.name]=member.type
            constructor_positions={}
            for member in getattr(declaration, 'body', None) or []:
                self.declared_type_parameters.update(
                    self.type_parameter_names(member))
                if type(member).__name__=='ConstructorDeclaration':
                    self.source_constructor_parameter_types[declaration.name]=[
                        parameter.type for parameter in member.parameters or []]
                    for index,parameter in enumerate(member.parameters or []):
                        parameter_name=self.name_of(parameter.type)
                        if parameter_name in class_parameters:
                            constructor_positions[index]=parameter_name
            if constructor_positions:
                self.generic_constructor_parameters[declaration.name]=constructor_positions
        for imp in tree.imports or []:
            p=imp.path
            if p.startswith('java.lang.reflect.') and p != 'java.lang.reflect.InvocationTargetException':
                self.diag.append(f'unsupported reflection type: {p}'); continue
            if any(p==x or p.startswith(x+'.') for x in UNSUPPORTED):
                self.diag.append(f'unsupported package: {p}'); continue
            if imp.wildcard:
                self.wildcard_imports.append(p)
                continue
            self.imports[p.split('.')[-1]]=p
            _,inc=self.qmap(p)
            if inc: self.includes.add(inc)
        self.includes.update({
            'lang/jxx.lang.Object.h',
            'lang/jxx.lang.System.h',
            'lang/jxx.lang.JxxArray.h',
            'lang/jxx.lang.ClassInfo.h',
            'lang/jxx.lang.GenericConversion.h',
        })
        if any(self.is_java_main(m,t.name)
               for t in self.class_declarations(tree)
               for m in getattr(t,'body',None) or []):
            self.includes.update({'lang/jxx.lang.System.h','lang/jxx.lang.String.h','lang/jxx.lang.Throwable.h'})

    def record_method_result_dependency(self, invocation):
        result_type=METHOD_RESULT_TYPES.get(getattr(invocation,'member',None))
        if not result_type:
            return
        _,include=self.qmap(result_type)
        if include:
            self.includes.add(include)

    def gather_body_type_includes(self, tree):
        """Resolve body-level declared and constructed types before header output."""
        filter_method=getattr(tree,'filter',None)
        tree_module=getattr(javalang,'tree',None)
        if callable(filter_method) and tree_module is not None:
            local_type=getattr(tree_module,'LocalVariableDeclaration',None)
            creator_type=getattr(tree_module,'ClassCreator',None)
            catch_type=getattr(tree_module,'CatchClauseParameter',None)
            invocation_type=getattr(tree_module,'MethodInvocation',None)
            if invocation_type is not None:
                for _,node in filter_method(invocation_type):
                    self.record_method_result_dependency(node)
            if local_type is not None:
                for _,node in filter_method(local_type):
                    self.resolve(node.type)
            if creator_type is not None:
                for _,node in filter_method(creator_type):
                    self.ref_raw(node.type)
            if catch_type is not None:
                for _,node in filter_method(catch_type):
                    for catch_name in getattr(node,'types',None) or []:
                        _,include=self.qmap(catch_name)
                        if include:self.includes.add(include)

    def is_java_main(self, m, owner_name: Optional[str] = None) -> bool:
        if type(m).__name__ != 'MethodDeclaration' or m.name != 'main':
            return False
        if owner_name in self.main_owner_names:
            return True
        mods=set(getattr(m,'modifiers',None) or [])
        if not {'public','static'}.issubset(mods): return False
        if self.name_of(getattr(m,'return_type',None)) != 'void': return False
        params=list(getattr(m,'parameters',None) or [])
        if len(params) != 1: return False
        param=params[0]
        if self.simple(self.name_of(param.type)) != 'String': return False
        dims=(len(getattr(param.type,'dimensions',None) or []) +
              len(getattr(param,'dimensions',None) or []))
        if getattr(param,'varargs',False) or dims == 1:
            return True
        # javalang does not consistently retain post-identifier array
        # declarators in formal parameters. In particular, `String args []`
        # can be exposed as a String parameter with no dimensions. A public
        # static void main with exactly one String parameter is treated as the
        # source entry point so all three accepted spellings generate a native
        # wrapper: String[] args, String args[], and String... args.
        return dims == 0

    def java_main_arg_type(self) -> str:
        return (
            "::jxx::Ptr<::jxx::JxxArray<"
            "::jxx::Ptr<::jxx::lang::String>, 1U>>"
        )
    @staticmethod
    def class_declarations(tree):
        # javalang exposes every package-level declaration through tree.types,
        # including package-private classes whose name differs from the source
        # filename. Prefer filter() when available so member classes are also
        # discoverable, then fall back to tree.types for test doubles.
        filter_method=getattr(tree,'filter',None)
        class_type=getattr(getattr(javalang,'tree',None),'ClassDeclaration',None)
        if callable(filter_method) and class_type is not None:
            return [node for _,node in filter_method(class_type)]
        return [node for node in getattr(tree,'types',None) or []
                if type(node).__name__=='ClassDeclaration']


    def sig(self,m,cname,definition=False):
        ctor=type(m).__name__=='ConstructorDeclaration'
        if self.is_java_main(m,cname):
            qualified = f'{cname}::jxxMain' if definition else 'jxxMain'
            parameter_name = m.parameters[0].name
            return f'void {qualified}(const {self.java_main_arg_type()}& {parameter_name})'
        ps=[]
        for p in m.parameters or []:
            extra=(1 if getattr(p,'varargs',False) else 0)+len(getattr(p,'dimensions',None) or [])
            parameter_type=self.resolve(p.type,extra)
            if self.name_of(p.type) in self.type_parameters and extra == 0:
                rendered=f'const {self.name_of(p.type)}&'
            else:
                rendered=parameter_type.param()
            ps.append(f'{rendered} {p.name}')
        qualified=f'{cname}::{m.name}' if definition else m.name
        if ctor: qualified=f'{cname}::{cname}' if definition else cname; return f'{qualified}({", ".join(ps)})'
        ret=self.resolve(m.return_type,len(getattr(m,'dimensions',None) or [])).value()
        return f'{ret} {qualified}({", ".join(ps)})'

    def apply_expression_selectors(self, base, selectors):
        result=base
        for selector in selectors or []:
            kind=type(selector).__name__
            if kind=='MethodInvocation':
                arguments=', '.join(self.expr(x)
                                    for x in selector.arguments or [])
                result+=f'->{selector.member}({arguments})'
            elif kind=='MemberReference':
                result+=f'->{selector.member}'
            elif kind=='ArraySelector':
                # JxxArray values are held through ::jxx::Ptr. Dereference the
                # array object before invoking its rank-aware operator().
                result=f'(*{result})({self.expr(selector.index)})'
            else:
                result+=str(selector)
        return result

    @staticmethod
    def is_string_literal_expression(expression):
        return (type(expression).__name__=='Literal' and
                isinstance(getattr(expression,'value',None),str) and
                getattr(expression,'value').startswith('"'))

    def is_string_concatenation(self, expression):
        if self.is_string_literal_expression(expression):
            return True
        return (type(expression).__name__=='BinaryOperation' and
                getattr(expression,'operator',None)=='+' and
                (self.is_string_concatenation(expression.operandl) or
                 self.is_string_concatenation(expression.operandr)))

    def string_value_expression(self, expression):
        if self.is_string_literal_expression(expression):
            return f'::jxx::NEW<::jxx::lang::String>({self.expr(expression)})'
        return f'::jxx::lang::String::valueOf({self.expr(expression)})'

    def string_concatenation_expression(self, expression):
        left=expression.operandl
        right=expression.operandr
        if self.is_string_concatenation(left):
            left_value=(self.string_concatenation_expression(left)
                        if type(left).__name__=='BinaryOperation'
                        else self.string_value_expression(left))
            return f'{left_value}->concat({self.string_value_expression(right)})'
        # A String on the right still makes the entire + expression a String.
        return (f'{self.string_value_expression(left)}->concat('
                f'{self.string_value_expression(right)})')

    def lambda_parameter_name(self, parameter):
        return (getattr(parameter,'name',None) or
                getattr(parameter,'member',None) or
                str(parameter))

    def lambda_expression(self, expression):
        parameters=', '.join(
            f'auto {self.lambda_parameter_name(parameter)}'
            for parameter in getattr(expression,'parameters',None) or [])
        body=getattr(expression,'body',None)
        output=Emit()
        if isinstance(body,list):
            for statement in body:
                self.stmt(statement,output)
        elif body is not None:
            # A source expression lambda used by forEach is statement-compatible.
            # Emit a block and discard any fluent return value such as append().
            output.w(self.expr(body)+';')
        lines=output.text().rstrip().splitlines()
        if not lines:
            return f'[&]({parameters}) {{}}'
        rendered='\n'.join('    '+line for line in lines)
        return f'[&]({parameters}) {{\n{rendered}\n}}'

    def source_field_type(self,class_name,field_name):
        visited=set()
        current=self.simple(class_name)
        while current and current not in visited:
            visited.add(current)
            field_type=self.source_field_types.get(current,{}).get(field_name)
            if field_type is not None:
                return field_type
            current=self.simple(self.source_class_bases.get(current,''))
        return None

    def assignment_target_type(self,target):
        if type(target).__name__!='MemberReference':
            return None
        qualifier=getattr(target,'qualifier',None)
        if not qualifier:
            if target.member in self.local_variable_types:
                return self.local_variable_types[target.member]
            return self.source_field_type(self.current_class,target.member)
        if qualifier in {'this','super'}:
            owner=(self.current_class if qualifier=='this' else
                   self.source_class_bases.get(self.current_class,''))
        else:
            owner_type=self.local_variable_types.get(qualifier)
            owner=self.name_of(owner_type) if owner_type is not None else ''
        return self.source_field_type(owner,target.member) if owner else None

    def assignment_value_expression(self,target,value):
        target_type=self.assignment_target_type(target)
        if target_type is not None and self.is_string_literal_expression(value):
            return self.constructor_argument_expression(value,target_type)
        return self.expr(value)

    def declaration_initializer_expression(self,target_type,initializer):
        if self.is_string_literal_expression(initializer):
            return self.constructor_argument_expression(initializer,target_type)
        return self.expr(initializer)

    def begin_member_scope(self,owner_name,member):
        self.current_class=owner_name
        self.local_variable_types={}
        for parameter in getattr(member,'parameters',None) or []:
            self.local_variable_types[parameter.name]=parameter.type

    def expr(self,e):
        if e is None:return ''
        n=type(e).__name__
        if n=='Literal': return 'nullptr' if e.value in (None,'null') else e.value
        if n=='LambdaExpression': return self.lambda_expression(e)
        if n=='This':
            base='this'
            for selector in getattr(e,'selectors',None) or []:
                kind=type(selector).__name__
                if kind=='MemberReference':
                    base+=f'->{selector.member}'
                elif kind=='MethodInvocation':
                    args=', '.join(self.expr(x) for x in selector.arguments or [])
                    base+=f'->{selector.member}({args})'
                elif kind=='ArraySelector':
                    base+=f'({self.expr(selector.index)})'
            return base
        if n=='SuperMemberReference':
            base=f'this->JxxSuper::{e.member}'
            base=self.apply_expression_selectors(
                base,getattr(e,'selectors',None))
            return (''.join(getattr(e,'prefix_operators',None) or [])+
                    base+
                    ''.join(getattr(e,'postfix_operators',None) or []))
        if n=='SuperMethodInvocation':
            arguments=', '.join(self.expr(x) for x in e.arguments or [])
            invocation=f'JxxSuper::{e.member}({arguments})'
            return self.apply_expression_selectors(
                invocation,getattr(e,'selectors',None))
        if n=='MemberReference':
            q=getattr(e,'qualifier',None)
            if q:
                fq=self.imports.get(q,q)
                if q in self.local_variable_types:
                    base=f'{q}->{e.member}'
                elif e.member in STATIC_ACCESSORS.get(fq,set()):
                    base=f'{self.qmap(fq)[0]}::{e.member}()'
                elif q in self.imports or (q and q[0].isupper()):
                    base=f'{self.qualified_static_owner(q)}::{e.member}'
                else:
                    base=f'{q}->{e.member}'
            else: base=e.member
            base=self.apply_expression_selectors(
                base,getattr(e,'selectors',None))
            return ''.join(getattr(e,'prefix_operators',None) or [])+base+''.join(getattr(e,'postfix_operators',None) or [])
        if n=='MethodInvocation':
            q=getattr(e,'qualifier',None)
            if q in self.local_variable_types:
                head=f'{q}->{e.member}'
            elif q and (q in self.imports or q[0].isupper()):
                owner=self.qualified_static_owner(q)
                # Class.method is static and uses ::. Class.field.method calls
                # through the field object and must use ->.
                access='->' if '.' in q else '::'
                head=f'{owner}{access}{e.member}'
            else:
                head=f'{q}->{e.member}' if q else e.member
            invocation=f'{head}({", ".join(self.expr(x) for x in e.arguments or [])})'
            return self.apply_expression_selectors(
                invocation,getattr(e,'selectors',None))
        if n=='ClassCreator': return self.class_creator_expression(e)
        if n=='ArrayCreator':
            rank=len(getattr(e.type,'dimensions',None) or []) + len(getattr(e,'dimensions',None) or [])
            rank=max(rank,1); elem=self.resolve(e.type).value()
            dims=[self.expr(x) for x in getattr(e,'dimensions',None) or [] if x is not None]
            return f'::jxx::NEW<::jxx::JxxArray<{elem}, {rank}U>>({", ".join(dims)})'
        if n=='Cast':
            t=self.resolve(e.type); x=self.expr(e.expression)
            return f'static_cast<{t.raw}>({x})' if t.primitive else f'::jxx::CAST<{t.raw}>({x})'
        if n=='BinaryOperation':
            if e.operator=='instanceof': return f'::jxx::INSTANCEOF<{self.ref_raw(e.operandr)}>({self.expr(e.operandl)})'
            if e.operator=='+' and self.is_string_concatenation(e):
                return self.string_concatenation_expression(e)
            return f'({self.expr(e.operandl)} {e.operator} {self.expr(e.operandr)})'
        if n=='Assignment':
            value=self.assignment_value_expression(e.expressionl,e.value)
            return f'{self.expr(e.expressionl)} {e.type or "="} {value}'
        if n=='TernaryExpression': return f'({self.expr(e.condition)} ? {self.expr(e.if_true)} : {self.expr(e.if_false)})'
        if n=='ArraySelector': return f'({self.expr(e.index)})'
        return str(e)

    def array_initializer_value(self, element_type, expression):
        value=self.expr(expression)
        name=self.name_of(element_type)
        if name in {'Integer','Character','Boolean','Byte','Short','Long','Float','Double'}:
            raw=self.qmap(name)[0]
            return f'::jxx::NEW<{raw}>({value})'
        return value

    def emit_local_array_initializer(self, declaration, declarator, o, dims):
        array_type=self.resolve(declaration.type,dims).value()
        element=self.resolve(declaration.type).value()
        values=list(getattr(declarator.initializer,'initializers',None) or [])
        o.w(f'{array_type} {declarator.name} = ::jxx::NEW<::jxx::JxxArray<{element}, {dims}U>>({len(values)}U);')
        for index,value in enumerate(values):
            converted=self.array_initializer_value(declaration.type,value)
            o.w(f'(*{declarator.name})({index}U) = {converted};')

    def class_creator_expression(self, creator, expected_type=None):
        allocation_type=creator.type
        creator_arguments=getattr(creator.type,'arguments',None) or []
        expected_arguments=(getattr(expected_type,'arguments',None) or []
                            if expected_type is not None else [])
        if expected_type is not None and not creator_arguments and expected_arguments:
            allocation_type=expected_type
        raw=self.ref_raw(allocation_type)
        allocation_arguments=getattr(allocation_type,'arguments',None) or []
        resolved_type_arguments=[]
        for argument in allocation_arguments:
            argument_type=getattr(argument,'type',None)
            resolved_type_arguments.append(
                self.resolve(argument_type).value() if argument_type is not None
                else '::jxx::Ptr<::jxx::lang::Object>')
        class_name=self.name_of(allocation_type)
        class_parameters=self.generic_class_parameters.get(class_name,[])
        constructor_positions=self.generic_constructor_parameters.get(class_name,{})
        constructor_parameter_types=self.source_constructor_parameter_types.get(
            class_name,[])
        converted_arguments=[]
        for index,argument in enumerate(creator.arguments or []):
            value=self.expr(argument)
            parameter_name=constructor_positions.get(index)
            if parameter_name in class_parameters:
                target_index=class_parameters.index(parameter_name)
                if target_index < len(resolved_type_arguments):
                    target=resolved_type_arguments[target_index]
                    value=f'::jxx::CONVERT<{target}>({value})'
            elif index < len(constructor_parameter_types):
                # Source constructors with concrete reference parameters still
                # require JXX conversion. For example, const char[] must become
                # Ptr<String> before it reaches jxx::NEW<T>.
                value=self.constructor_argument_expression(
                    argument,constructor_parameter_types[index])
            converted_arguments.append(value)
        return f'::jxx::NEW<{raw}>({", ".join(converted_arguments)})'

    def stmt(self,s,o):
        if s is None:return
        n=type(s).__name__
        if n=='BlockStatement':
            for x in getattr(s,'statements',None) or []: self.stmt(x,o)
        elif n=='ReturnStatement': o.w('return;' if s.expression is None else f'return {self.expr(s.expression)};')
        elif n=='StatementExpression': o.w(self.expr(s.expression)+';')
        elif n=='ThrowStatement':
            x=s.expression
            if type(x).__name__=='ClassCreator': o.w(f'throw {self.ref_raw(x.type)}({", ".join(self.expr(a) for a in x.arguments or [])});')
            else:o.w(f'throw {self.expr(x)};')
        elif n=='LocalVariableDeclaration':
            for d in s.declarators:
                dims=len(getattr(d,'dimensions',None) or [])
                if type(getattr(d,'initializer',None)).__name__=='ArrayInitializer':
                    self.emit_local_array_initializer(s,d,o,max(dims,1))
                    continue
                t=self.resolve(s.type,dims)
                self.local_variable_types[d.name]=s.type
                if type(getattr(d,'initializer',None)).__name__=='ClassCreator':
                    initial_value=self.class_creator_expression(d.initializer,s.type)
                else:
                    initial_value=(self.declaration_initializer_expression(
                        s.type,d.initializer) if d.initializer is not None else '')
                init=f' = {initial_value}' if d.initializer is not None else ''
                const='const ' if 'final' in (s.modifiers or set()) else ''
                o.w(f'{const}{t.value()} {d.name}{init};')
        elif n=='IfStatement':
            o.w(f'if ({self.expr(s.condition)}) {{');o.enter();self.stmt(s.then_statement,o);o.exit()
            if s.else_statement is not None:o.w('} else {');o.enter();self.stmt(s.else_statement,o);o.exit()
            o.w('}')
        elif n=='WhileStatement':
            o.w(f'while ({self.expr(s.condition)}) {{');o.enter();self.stmt(s.body,o);o.exit();o.w('}')
        elif n=='SynchronizedStatement':
            lock=self.expr(getattr(s,'lock',None) or getattr(s,'expression',None)); body=getattr(s,'block',None) or []
            o.w(f'{lock}->synchronized([&]() {{');o.enter()
            for x in body:self.stmt(x,o)
            o.exit();o.w('});')
        elif n=='BreakStatement':o.w('break;')
        elif n=='ContinueStatement':o.w('continue;')
        else:o.w(f'/* TODO unsupported statement: {n} */')

    @staticmethod
    def type_parameter_names(declaration)->List[str]:
        return [Translator.name_of(parameter)
                for parameter in getattr(declaration, 'type_parameters', None) or []]

    @staticmethod
    def template_line(names:Sequence[str])->str:
        return ('template <' + ', '.join(f'typename {name}' for name in names) + '>') if names else ''

    @staticmethod
    def specialized_class_name(name:str, type_parameters:Sequence[str])->str:
        if not type_parameters:
            return name
        return f'{name}<' + ', '.join(type_parameters) + '>'

    def generic_conversion_parameters(self, method):
        converted=[]
        for index,parameter in enumerate(getattr(method,'parameters',None) or []):
            dimensions=(len(getattr(parameter,'dimensions',None) or []) +
                        (1 if getattr(parameter,'varargs',False) else 0) +
                        len(getattr(parameter.type,'dimensions',None) or []))
            target=self.name_of(parameter.type)
            if target in self.type_parameters and dimensions == 0:
                converted.append((index,parameter,target))
        return converted

    def emit_generic_conversion_overload(self, method, owner_name, o):
        kind=type(method).__name__
        if kind not in ('MethodDeclaration','ConstructorDeclaration'):
            return
        converted=self.generic_conversion_parameters(method)
        if not converted:
            return
        source_names=['Source' if len(converted)==1 else f'Source{position}'
                      for position in range(len(converted))]
        bindings={index:(parameter,target,source_names[position])
                  for position,(index,parameter,target) in enumerate(converted)}
        constraints=[]
        for _,(_,target,source) in bindings.items():
            constraints.extend([
                f'::jxx::is_generic_convertible_v<{target}, {source}&&>',
                f'!std::is_same_v<std::decay_t<{source}>, {target}>'])
        parameters=[];arguments=[]
        for index,parameter in enumerate(method.parameters or []):
            if index in bindings:
                _,target,source=bindings[index]
                parameters.append(f'{source}&& {parameter.name}')
                arguments.append(
                    f'::jxx::CONVERT<{target}>('
                    f'std::forward<{source}>({parameter.name}))')
            else:
                extra=(1 if getattr(parameter,'varargs',False) else 0)+len(
                    getattr(parameter,'dimensions',None) or [])
                parameters.append(
                    f'{self.resolve(parameter.type,extra).param()} '
                    f'{parameter.name}')
                arguments.append(parameter.name)
        condition=' && '.join(constraints)
        if kind=='ConstructorDeclaration':
            # Constructors cannot use return-type SFINAE. Keep the condition in
            # a defaulted type template parameter instead of a function pointer
            # parameter. This form is accepted consistently by MSVC C++17.
            template_parameters=[f'typename {source}' for source in source_names]
            template_parameters.append(
                f'typename = std::enable_if_t<{condition}>')
            o.w('template <' + ', '.join(template_parameters) + '>')
            o.w(f'{owner_name}({", ".join(parameters)})')
            o.enter();o.w(f': {owner_name}({", ".join(arguments)}) {{}}');o.exit()
            return
        o.w('template <' + ', '.join(
            f'typename {source}' for source in source_names) + '>')
        prefix='static ' if 'static' in (
            getattr(method,'modifiers',None) or set()) else ''
        return_type=self.resolve(
            method.return_type,
            len(getattr(method,'dimensions',None) or [])).value()
        enabled_return=f'std::enable_if_t<{condition}, {return_type}>'
        o.w(f'{prefix}{enabled_return} {method.name}('
            f'{", ".join(parameters)}) {{');o.enter()
        invocation=f'{method.name}({", ".join(arguments)})'
        o.w(f'{invocation};' if return_type=='void'
            else f'return {invocation};')
        o.exit();o.w('}')

    def emit_inline_member(self, member, owner_name, o):
        kind=type(member).__name__
        is_constructor=kind=='ConstructorDeclaration'
        modifiers=getattr(member,'modifiers',None) or set()
        prefix='static ' if kind=='MethodDeclaration' and 'static' in modifiers else ''
        body=getattr(member,'body',None)
        if body is None:
            virt='virtual ' if kind=='MethodDeclaration' and 'abstract' in modifiers else ''
            pure=' = 0' if virt else ''
            o.w(f'{virt}{prefix}{self.sig(member,owner_name)}{pure};')
            return
        initializer,statements=(self.constructor_initializer(member,owner_name)
                                if is_constructor else ('',list(body or [])))
        o.w(f'{prefix}{self.sig(member,owner_name)}{initializer} {{');o.enter()
        self.begin_member_scope(owner_name,member)
        old=self.current_return
        self.current_return='void' if is_constructor else self.resolve(
            member.return_type,len(getattr(member,'dimensions',None) or [])).value()
        for statement in statements:
            self.stmt(statement,o)
        self.current_return=old
        o.exit();o.w('}')
        self.emit_generic_conversion_overload(member,owner_name,o)

    def emit_nested_class(self, declaration, o):
        names=self.type_parameter_names(declaration)
        previous=set(self.type_parameters)
        self.type_parameters.update(names)
        template=self.template_line(names)
        if template: o.w(template)
        ext=getattr(declaration,'extends',None)
        base=self.ref_raw(ext) if ext is not None else '::jxx::lang::Object'
        impls=getattr(declaration,'implements',None) or []
        iface_types=[self.ref_raw(i) for i in impls]
        self_type=self.specialized_class_name(declaration.name,names)
        class_args=', '.join([self_type, base] + iface_types)
        o.w(f'class {declaration.name} : public ::jxx::lang::ClassBase<{class_args}>')
        o.w('{')
        # Preserve the source member-class access. It must be declared before
        # outer fields whose types refer to it.
        access='private' if 'private' in (getattr(declaration,'modifiers',None) or set()) else 'public'
        o.w(f'{access}:'); o.enter()
        o.w(f'using JxxSuper = {base};')
        o.w(f'using Super = ::jxx::lang::ClassBase<{class_args}>;')
        for member in getattr(declaration,'body',None) or []:
            kind=type(member).__name__
            if kind=='FieldDeclaration':
                for d in member.declarators:
                    typ=self.resolve(member.type).value()
                    init=' = nullptr' if typ.startswith('::jxx::Ptr<') else ''
                    o.w(f'{typ} {d.name}{init};')
            elif kind in ('MethodDeclaration','ConstructorDeclaration'):
                self.emit_inline_member(member,declaration.name,o)
        o.exit(); o.w('};')
        self.type_parameters=previous

    def method_type_parameter_names(self, method)->List[str]:
        return self.type_parameter_names(method)

    def emit_template_method(self, method, owner_name, o):
        names=self.method_type_parameter_names(method)
        previous=set(self.type_parameters)
        self.type_parameters.update(names)
        template=self.template_line(names)
        if template:
            o.w(template)
        modifiers=getattr(method,'modifiers',None) or set()
        prefix='static ' if 'static' in modifiers else ''
        if getattr(method,'body',None) is None:
            o.w(f'{prefix}{self.sig(method,owner_name)};')
        else:
            o.w(f'{prefix}{self.sig(method,owner_name)} {{');o.enter()
            self.begin_member_scope(owner_name,method)
            old=self.current_return
            self.current_return=self.resolve(method.return_type,len(getattr(method,'dimensions',None) or [])).value()
            for statement in method.body or []:
                self.stmt(statement,o)
            self.current_return=old;o.exit();o.w('}')
        self.type_parameters=previous

    def header(self,tree)->str:
        o=Emit();o.w('#pragma once')
        o.w('#include <type_traits>');o.w('#include <utility>')
        for x in sorted(self.includes):o.w(f'#include "{x}"')
        o.w(); ns=('jxx::'+self.package.replace('.','::')) if self.package else ''
        if ns:o.w(f'namespace {ns} {{');o.enter()
        for t in tree.types or []:
            outer_parameters=self.type_parameter_names(t)
            self.type_parameters=set(outer_parameters)
            template=self.template_line(outer_parameters)
            if template:o.w(template)
            iface=type(t).__name__=='InterfaceDeclaration'; self.current_class=t.name
            if iface:
                o.w(f'class {t.name} : public ::jxx::lang::InterfaceBase<{t.name}> {{');o.w('public:');o.enter();o.w(f'virtual ~{t.name}() = default;')
                for m in t.body or []:
                    if type(m).__name__=='MethodDeclaration':o.w(f'virtual {self.sig(m,t.name)} = 0;')
                o.exit();o.w('};');o.w();continue
            ext=getattr(t,'extends',None); base=self.ref_raw(ext) if ext is not None else '::jxx::lang::Object'
            impls=getattr(t,'implements',None) or []
            iface_types=[self.ref_raw(i) for i in impls]
            self_type=self.specialized_class_name(t.name,outer_parameters)
            class_args=', '.join([self_type, base] + iface_types)
            o.w(f'class {t.name} : public ::jxx::lang::ClassBase<{class_args}>')
            o.w('{');o.w('public:');o.enter();o.w(f'using JxxSuper = {base};')
            o.w(f'using Super = ::jxx::lang::ClassBase<{class_args}>;')
            o.w(f'using JxxClassInfoMarker = ::jxx::lang::ClassInfo<{class_args}>;')
            # Emit member classes before fields and methods so types such as
            # node<T> are complete and visible at their first use.
            for nested in t.body or []:
                if type(nested).__name__=='ClassDeclaration':
                    self.emit_nested_class(nested,o)
            for m in t.body or []:
                n=type(m).__name__
                if n=='FieldDeclaration':
                    mods=m.modifiers or set(); prefix='inline static ' if 'static' in mods else ''
                    for d in m.declarators:
                        typ=self.resolve(m.type).value()
                        init=(f' = {self.declaration_initializer_expression(m.type,d.initializer)}'
                              if d.initializer is not None else
                              (' = nullptr' if typ.startswith('::jxx::Ptr<') else ''))
                        o.w(f'{prefix}{typ} {d.name}{init};')
                elif n in ('MethodDeclaration','ConstructorDeclaration'):
                    if outer_parameters:
                        if n=='MethodDeclaration' and self.method_type_parameter_names(m):
                            self.emit_template_method(m,t.name,o)
                        else:
                            self.emit_inline_member(m,t.name,o)
                        continue
                    if n=='MethodDeclaration' and self.method_type_parameter_names(m):
                        # Function templates must be defined in the header.
                        self.emit_template_method(m,t.name,o)
                        continue
                    if self.is_java_main(m,t.name):
                        o.w(f'static {self.sig(m,t.name)};')
                        continue
                    mods=m.modifiers or set()
                    if n=='MethodDeclaration' and 'static' in mods:
                        o.w(f'static {self.sig(m,t.name)};')
                    else:
                        virt='virtual ' if n=='MethodDeclaration' and ('abstract' in mods or m.body is None) else ''
                        pure=' = 0' if virt else ''
                        override=' override' if n=='MethodDeclaration' and 'override' in getattr(m,'annotations',[]) else ''
                        o.w(f'{virt}{self.sig(m,t.name)}{override}{pure};')
            o.exit();o.w('};');o.w()
            self.type_parameters=set()
        if ns:o.exit();o.w(f'}} // namespace {ns}')
        return o.text()

    def constructor_argument_expression(self, expression, target_type):
        target_name=self.name_of(target_type)
        value=self.expr(expression)
        if target_name in PRIMITIVES:
            return value
        target=self.resolve(target_type).value()
        return f'::jxx::CONVERT<{target}>({value})'

    def source_constructor_arguments(self, class_name, arguments):
        parameter_types=self.source_constructor_parameter_types.get(class_name,[])
        converted=[]
        for index,argument in enumerate(arguments or []):
            if index < len(parameter_types):
                converted.append(self.constructor_argument_expression(
                    argument,parameter_types[index]))
            else:
                converted.append(self.expr(argument))
        return converted

    def constructor_initializer(self, constructor, class_name):
        body=list(getattr(constructor,'body',None) or [])
        if not body:
            return '',body
        first=body[0]
        invocation=first
        if type(first).__name__=='StatementExpression':
            invocation=getattr(first,'expression',None)
        kind=type(invocation).__name__
        if kind=='ExplicitConstructorInvocation':
            args=', '.join(self.source_constructor_arguments(
                class_name,invocation.arguments or []))
            return f' : {class_name}({args})',body[1:]
        if kind=='SuperConstructorInvocation':
            base_name=self.source_class_bases.get(class_name,'')
            args=', '.join(self.source_constructor_arguments(
                base_name,invocation.arguments or []))
            # The converted class directly inherits Super, the ClassBase CRTP
            # specialization. JxxSuper names the source superclass carried by
            # ClassBase and is not a direct C++ base, so it cannot appear in a
            # derived constructor's mem-initializer list.
            return f' : Super({args})',body[1:]
        return '',body

    def source(self,tree,header_inc)->str:
        o=Emit();o.w(f'#include "{header_inc}"');o.w();ns=('jxx::'+self.package.replace('.', '::')) if self.package else ''
        if ns:o.w(f'namespace {ns} {{');o.enter()
        for t in self.class_declarations(tree):
            if self.type_parameter_names(t):
                continue
            for m in t.body or []:
                if type(m).__name__ not in ('MethodDeclaration','ConstructorDeclaration') or getattr(m,'body',None) is None:continue
                if type(m).__name__=='MethodDeclaration' and self.method_type_parameter_names(m):
                    continue
                is_constructor=type(m).__name__=='ConstructorDeclaration'
                initializer,statements=(
                    self.constructor_initializer(m,t.name)
                    if is_constructor else ('',list(m.body or [])))
                o.w(self.sig(m,t.name,True)+initializer+' {');o.enter()
                self.begin_member_scope(t.name,m)
                old=self.current_return
                self.current_return='void' if is_constructor else self.resolve(m.return_type).value()
                for statement in statements:self.stmt(statement,o)
                self.current_return=old;o.exit();o.w('}');o.w()
        if ns:o.exit();o.w(f'}} // namespace {ns}')
        return o.text()

    def main_source(self, owner_name, parameter_name, header_inc,
                    owner_type_parameters:Sequence[str]=())->str:
        o=Emit()
        o.w(f'#include "{header_inc}"')
        o.w('#include <cstdint>')
        o.w('#include "jxx.initialize.h"')
        o.w('#include "lang/jxx.lang.String.h"')
        o.w('#include "lang/jxx.lang.Throwable.h"')
        o.w('#include "lang/jxx.lang.System.h"')
        o.w()
        ns=('jxx::'+self.package.replace('.', '::')) if self.package else ''
        owner_specialization=owner_name
        if owner_type_parameters:
            default_type='::jxx::Ptr<::jxx::lang::Object>'
            owner_specialization=(owner_name + '<' +
                ', '.join(default_type for _ in owner_type_parameters) + '>')
        target=(f'::{ns}::{owner_specialization}'
                if ns else owner_specialization)
        o.w('// Native C++ process entry point for this converted source main.')
        o.w('int main(int argc, char** argv) {')
        o.enter();o.w('try {');o.enter()
        o.w('::jxx::initialize();')
        o.w('using JxxString = ::jxx::lang::String;')
        o.w('using JxxStringArray = ::jxx::JxxArray<::jxx::Ptr<JxxString>, 1U>;')
        o.w('const auto sourceArgc = argc > 0 ? argc - 1 : 0;')
        o.w(f'auto {parameter_name} = ::jxx::NEW<JxxStringArray>(')
        o.enter();o.w('static_cast<std::uint32_t>(sourceArgc));');o.exit()
        o.w('for (int index = 1; index < argc; ++index) {');o.enter()
        o.w(f'(*{parameter_name})(static_cast<std::uint32_t>(index - 1)) =')
        o.enter();o.w('::jxx::NEW<JxxString>(argv[index]);');o.exit()
        o.exit();o.w('}')
        o.w(f'{target}::jxxMain({parameter_name});')
        o.w('return 0;');o.exit()
        o.w('} catch (const ::jxx::lang::Throwable& throwable) {');o.enter()
        o.w('throwable.printStackTrace();');o.w('return 1;');o.exit();o.w('}')
        o.exit();o.w('}')
        return o.text()

    def translate(self,src,out_root,stem,source_relative_dir:Optional[Path]=None):
        self.main_entries=self.detect_main_entries(src)
        self.main_owner_names=set(self.main_entries)
        tree=javalang.parse.parse(src);self.gather(tree)
        # Header includes must be complete before header() is emitted. Body
        # translation can discover types such as Scanner, but that is too late.
        self.gather_body_type_includes(tree)
        pkg=tree.package.name if tree.package else ''
        # Named packages own their output path. For an unnamed package, retain
        # the source directory relative to the conversion root so classes in
        # folders such as Lab_Practice remain colocated and includeable.
        rel=(Path(*pkg.split('.')) if pkg else
             Path(source_relative_dir or Path()))
        output_dir=Path(out_root)/rel
        # The package directory already provides name separation. Preserve
        # the source filename so Abstraction.java generates Abstraction.h and
        # Abstraction.cpp in the replicated package folder.
        hp=output_dir/f'{stem}.h';cp=output_dir/f'{stem}.cpp'
        output_dir.mkdir(parents=True,exist_ok=True)
        # resolve declarations before includes are emitted
        for t in tree.types or []:
            if getattr(t,'extends',None) is not None:self.ref_raw(t.extends)
            for i in getattr(t,'implements',None) or []:self.ref_raw(i)
            for m in t.body or []:
                if hasattr(m,'type'):self.resolve(m.type)
                if hasattr(m,'return_type') and m.return_type is not None:self.resolve(m.return_type)
                for p in getattr(m,'parameters',None) or []:self.resolve(p.type)
                for thrown in getattr(m,'throws',None) or []:
                    thrown_name=self.name_of(thrown)
                    _,include=self.qmap(thrown_name)
                    if include:self.includes.add(include)
        hp.write_text(self.header(tree),encoding='utf-8')
        header_inc=os.path.relpath(hp,cp.parent).replace(os.sep,'/')
        cp.write_text(self.source(tree,header_inc),encoding='utf-8')
        self.generated_files=[hp,cp]

        # Wrapper generation is driven entirely by the validated source scan.
        # It does not depend on javalang retaining the main method in owner.body.
        parsed_declarations={owner.name:owner
                             for owner in self.class_declarations(tree)}
        parsed_classes=set(parsed_declarations)
        for owner_name,parameter_name in sorted(self.main_entries.items()):
            if owner_name not in parsed_classes:
                self.diag.append(
                    f'generating {owner_name}_main.cpp from source detection; '
                    'the class was not returned by the AST class iterator')
            wrapper=output_dir/f'{owner_name}_main.cpp'
            wrapper.write_text(
                self.main_source(
                    owner_name,parameter_name,header_inc,
                    self.type_parameter_names(parsed_declarations[owner_name])
                    if owner_name in parsed_declarations else ()),
                encoding='utf-8')
            self.generated_files.append(wrapper)
        return hp,cp

def _contains_native_main(source: Path) -> bool:
    try:
        text=source.read_text(encoding='utf-8')
    except OSError:
        return False
    return bool(re.search(
        r'\bint\s+main\s*\(\s*int\s+argc\s*,\s*char\s*(?:\*\*|\*\s*argv\s*\[\s*\])',
        text))

def _safe_target(source: Path, root: Path) -> str:
    raw = '_'.join(source.relative_to(root).with_suffix('').parts)
    name = ''.join(ch if ch.isalnum() or ch == '_' else '_' for ch in raw)
    return ('_' + name) if name and name[0].isdigit() else (name or 'jxx_app')


def normalize_cmake_include_path(jxx_include: str) -> str:
    """Normalize a JXX include path for generated CMake files.

    Native Windows drive paths are preserved without passing through
    pathlib.Path.resolve(), which would incorrectly treat them as relative
    paths when the converter runs under WSL/Linux. WSL /mnt/<drive>/ paths
    are converted to Visual Studio drive paths. Other paths retain the
    existing expanduser/resolve behavior.
    """
    path = os.fspath(jxx_include)

    # A native Windows absolute path must not be resolved by pathlib while
    # running on a POSIX host. Preserve its drive-letter spelling and make
    # separators portable for CMake.
    if re.match(r'^[A-Za-z]:[\\/]', path):
        return path.replace('\\', '/')

    # Visual Studio consumes a drive path rather than WSL's /mnt/<drive>/
    # mount spelling. Use an uppercase drive letter for this conversion.
    wsl_match = re.match(r'^/mnt/([A-Za-z])(?:/(.*))?$', path.replace('\\', '/'))
    if wsl_match:
        drive = wsl_match.group(1).upper()
        remainder = wsl_match.group(2) or ''
        return f'{drive}:/{remainder}' if remainder else f'{drive}:/'

    return Path(path).expanduser().resolve().as_posix()


def generate_cmake_files(out_root: str, project: str = 'TranspiledProject',
                         support_target: str = 'transpiled',
                         jxx_include: Optional[str] = None) -> Path:
    root = Path(out_root)
    jxx_include_path = (
        normalize_cmake_include_path(jxx_include)
        if jxx_include else None)
    sources = sorted(root.rglob('*.cpp'))
    mains = [source for source in sources
             if source.name.endswith('_main.cpp') and _contains_native_main(source)]
    support = [source for source in sources if source not in mains]

    counts = {}
    for source in mains:
        logical_name = source.stem[:-5] if source.stem.endswith('_main') else source.stem
        counts[logical_name] = counts.get(logical_name, 0) + 1
    names = {}
    used = set()
    for source in mains:
        logical_name = source.stem[:-5] if source.stem.endswith('_main') else source.stem
        name = logical_name if counts[logical_name] == 1 else _safe_target(source, root)
        if name.endswith('_main'):
            name = name[:-5]
        while name in used:
            name += '_app'
        used.add(name)
        names[source] = name

    root_lines = [
        'cmake_minimum_required(VERSION 3.16)',
        f'project({project} LANGUAGES CXX)', '',
        'set(CMAKE_CXX_STANDARD 17)',
        'set(CMAKE_CXX_STANDARD_REQUIRED ON)',
        'set(CMAKE_CXX_EXTENSIONS OFF)', ''
    ]
    if support:
        root_lines.append(f'add_library({support_target} STATIC')
        root_lines.extend(f'    {x.relative_to(root).as_posix()}' for x in support)
        root_lines.append(')')
        root_lines.append(f'target_include_directories({support_target} PUBLIC')
        root_lines.append('    ${CMAKE_CURRENT_SOURCE_DIR}')
        if jxx_include_path:
            root_lines.append(f'    "{jxx_include_path}"')
        root_lines += [')', '']

    main_folders = sorted({source.parent for source in mains})
    for folder in main_folders:
        if folder != root:
            root_lines.append(f'add_subdirectory({folder.relative_to(root).as_posix()})')

    for source in [x for x in mains if x.parent == root]:
        target = names[source]
        root_lines += ['', f'add_executable({target} {source.name})',
                       f'target_include_directories({target} PRIVATE',
                       '    ${CMAKE_CURRENT_SOURCE_DIR}']
        if jxx_include_path:
            root_lines.append(f'    "{jxx_include_path}"')
        root_lines.append(')')
        if support:
            root_lines.append(f'target_link_libraries({target} PRIVATE {support_target})')

    if not mains:
        root_lines.append('# No converted source main method was found.')
    root_file = root / 'CMakeLists.txt'
    root_file.write_text('\n'.join(root_lines) + '\n', encoding='utf-8')

    for folder in main_folders:
        if folder == root:
            continue
        lines = ['# Generated executable targets for converted source main methods.', '']
        for source in [x for x in mains if x.parent == folder]:
            target = names[source]
            lines += [f'add_executable({target} {source.name})',
                      f'target_include_directories({target} PRIVATE',
                      '    ${PROJECT_SOURCE_DIR}']
            if jxx_include_path:
                lines.append(f'    "{jxx_include_path}"')
            lines.append(')')
            if support:
                lines.append(f'target_link_libraries({target} PRIVATE {support_target})')
            lines.append('')
        (folder / 'CMakeLists.txt').write_text('\n'.join(lines), encoding='utf-8')
    return root_file


def generate_cmake(out_root: str, project: str = 'TranspiledProject',
                   target: str = 'transpiled',
                   jxx_include: Optional[str] = None) -> str:
    return generate_cmake_files(
        out_root, project, target, jxx_include).read_text(encoding='utf-8')

def build_source_type_index(
        files:Sequence[Path],
        source_root:Optional[Path]=None) -> Dict[str,Tuple[str,str]]:
    """Map source class names to generated qualified types and headers."""
    index:Dict[str,Tuple[str,str]]={}
    package_pattern=re.compile(r'\bpackage\s+([A-Za-z_$][\w$]*(?:\.[A-Za-z_$][\w$]*)*)\s*;')
    class_pattern=re.compile(r'\bclass\s+([A-Za-z_$][\w$]*)')
    for path in files:
        try:
            source=path.read_text(encoding='utf-8')
        except OSError:
            continue
        # Package declarations inside line or block comments are inert.
        # Remove comments before indexing so `// package Lab_Practice;` does
        # not incorrectly create a package namespace or package-derived path.
        index_source=re.sub(r'/\*.*?\*/|//[^\r\n]*', '', source,
                            flags=re.DOTALL)
        package_match=package_pattern.search(index_source)
        package=package_match.group(1) if package_match else ''
        namespace='::jxx' + (('::'+package.replace('.','::')) if package else '')
        if package:
            folder=package.replace('.','/')
        elif source_root is not None:
            try:
                relative_parent=path.parent.resolve().relative_to(
                    source_root.resolve())
                folder=relative_parent.as_posix()
                if folder == '.':
                    folder=''
            except ValueError:
                folder=''
        else:
            folder=''
        include=(f'{folder}/{path.stem}.h' if folder else f'{path.stem}.h')
        for match in class_pattern.finditer(index_source):
            name=match.group(1)
            value=(f'{namespace}::{name}',include)
            qualified_name=f'{package}.{name}' if package else name
            index[qualified_name]=value
            index[name]=value
    return index

def translate_file(path:Path,out_root:str,
                   source_type_index:Optional[Dict[str,Tuple[str,str]]]=None,
                   source_root:Optional[Path]=None):
    src=path.read_text(encoding='utf-8')
    t=Translator()
    t.source_type_index=dict(source_type_index or {})
    source_relative_dir=Path()
    if source_root is not None:
        try:
            source_relative_dir=path.parent.resolve().relative_to(
                source_root.resolve())
        except ValueError:
            source_relative_dir=Path()
    h,c=t.translate(src,out_root,path.stem,source_relative_dir)
    for generated in t.generated_files:
        print(f'Wrote {generated}')
    for d in t.diag: print(f'{path}: WARNING: {d}',file=sys.stderr)

def main():
    ap=argparse.ArgumentParser(description='Java 8 to JXX C++17 translator')
    ap.add_argument('input',nargs='?',help='single Java source file')
    ap.add_argument('--dir',dest='src_dir',help='recursively convert every .java file')
    ap.add_argument('--out',required=True,help='output root; .h and .cpp are written together under package folders')
    ap.add_argument('--cmake',action='store_true',help='generate CMakeLists.txt in output root')
    ap.add_argument('--cmake-project',default='TranspiledProject')
    ap.add_argument('--cmake-target',default='transpiled')
    ap.add_argument(
        '--jxx-include',
        help=('JXX SDK root containing package directories such as lang/, io/, '
              'util/, net/, ext/, com/, and org/. The normalized path is added '
              'to generated CMake target_include_directories.'))
    a=ap.parse_args()
    if javalang is None: raise SystemExit('Install dependency: pip install javalang')
    if bool(a.input)==bool(a.src_dir): ap.error('provide either input or --dir')
    if a.src_dir:
        source_root=Path(a.src_dir)
        files=sorted(source_root.rglob('*.java'))
        if not files: raise SystemExit(f'No .java files found under {source_root}')
        source_type_index=build_source_type_index(files,source_root)
        for path in files:
            translate_file(path,a.out,source_type_index,source_root)
    else:
        input_path=Path(a.input)
        sibling_files=sorted(input_path.parent.glob('*.java'))
        source_type_index=build_source_type_index(
            sibling_files,input_path.parent)
        translate_file(
            input_path,a.out,source_type_index,input_path.parent)
    if a.cmake:
        cmake = generate_cmake_files(
            a.out, a.cmake_project, a.cmake_target, a.jxx_include)
        for generated in sorted(Path(a.out).rglob('CMakeLists.txt')):
            print(f'Wrote {generated}')
if __name__=='__main__': main()
