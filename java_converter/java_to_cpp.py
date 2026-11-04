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
import argparse, os, re, sys, time
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, List, Optional, Sequence, Set, Tuple
from sdk_metadata import SdkMetadataIndex

try:
    import javalang
except Exception:
    javalang=None

PRIMITIVES={
 'byte':'::jxx::lang::jbyte','short':'::jxx::lang::jshort',
 'int':'::jxx::lang::jint','long':'::jxx::lang::jlong',
 'float':'::jxx::lang::jfloat','double':'::jxx::lang::jdouble',
 'boolean':'::jxx::lang::jbool','char':'::jxx::lang::jchar','void':'void'}
PRIMITIVE_ARRAY_ALIASES={
    'boolean':'BooleanArray','byte':'ByteArray','char':'CharArray',
    'short':'ShortArray','int':'IntArray','long':'LongArray',
    'float':'FloatArray','double':'DoubleArray',
}
PRIMITIVE_UNBOX_METHODS={
    'byte':'byteValue','short':'shortValue','int':'intValue',
    'long':'longValue','float':'floatValue','double':'doubleValue',
    'boolean':'booleanValue','char':'charValue',
}
WRAPPER_TO_PRIMITIVE={
    'Byte':'byte','Short':'short','Integer':'int','Long':'long',
    'Float':'float','Double':'double','Boolean':'boolean','Character':'char',
}
NUMERIC_PRIMITIVES={'byte','short','char','int','long','float','double'}
NUMERIC_OPERATORS={'+','-','*','/','%','<','<=','>','>=','==','!='}
UNSUPPORTED=('java.lang.annotation','java.lang.instrument','java.lang.invoke')
# Names supplied by the implicit language package or by the converter's
# standard support includes must not be resolved through an on-demand import.
DEFAULT_LANGUAGE_TYPES={
    'Object','String','StringBuilder','StringBuffer','System','JxxArray',
    'Class','Integer','Character','Boolean','Byte','Short','Long','Float',
    'Double','Math','StrictMath','Throwable','Exception','RuntimeException',
    'Error',
    # Implicit java.lang throwable types. Catch clauses do not need source
    # imports, but generated C++ requires the concrete JXX declaration.
    'ArithmeticException','ArrayIndexOutOfBoundsException',
    'ArrayStoreException','ClassCastException','ClassNotFoundException',
    'CloneNotSupportedException','EnumConstantNotPresentException',
    'IllegalAccessException','IllegalArgumentException',
    'IllegalMonitorStateException','IllegalStateException',
    'IllegalThreadStateException','IndexOutOfBoundsException',
    'InstantiationException','InterruptedException',
    'NegativeArraySizeException','NoSuchFieldException',
    'NoSuchMethodException','NullPointerException','NumberFormatException',
    'ReflectiveOperationException','SecurityException',
    'StringIndexOutOfBoundsException','TypeNotPresentException',
    'UnsupportedOperationException','AbstractMethodError','AssertionError',
    'ClassCircularityError','ClassFormatError','ExceptionInInitializerError',
    'IllegalAccessError','IncompatibleClassChangeError',
    'InstantiationError','InternalError','LinkageError',
    'NoClassDefFoundError','NoSuchFieldError','NoSuchMethodError',
    'OutOfMemoryError','StackOverflowError','ThreadDeath',
    'UnknownError','UnsatisfiedLinkError','UnsupportedClassVersionError',
    'VerifyError','VirtualMachineError'
}
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
METHOD_CALL_DEPENDENCIES={
    # IntStream::forEach has an IntConsumer parameter. Generated lambdas are
    # passed through that source-parity interface, so its complete declaration
    # must be visible in the generated translation unit.
    'forEach': ('java.util.function.IntConsumer',),
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

class ConversionFailure(RuntimeError):
    def __init__(self,message,warnings=0):
        super().__init__(message)
        self.warnings=warnings


class Translator:
    def __init__(self):
        self.imports:Dict[str,str]={}; self.wildcard_imports:List[str]=[]
        self.includes:Set[str]=set(); self.source_includes:Set[str]=set(); self.diag:List[str]=[]
        self.errors:List[str]=[]
        self.package=''; self.current_class=''; self.current_method=''; self.current_return='void'
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
        self.source_method_parameters:Dict[str,Dict[str,List[object]]]={}
        self.source_method_return_types:Dict[str,Dict[str,object]]={}
        self.source_method_varargs:Dict[str,Dict[str,bool]]={}
        self.local_variable_types:Dict[str,object]={}
        self.local_array_variables:Set[str]=set()
        self.local_primitive_rank1_arrays:Set[str]=set()
        self.catch_reference_variables:Set[str]=set()
        self.source_type_index:Dict[str,Tuple[str,str]]={}
        self.source_type_kinds:Dict[str,str]={}
        self.current_output_relative_dir=Path()
        self.active_wildcard_types:Dict[int,str]={}
        self.sdk_metadata:Optional[SdkMetadataIndex]=None

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

    def source_include_for_current_output(self, include:str)->str:
        include_path=Path(include)
        current_dir=Path(self.current_output_relative_dir)
        # Headers generated beside the current header are included by basename.
        # Otherwise retain the output-root-relative path used by generated CMake.
        if include_path.parent == current_dir:
            return include_path.name
        return include_path.as_posix()

    def qmap(self,name:str)->Tuple[str,Optional[str]]:
        # Resolve implicit language-package types before considering wildcard
        # imports. Otherwise import java.util.* incorrectly maps String to
        # ::jxx::util::String and emits util/jxx.util.String.h.
        if name in DEFAULT_LANGUAGE_TYPES:
            if name=='String': name='java.lang.String'
            elif name=='Object': name='java.lang.Object'
            elif name=='System': name='java.lang.System'
            elif name=='Class': name='java.lang.ClassAny'
            elif name=='JxxArray': return '::jxx::lang::JxxArray',None
            else: name=f'java.lang.{name}'
        elif name in self.imports:
            imported_name=self.imports[name]
            if imported_name in self.source_type_index:
                qualified,include=self.source_type_index[imported_name]
                return qualified,self.source_include_for_current_output(include)
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
            return qualified,self.source_include_for_current_output(include)
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

    def template_argument_type(self,t)->str:
        if t is None:
            return '::jxx::lang::Object'
        name=self.name_of(t)
        dimensions=len(getattr(t,'dimensions',None) or [])
        if dimensions:
            return self.resolve(t).value()
        if name in self.type_parameters:
            return name
        if type(t).__name__=='BasicType' or name in PRIMITIVES:
            return self.resolve(t).value()
        # JXX generic containers apply reference semantics internally to their
        # source object type parameter. Pass the raw class/interface type here,
        # not Ptr<T>, to avoid Ptr<Ptr<T>> in methods such as List<E>::get().
        return self.ref_raw(t)

    def ref_raw(self,t)->str:
        name=self.name_of(t); raw,inc=self.qmap(name)
        if inc: self.includes.add(inc)
        args=[]
        declared_arguments=getattr(t,'arguments',None) or []
        for a in declared_arguments:
            at=getattr(a,'type',None)
            if at is None:
                args.append(self.active_wildcard_types.get(
                    id(a),'::jxx::lang::Object'))
            else:
                args.append(self.template_argument_type(at))
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

    @staticmethod
    def primitive_array_alias(name:str,rank:int,concrete:bool=False)->Optional[str]:
        if rank != 1 or name not in PRIMITIVE_ARRAY_ALIASES:
            return None
        alias=PRIMITIVE_ARRAY_ALIASES[name]
        if concrete:
            alias+="Type"
        return f'::jxx::lang::{alias}'

    def array_concrete_type(self,t,rank:int)->str:
        name=self.name_of(t)
        alias=self.primitive_array_alias(name,rank,True)
        if alias:
            return alias
        element=self.resolve(t).value()
        return f'::jxx::lang::JxxArray<{element}, {rank}U>'

    def resolve(self,t,extra_dims=0)->CType:
        if t is None: return CType('void',void=True)
        tn=type(t).__name__; name=self.name_of(t)
        dims=len(getattr(t,'dimensions',None) or [])+extra_dims
        # A source type parameter denotes a reference type. The C++ template
        # argument is the raw JXX class/interface type, while values of that
        # parameter use Ptr<T>, matching source reference semantics.
        if name in self.type_parameters:
            base=CType(name)
        elif tn=='BasicType' or name in PRIMITIVES:
            base=CType(PRIMITIVES.get(name,name),primitive=True,void=(name=='void'))
        else: base=CType(self.ref_raw(t))
        if not dims: return base
        primitive_alias=self.primitive_array_alias(name,dims)
        if primitive_alias:
            return CType(primitive_alias,primitive=True)
        elem=base.value() if not base.primitive else base.raw
        return CType(
            f'::jxx::Ptr<::jxx::lang::JxxArray<{elem}, {dims}U>>',
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

    @classmethod
    def ast_main_entries(cls, tree) -> Dict[str,str]:
        """Return source entry methods keyed by their exact top-level owner."""
        entries:Dict[str,str]={}
        # Use compilation-unit declarations directly. tree.filter() also walks
        # nested classes and supplies path tuples, which is unnecessary for a
        # native process entry point and can lose the intended top-level owner.
        for declaration in getattr(tree,'types',None) or []:
            if type(declaration).__name__!='ClassDeclaration':
                continue
            for method in getattr(declaration,'body',None) or []:
                if type(method).__name__!='MethodDeclaration':
                    continue
                if getattr(method,'name',None)!='main':
                    continue
                modifiers=set(getattr(method,'modifiers',None) or [])
                if not {'public','static'}.issubset(modifiers):
                    continue
                # javalang represents a source void return type as None.
                return_type=getattr(method,'return_type',None)
                if return_type is not None and cls.name_of(return_type)!='void':
                    continue
                parameters=list(getattr(method,'parameters',None) or [])
                if len(parameters)!=1:
                    continue
                parameter=parameters[0]
                if cls.simple(cls.name_of(parameter.type))!='String':
                    continue
                entries[declaration.name]=parameter.name
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
        enum_type=getattr(getattr(javalang,'tree',None),'EnumDeclaration',None)
        filter_method=getattr(tree,'filter',None)
        if enum_type is not None and callable(filter_method):
            self.known_types.update(
                declaration.name for _,declaration in filter_method(enum_type))
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
            methods=self.source_method_parameters.setdefault(declaration.name,{})
            returns=self.source_method_return_types.setdefault(declaration.name,{})
            varargs=self.source_method_varargs.setdefault(declaration.name,{})
            for member in getattr(declaration,'body',None) or []:
                member_kind=type(member).__name__
                if member_kind=='FieldDeclaration':
                    for declarator in member.declarators or []:
                        fields[declarator.name]=member.type
                elif member_kind=='MethodDeclaration':
                    methods[member.name]=list(member.parameters or [])
                    returns[member.name]=member.return_type
                    varargs[member.name]=bool(
                        member.parameters and
                        getattr(member.parameters[-1],'varargs',False))
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
                self.errors.append(f'unsupported reflection type: {p}'); continue
            if any(p==x or p.startswith(x+'.') for x in UNSUPPORTED):
                self.errors.append(f'unsupported package: {p}'); continue
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
        if any(type(node).__name__=='EnumDeclaration'
               for declaration in self.class_declarations(tree)
               for node in getattr(declaration,'body',None) or []):
            self.includes.update({
                'lang/jxx.lang.String.h',
                'lang/jxx.lang.Enum.h',
            })
        if any(self.is_java_main(m,t.name)
               for t in self.class_declarations(tree)
               for m in getattr(t,'body',None) or []):
            self.includes.update({'lang/jxx.lang.System.h','lang/jxx.lang.String.h','lang/jxx.lang.Throwable.h'})

    def record_qualified_type_dependency(self, qualifier):
        if not qualifier:
            return
        root=str(qualifier).split('.')[0]
        # A qualified expression beginning with a source local is instance
        # access, not a static type dependency.
        if root in self.local_variable_types:
            return
        if (root in self.imports or root in DEFAULT_LANGUAGE_TYPES or
                root in self.source_type_index or
                (root and root[0].isupper())):
            qualified_name=self.imports.get(root,root)
            _,include=self.qmap(qualified_name)
            if include:
                self.includes.add(include)

    def record_method_dependencies(self, invocation):
        self.record_qualified_type_dependency(
            getattr(invocation,'qualifier',None))
        member=getattr(invocation,'member',None)
        result_type=METHOD_RESULT_TYPES.get(member)
        if result_type:
            _,include=self.qmap(result_type)
            if include:
                self.includes.add(include)
        for dependency in METHOD_CALL_DEPENDENCIES.get(member,()):
            _,include=self.qmap(dependency)
            if include:
                self.includes.add(include)

    def gather_body_type_includes(self, tree):
        """Resolve body-level declared and constructed types before header output."""
        filter_method=getattr(tree,'filter',None)
        tree_module=getattr(javalang,'tree',None)
        if callable(filter_method) and tree_module is not None:
            local_type=getattr(tree_module,'LocalVariableDeclaration',None)
            reference_type=getattr(tree_module,'ReferenceType',None)
            creator_type=getattr(tree_module,'ClassCreator',None)
            catch_type=getattr(tree_module,'CatchClauseParameter',None)
            invocation_type=getattr(tree_module,'MethodInvocation',None)
            member_reference_type=getattr(tree_module,'MemberReference',None)
            try_statement_type=getattr(tree_module,'TryStatement',None)
            # Include the RAII helper before header/source emission is split.
            # Adding this dependency only while emitting emit_try_statement()
            # is too late because source() writes its include list first.
            if try_statement_type is not None:
                for _,node in filter_method(try_statement_type):
                    if getattr(node,'finally_block',None) is not None:
                        self.includes.add('lang/jxx.lang.Finally.h')
                        break
            if invocation_type is not None:
                for _,node in filter_method(invocation_type):
                    self.record_method_dependencies(node)
            if member_reference_type is not None:
                for _,node in filter_method(member_reference_type):
                    self.record_qualified_type_dependency(
                        getattr(node,'qualifier',None))
            # Source types from the same package or unnamed-package folder
            # do not require import declarations. Resolve every reference type
            # before emitting the header so cross-source dependencies such as
            # SavingAccount and CheckingAccount add their generated headers.
            if reference_type is not None:
                for _,node in filter_method(reference_type):
                    name=self.name_of(node)
                    if name not in self.known_types:
                        self.ref_raw(node)
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
        # javalang represents a source void return type as None.
        return_type=getattr(m,'return_type',None)
        if return_type is not None and self.name_of(return_type) != 'void':
            return False
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
            "::jxx::Ptr<::jxx::lang::JxxArray<"
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
            declarator_dimensions=(
                (1 if getattr(p,'varargs',False) else 0) +
                len(getattr(p,'dimensions',None) or []))
            type_dimensions=len(getattr(p.type,'dimensions',None) or [])
            total_dimensions=type_dimensions+declarator_dimensions
            parameter_type=self.resolve(p.type,declarator_dimensions)
            # A scalar source type parameter uses reference semantics. An array
            # of that type parameter must retain its JxxArray wrapper; checking
            # only declarator dimensions incorrectly turned E[] into const E&
            # whenever javalang stored [] on the ReferenceType node.
            if (self.name_of(p.type) in self.type_parameters and
                    total_dimensions == 0):
                rendered=f'const ::jxx::Ptr<{self.name_of(p.type)}>&'
            elif total_dimensions > 0:
                rendered=f'const {parameter_type.value()}&'
            else:
                rendered=parameter_type.param()
            ps.append(f'{rendered} {p.name}')
        qualified=f'{cname}::{m.name}' if definition else m.name
        if ctor: qualified=f'{cname}::{cname}' if definition else cname; return f'{qualified}({", ".join(ps)})'
        ret=self.resolve(m.return_type,len(getattr(m,'dimensions',None) or [])).value()
        return f'{ret} {qualified}({", ".join(ps)})'

    def is_primitive_rank1_array(self,type_node,extra_dims=0)->bool:
        rank=len(getattr(type_node,'dimensions',None) or [])+extra_dims
        return rank==1 and self.name_of(type_node) in PRIMITIVE_ARRAY_ALIASES

    def array_size_expression(self,name:str)->str:
        return f'{name}->length'

    def array_element_expression(self,name:str,index:str)->str:
        return f'(*{name})[{index}]'

    def local_type_name(self,name:str)->str:
        return self.simple(self.name_of(self.local_variable_types.get(name)))

    def indexed_iterable_element_expression(self,name:str,index:str)->str:
        if name in self.local_array_variables:
            return self.array_element_expression(name,index)
        if self.local_type_name(name) in {
                'List','ArrayList','LinkedList','Vector','Stack'}:
            return f'{name}->get({index})'
        # Fallback for iterable types with indexed source semantics.
        return f'{name}->get({index})'

    def indexed_iterable_size_expression(self,name:str)->str:
        if name in self.local_array_variables:
            return self.array_size_expression(name)
        return f'{name}->size()'

    def apply_expression_selectors(
            self,base,selectors,primitive_rank1_array:bool=False):
        result=base
        array_depth=0
        for selector in selectors or []:
            kind=type(selector).__name__
            if kind=='MethodInvocation':
                arguments=', '.join(
                    self.method_invocation_arguments(selector))
                result+=f'->{selector.member}({arguments})'
            elif kind=='MemberReference':
                result+=f'->{selector.member}'
            elif kind=='ArraySelector':
                index=self.expr(selector.index)
                if array_depth == 0:
                    # A source array expression is Ptr<JxxArray<...>> and must
                    # be dereferenced once. Higher ranks use recursive RowProxy
                    # operator[] access and therefore chain brackets directly.
                    result=f'(*{result})[{index}]'
                else:
                    result=f'{result}[{index}]'
                array_depth+=1
                primitive_rank1_array=False
            else:
                result+=str(selector)
        return result

    @staticmethod
    def is_string_literal_expression(expression):
        return (type(expression).__name__=='Literal' and
                isinstance(getattr(expression,'value',None),str) and
                getattr(expression,'value').startswith('"'))

    def expression_type_name(self, expression):
        kind=type(expression).__name__
        if kind=='MemberReference':
            qualifier=getattr(expression,'qualifier',None)
            if not qualifier:
                value_type=(self.local_variable_types.get(expression.member) or
                            self.source_field_type(
                                self.current_class,expression.member))
                return self.simple(self.name_of(value_type))
            return self.simple(self.name_of(
                self.assignment_target_type(expression)))
        if kind=='Literal':
            value=str(getattr(expression,'value',''))
            if value in {'true','false'}: return 'boolean'
            if value.startswith("'"): return 'char'
            if value.lower().endswith('l'): return 'long'
            if value.lower().endswith('f'): return 'float'
            if any(ch in value for ch in '.eE'): return 'double'
            return 'int'
        if kind=='MethodInvocation':
            owner=self.invocation_owner_name(
                getattr(expression,'qualifier',None))
            declared=self.source_method_return_types.get(owner,{}).get(
                expression.member)
            if declared is not None:
                return self.simple(self.name_of(declared))
            metadata=self.sdk_method_metadata(expression)
            if metadata is not None:
                if metadata.return_kind=='primitive':
                    return metadata.primitive_name
                if metadata.return_kind=='reference':
                    return metadata.pointee.split('::')[-1].replace('>','').strip()
        if kind=='Cast':
            return self.simple(self.name_of(expression.type))
        if kind=='BinaryOperation':
            left=self.expression_type_name(expression.operandl)
            right=self.expression_type_name(expression.operandr)
            if left=='String' or right=='String': return 'String'
            primitive_left=WRAPPER_TO_PRIMITIVE.get(left,left)
            primitive_right=WRAPPER_TO_PRIMITIVE.get(right,right)
            order=['byte','short','char','int','long','float','double']
            candidates=[name for name in (primitive_left,primitive_right)
                        if name in order]
            if candidates:
                result=max(candidates,key=order.index)
                return 'int' if result in {'byte','short','char'} else result
        return ''

    def unboxed_numeric_expression(self, expression):
        value=self.expr(expression)
        type_name=self.expression_type_name(expression)
        primitive=WRAPPER_TO_PRIMITIVE.get(type_name)
        if primitive in NUMERIC_PRIMITIVES:
            return f'({value})->{PRIMITIVE_UNBOX_METHODS[primitive]}()'
        return value

    def numeric_binary_expression(self, expression):
        left=self.unboxed_numeric_expression(expression.operandl)
        right=self.unboxed_numeric_expression(expression.operandr)
        return f'({left} {expression.operator} {right})'

    def return_expression(self, expression):
        value=self.expr(expression)
        if not self.current_return.startswith('::jxx::Ptr<'):
            return value
        target=self.current_return[len('::jxx::Ptr<'):-1]
        target_simple=target.split('::')[-1].replace('>','').strip()
        primitive=WRAPPER_TO_PRIMITIVE.get(target_simple)
        if (primitive is not None and
                type(expression).__name__=='BinaryOperation' and
                expression.operator in NUMERIC_OPERATORS):
            return f'::jxx::lang::{target_simple}::valueOf({value})'
        return value

    def is_string_expression(self, expression):
        if self.is_string_literal_expression(expression):
            return True
        kind=type(expression).__name__
        if kind=='MemberReference':
            qualifier=getattr(expression,'qualifier',None)
            if not qualifier:
                value_type=self.local_variable_types.get(expression.member)
                return self.simple(self.name_of(value_type))=='String'
            field_type=self.assignment_target_type(expression)
            return self.simple(self.name_of(field_type))=='String'
        if kind=='MethodInvocation':
            return expression.member in {
                'toString','nextLine','next','substring','concat','valueOf'}
        if kind=='BinaryOperation' and expression.operator=='+':
            return (self.is_string_expression(expression.operandl) or
                    self.is_string_expression(expression.operandr))
        return False

    def is_string_concatenation(self, expression):
        if self.is_string_literal_expression(expression):
            return True
        return (type(expression).__name__=='BinaryOperation' and
                getattr(expression,'operator',None)=='+' and
                (self.is_string_expression(expression.operandl) or
                 self.is_string_expression(expression.operandr)))

    def string_value_expression(self, expression):
        if self.is_string_literal_expression(expression):
            return f'::jxx::NEW<::jxx::lang::String>({self.expr(expression)})'
        if (type(expression).__name__=='MemberReference' and
                not getattr(expression,'qualifier',None) and
                expression.member in self.catch_reference_variables):
            return (f'::jxx::lang::String::valueOf('
                    f'::jxx::CAST<::jxx::lang::Object>('
                    f'{expression.member}.thisPtr()))')
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
        if (target_type is not None and
                type(value).__name__=='ClassCreator'):
            # Assignment context supplies diamond/raw generic arguments just as
            # declaration initialization does. Preserve the concrete creator
            # type while specializing it from the target type.
            return self.class_creator_initializer_expression(target_type,value)
        return self.expr(value)

    def sdk_method_metadata(self, invocation):
        if (self.sdk_metadata is None or
                type(invocation).__name__!='MethodInvocation'):
            return None
        owner=self.invocation_owner_name(
            getattr(invocation,'qualifier',None))
        return self.sdk_metadata.lookup(
            owner,invocation.member,
            len(getattr(invocation,'arguments',None) or []))

    @staticmethod
    def wrapper_for_primitive(name):
        return {
            'byte':'Byte','short':'Short','int':'Integer','long':'Long',
            'float':'Float','double':'Double','boolean':'Boolean',
            'char':'Character'}.get(name,'')

    def expression_requires_unboxing(self,target_name,initializer):
        if target_name not in PRIMITIVE_UNBOX_METHODS:
            return False
        metadata=self.sdk_method_metadata(initializer)
        if metadata is not None:
            # SDK headers are authoritative. Primitive-returning methods such as
            # Scanner::nextLong already produce jlong and must not be unboxed.
            if metadata.return_kind=='primitive':
                return False
            if metadata.return_kind=='reference':
                pointee=metadata.pointee.split('::')[-1]
                pointee=pointee.replace('>','').strip()
                return pointee==self.wrapper_for_primitive(target_name)
            return False
        # Source generic methods can return a wrapper through a type parameter.
        if type(initializer).__name__=='MethodInvocation':
            qualifier=getattr(initializer,'qualifier',None)
            owner_type=self.local_variable_types.get(qualifier)
            owner=self.simple(self.name_of(owner_type))
            declared=self.source_method_return_types.get(owner,{}).get(
                initializer.member)
            declared_name=self.name_of(declared)
            parameters=self.generic_class_parameters.get(owner,[])
            arguments=list(getattr(owner_type,'arguments',None) or [])
            if declared_name in parameters:
                index=parameters.index(declared_name)
                if index < len(arguments):
                    actual=self.name_of(
                        getattr(arguments[index],'type',None))
                    return actual==self.wrapper_for_primitive(target_name)
        return False

    def expression_for_target_type(self, expression, target_type):
        """Apply source boxing or unboxing for a contextually expected type."""
        target_name=self.simple(self.name_of(target_type))
        source_name=self.expression_type_name(expression)
        value=self.expr(expression)
        if target_name in PRIMITIVE_UNBOX_METHODS:
            if WRAPPER_TO_PRIMITIVE.get(source_name) == target_name:
                return f'({value})->{PRIMITIVE_UNBOX_METHODS[target_name]}()'
            if self.expression_requires_unboxing(target_name,expression):
                return f'({value})->{PRIMITIVE_UNBOX_METHODS[target_name]}()'
            return value
        if (target_name in WRAPPER_TO_PRIMITIVE and
                source_name in PRIMITIVES):
            return self.constructor_argument_expression(expression,target_type)
        return value

    def declaration_initializer_expression(self,target_type,initializer):
        if self.is_string_literal_expression(initializer):
            return self.constructor_argument_expression(initializer,target_type)
        return self.expression_for_target_type(initializer,target_type)

    def begin_member_scope(self,owner_name,member):
        self.current_class=owner_name
        self.current_method=getattr(member,'name','')
        self.local_variable_types={}
        self.local_array_variables=set()
        self.local_primitive_rank1_arrays=set()
        self.catch_reference_variables=set()
        for parameter in getattr(member,'parameters',None) or []:
            self.local_variable_types[parameter.name]=parameter.type
            dimensions=(len(getattr(parameter.type,'dimensions',None) or []) +
                        len(getattr(parameter,'dimensions',None) or []) +
                        (1 if getattr(parameter,'varargs',False) else 0))
            if dimensions > 0:
                self.local_array_variables.add(parameter.name)
            if self.is_primitive_rank1_array(parameter.type,
                    len(getattr(parameter,'dimensions',None) or []) +
                    (1 if getattr(parameter,'varargs',False) else 0)):
                self.local_primitive_rank1_arrays.add(parameter.name)

    def invocation_owner_name(self,qualifier):
        if not qualifier:
            return self.current_class
        owner_type=self.local_variable_types.get(qualifier)
        if owner_type is not None:
            return self.simple(self.name_of(owner_type))
        return self.simple(qualifier)

    def boxed_object_expression(self, expression):
        value=self.expr(expression)
        kind=type(expression).__name__
        if self.is_string_literal_expression(expression):
            boxed=f'::jxx::NEW<::jxx::lang::String>({value})'
        elif kind=='Cast':
            name=self.name_of(expression.type)
            wrapper={
                'byte':'Byte','short':'Short','int':'Integer','long':'Long',
                'float':'Float','double':'Double','boolean':'Boolean',
                'char':'Character'}.get(name)
            boxed=(f'::jxx::lang::{wrapper}::valueOf({value})'
                   if wrapper else value)
        elif kind=='Literal':
            literal=str(getattr(expression,'value',''))
            if literal in {'true','false'}:
                boxed=f'::jxx::lang::Boolean::valueOf({value})'
            elif literal.startswith("'"):
                boxed=f'::jxx::lang::Character::valueOf({value})'
            elif any(ch in literal for ch in '.eE'):
                boxed=f'::jxx::lang::Double::valueOf({value})'
            else:
                boxed=f'::jxx::lang::Integer::valueOf({value})'
        elif kind=='MemberReference':
            local_type=self.local_variable_types.get(expression.member)
            name=self.name_of(local_type)
            wrapper={
                'byte':'Byte','short':'Short','int':'Integer','long':'Long',
                'float':'Float','double':'Double','boolean':'Boolean',
                'char':'Character'}.get(name)
            boxed=(f'::jxx::lang::{wrapper}::valueOf({value})'
                   if wrapper else value)
        elif kind=='MethodInvocation':
            owner=self.invocation_owner_name(
                getattr(expression,'qualifier',None))
            return_type=self.source_method_return_types.get(owner,{}).get(
                expression.member)
            return_name=self.name_of(return_type)
            wrapper={
                'byte':'Byte','short':'Short','int':'Integer','long':'Long',
                'float':'Float','double':'Double','boolean':'Boolean',
                'char':'Character'}.get(return_name)
            boxed=(f'::jxx::lang::{wrapper}::valueOf({value})'
                   if wrapper else value)
        elif kind=='BinaryOperation':
            # Arithmetic expressions used as Object varargs have source numeric
            # type. Preserve floating-point expressions as Double; integral
            # expressions default to Integer in the absence of a wider operand.
            rendered=value
            text=str(rendered)
            wrapper=('Double' if any(token in text for token in ('.','jdouble'))
                     else 'Integer')
            boxed=f'::jxx::lang::{wrapper}::valueOf({value})'
        else:
            boxed=value
        return (f'::jxx::CAST<::jxx::lang::Object>({boxed})')

    def sdk_object_varargs_arguments(self, invocation):
        member=getattr(invocation,'member',None)
        if member not in {'printf','format'}:
            return None
        arguments=list(getattr(invocation,'arguments',None) or [])
        if not arguments:
            return None
        locale_first=(len(arguments)>=2 and
                      self.simple(self.name_of(
                          self.local_variable_types.get(
                              getattr(arguments[0],'member',''))))=='Locale')
        fixed_count=2 if locale_first else 1
        if len(arguments)<fixed_count:
            return None
        rendered=[]
        for argument in arguments[:fixed_count]:
            rendered.append(
                f'::jxx::NEW<::jxx::lang::String>({self.expr(argument)})'
                if self.is_string_literal_expression(argument)
                else self.expr(argument))
        values=arguments[fixed_count:]
        array_type=('::jxx::lang::JxxArray<'
                    '::jxx::Ptr<::jxx::lang::Object>, 1U>')
        array=(f'::jxx::NEW<{array_type}>('
               f'static_cast<::jxx::lang::jint>({len(values)}))')
        if values:
            assignments=' '.join(
                f'(*array)[{index}] = {self.boxed_object_expression(value)};'
                for index,value in enumerate(values))
            array=(f'([&]() {{ auto array = {array}; {assignments} '
                   'return array; })()')
        rendered.append(array)
        return rendered

    def method_invocation_arguments(self,invocation):
        sdk_varargs=self.sdk_object_varargs_arguments(invocation)
        if sdk_varargs is not None:
            return sdk_varargs
        arguments=list(getattr(invocation,'arguments',None) or [])
        owner=self.invocation_owner_name(getattr(invocation,'qualifier',None))
        parameters=self.source_method_parameters.get(owner,{}).get(
            invocation.member,[])
        is_varargs=self.source_method_varargs.get(owner,{}).get(
            invocation.member,False)
        if not is_varargs or not parameters:
            rendered=[]
            for index,argument in enumerate(arguments):
                if (self.is_this_expression(argument) and
                        index < len(parameters)):
                    rendered.append(self.this_ptr_expression(
                        parameters[index].type))
                elif (type(argument).__name__=='ClassCreator' and
                        index < len(parameters)):
                    # Invocation context supplies the expected generic type for
                    # a diamond/raw constructor expression. For example,
                    # addLast(new Node<>(value)) must allocate Node<T>, not the
                    # incomplete C++ template Node.
                    rendered.append(
                        self.class_creator_initializer_expression(
                            parameters[index].type,argument))
                elif self.is_string_literal_expression(argument):
                    # Source string literals are source String objects, not
                    # native const char*. Keep this ahead of parameter-directed
                    # conversion because a generic parameter such as T or E is
                    # not a type name available in a non-template caller.
                    rendered.append(
                        f'::jxx::NEW<::jxx::lang::String>('
                        f'{self.expr(argument)})')
                elif index < len(parameters):
                    parameter_type=parameters[index].type
                    parameter_name=self.simple(self.name_of(parameter_type))
                    argument_name=self.expression_type_name(argument)
                    # Invocation conversion is required here only for a concrete
                    # wrapper parameter receiving a primitive expression. Do not
                    # emit CONVERT<Ptr<T>> or CONVERT<Ptr<E>> from the callee's
                    # declaration: those type parameters are scoped to the
                    # generic class/method, not to this calling function. Generic
                    # forwarding overloads handle those calls instead.
                    if ((parameter_name in WRAPPER_TO_PRIMITIVE and
                             argument_name in PRIMITIVES) or
                            (parameter_name in PRIMITIVES and
                             argument_name in WRAPPER_TO_PRIMITIVE)):
                        rendered.append(self.expression_for_target_type(
                            argument,parameter_type))
                    else:
                        rendered.append(self.expr(argument))
                else:
                    rendered.append(self.expr(argument))
            return rendered
        fixed_count=len(parameters)-1
        rendered=[
            (f'::jxx::NEW<::jxx::lang::String>({self.expr(argument)})'
             if self.is_string_literal_expression(argument)
             else self.expr(argument))
            for argument in arguments[:fixed_count]]
        vararg_parameter=parameters[-1]
        concrete=self.array_concrete_type(vararg_parameter.type,1)
        values=arguments[fixed_count:]
        array_expression=(
            f'::jxx::NEW<{concrete}>('
            f'static_cast<::jxx::lang::jint>({len(values)}))')
        if values:
            primitive_varargs=(
                self.name_of(vararg_parameter.type) in PRIMITIVE_ARRAY_ALIASES)
            def vararg_value(value):
                if self.is_string_literal_expression(value):
                    return (f'::jxx::NEW<::jxx::lang::String>('
                            f'{self.expr(value)})')
                return self.expr(value)
            assignments=' '.join(
                ((f'(*array)[{index}] = {vararg_value(value)};')
                 if primitive_varargs else
                 (f'(*array)[{index}] = {vararg_value(value)};'))
                for index,value in enumerate(values))
            array_expression=(
                f'([&]() {{ auto array = {array_expression}; '
                f'{assignments} return array; }})()')
        rendered.append(array_expression)
        return rendered

    def expr(self,e):
        if e is None:return ''
        n=type(e).__name__
        if n=='Literal': return 'nullptr' if e.value in (None,'null') else e.value
        if n=='LambdaExpression': return self.lambda_expression(e)
        if n=='This':
            selectors=list(getattr(e,'selectors',None) or [])
            if not selectors:
                # A bare source `this` is an object reference. Share the
                # existing JXX ownership block rather than exposing raw `this`.
                # Value-copy contexts explicitly request `*this` elsewhere.
                return 'this->thisPtr()'
            # Qualified access such as this.field or this.method() operates on
            # the current object and does not transfer a reference.
            base='this'
            array_depth=0
            for selector in selectors:
                kind=type(selector).__name__
                if kind=='MemberReference':
                    base+=f'->{selector.member}'
                elif kind=='MethodInvocation':
                    args=', '.join(
                        self.method_invocation_arguments(selector))
                    base+=f'->{selector.member}({args})'
                elif kind=='ArraySelector':
                    index=self.expr(selector.index)
                    base=(f'(*{base})[{index}]' if array_depth == 0
                          else f'{base}[{index}]')
                    array_depth+=1
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
                    if e.member=='length' and q in self.local_array_variables:
                        base=self.array_size_expression(q)
                    else:
                        base=f'{q}->{e.member}'
                elif e.member in STATIC_ACCESSORS.get(fq,set()):
                    base=f'{self.qmap(fq)[0]}::{e.member}()'
                elif q in self.imports or (q and q[0].isupper()):
                    base=f'{self.qualified_static_owner(q)}::{e.member}'
                else:
                    base=f'{q}->{e.member}'
            else: base=e.member
            array_name=(e.member if not q else q)
            base=self.apply_expression_selectors(
                base,getattr(e,'selectors',None),
                array_name in self.local_primitive_rank1_arrays)
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
            invocation=f'{head}({", ".join(self.method_invocation_arguments(e))})'
            return self.apply_expression_selectors(
                invocation,getattr(e,'selectors',None))
        if n=='ClassCreator': return self.class_creator_expression(e)
        if n=='ArrayCreator':
            rank=len(getattr(e.type,'dimensions',None) or []) + len(getattr(e,'dimensions',None) or [])
            rank=max(rank,1)
            concrete=self.array_concrete_type(e.type,rank)
            dims=[self.expr(x) for x in getattr(e,'dimensions',None) or [] if x is not None]
            return f'::jxx::NEW<{concrete}>({", ".join(dims)})'
        if n=='Cast':
            t=self.resolve(e.type); x=self.expr(e.expression)
            return f'static_cast<{t.raw}>({x})' if t.primitive else f'::jxx::CAST<{t.raw}>({x})'
        if n=='BinaryOperation':
            if e.operator=='instanceof':
                return self.instanceof_expression(e.operandl,e.operandr)
            if e.operator=='+' and self.is_string_concatenation(e):
                return self.string_concatenation_expression(e)
            left_type=self.expression_type_name(e.operandl)
            right_type=self.expression_type_name(e.operandr)
            if (e.operator in NUMERIC_OPERATORS and
                    (left_type in WRAPPER_TO_PRIMITIVE or
                     right_type in WRAPPER_TO_PRIMITIVE)):
                return self.numeric_binary_expression(e)
            return f'({self.expr(e.operandl)} {e.operator} {self.expr(e.operandr)})'
        if n=='Assignment':
            value=self.assignment_value_expression(e.expressionl,e.value)
            return f'{self.expr(e.expressionl)} {e.type or "="} {value}'
        if n=='TernaryExpression': return self.ternary_expression(e)
        if n=='ArraySelector': return f'({self.expr(e.index)})'
        return str(e)

    def ternary_expression(self, expression):
        condition=self.expr(expression.condition)
        true_expression=expression.if_true
        false_expression=expression.if_false
        # Source conditional expressions with String operands have String type.
        # Materialize source literals as JXX String references rather than
        # emitting native const char* branches.
        string_result=(self.is_string_expression(true_expression) or
                       self.is_string_expression(false_expression))
        if string_result:
            true_value=self.string_value_expression(true_expression)
            false_value=self.string_value_expression(false_expression)
        else:
            true_value=self.expr(true_expression)
            false_value=self.expr(false_expression)
        return f'({condition} ? {true_value} : {false_value})'

    def instanceof_expression(self, value_expression, target_type):
        # Evaluate the operand exactly once, preserve null instanceof semantics,
        # and route runtime type testing through Object::instanceOf(ClassAny).
        # Casting through Object also supports expressions statically typed as
        # a JXX interface, since interfaces themselves do not inherit Object.
        value=self.expr(value_expression)
        target=self.ref_raw(target_type)
        return (
            '([&]() -> ::jxx::lang::jbool { '
            f'const auto jxxInstanceOfValue = {value}; '
            'if (jxxInstanceOfValue == nullptr) return false; '
            'const auto jxxInstanceOfObject = '
            '::jxx::CAST<::jxx::lang::Object>(jxxInstanceOfValue); '
            'return jxxInstanceOfObject != nullptr && '
            f'jxxInstanceOfObject->instanceOf({target}::Class()); '
            '})()')

    def array_initializer_value(self, element_type, expression):
        value=self.expr(expression)
        name=self.name_of(element_type)
        if name in {'Integer','Character','Boolean','Byte','Short','Long','Float','Double'}:
            raw=self.qmap(name)[0]
            return f'::jxx::NEW<{raw}>({value})'
        return value

    def emit_local_array_initializer(self, declaration, declarator, o, dims):
        array_type=self.resolve(declaration.type,dims).value()
        concrete=self.array_concrete_type(declaration.type,dims)
        values=list(getattr(declarator.initializer,'initializers',None) or [])
        o.w(f'{array_type} {declarator.name} = ::jxx::NEW<{concrete}>({len(values)}U);')
        for index,value in enumerate(values):
            converted=self.array_initializer_value(declaration.type,value)
            o.w(f'(*{declarator.name})[{index}U] = {converted};')

    def class_creator_expression(self, creator, expected_type=None):
        creator_type=creator.type
        creator_arguments=list(getattr(creator_type,'arguments',None) or [])
        expected_arguments=(list(getattr(expected_type,'arguments',None) or [])
                            if expected_type is not None else [])

        # A diamond expression inherits type arguments from its assignment or
        # invocation target, but it must retain its concrete allocation class.
        # For example, List<Object> x = new ArrayList<>() allocates
        # ArrayList<Object>, never the abstract List<Object> target.
        raw=self.ref_raw(creator_type)
        resolved_type_arguments=[]
        effective_arguments=(creator_arguments if creator_arguments
                             else expected_arguments)
        if not creator_arguments and expected_arguments:
            base_raw,include=self.qmap(self.name_of(creator_type))
            if include:
                self.includes.add(include)
            resolved_type_arguments=[
                (self.template_argument_type(getattr(argument,'type',None))
                 if getattr(argument,'type',None) is not None
                 else '::jxx::lang::Object')
                for argument in expected_arguments]
            raw=base_raw+'<'+', '.join(resolved_type_arguments)+'>'
        else:
            for argument in effective_arguments:
                argument_type=getattr(argument,'type',None)
                resolved_type_arguments.append(
                    self.template_argument_type(argument_type)
                    if argument_type is not None
                    else '::jxx::lang::Object')

        class_name=self.name_of(creator_type)
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
                    # Source type parameters are reference types in JXX. The
                    # C++ template argument remains the raw class/interface
                    # type, but constructor values must be Ptr<T>. Therefore
                    # primitive arguments box to Ptr<Integer>, and string
                    # literals convert to Ptr<String>.
                    target_value=f'::jxx::Ptr<{target}>'
                    value=f'::jxx::CONVERT<{target_value}>({value})'
            elif index < len(constructor_parameter_types):
                value=self.constructor_argument_expression(
                    argument,constructor_parameter_types[index])
            elif self.is_string_literal_expression(argument):
                # Constructor metadata is currently gathered from the active
                # compilation unit. A constructor on a sibling converted type
                # (for example Item(String,double)) may therefore be unknown
                # here. Source string literals are still String objects and
                # must never be passed to NEW as native const char* values.
                value=(f'::jxx::NEW<::jxx::lang::String>('
                       f'{self.expr(argument)})')
            converted_arguments.append(value)
        return f'::jxx::NEW<{raw}>({", ".join(converted_arguments)})'

    def class_creator_initializer_expression(self, target_type, creator):
        created=self.class_creator_expression(creator,target_type)
        target_name=self.name_of(target_type)
        creator_name=self.name_of(creator.type)
        if target_name in PRIMITIVES or target_name==creator_name:
            return created
        return f'::jxx::CAST<{self.ref_raw(target_type)}>({created})'

    def for_initializer_expression(self, initializer):
        kind=type(initializer).__name__
        if kind=='VariableDeclaration':
            declarations=[]
            for declarator in initializer.declarators or []:
                dimensions=len(getattr(declarator,'dimensions',None) or [])
                variable_type=self.resolve(initializer.type,dimensions).value()
                self.local_variable_types[declarator.name]=initializer.type
                if dimensions > 0:
                    self.local_array_variables.add(declarator.name)
                if self.is_primitive_rank1_array(initializer.type,dimensions):
                    self.local_primitive_rank1_arrays.add(declarator.name)
                value=(self.declaration_initializer_expression(
                    initializer.type,declarator.initializer)
                    if declarator.initializer is not None else '')
                declarations.append(
                    f'{variable_type} {declarator.name}' +
                    (f' = {value}' if value else ''))
            return ', '.join(declarations)
        return self.expr(initializer)

    @staticmethod
    def node_list(value):
        if value is None:
            return []
        if isinstance(value,(list,tuple)):
            return list(value)
        return [value]

    def traditional_for_statement(self, statement, output):
        control=statement.control
        initializers=self.node_list(getattr(control,'init',None))
        updates=self.node_list(getattr(control,'update',None))
        initializer=', '.join(
            self.for_initializer_expression(value) for value in initializers)
        condition=(self.expr(control.condition)
                   if getattr(control,'condition',None) is not None else '')
        update=', '.join(self.expr(value) for value in updates)
        output.w(f'for ({initializer}; {condition}; {update}) {{')
        output.enter()
        self.stmt(statement.body,output)
        output.exit()
        output.w('}')

    def switch_case_labels(self, case):
        labels=getattr(case,'case',None)
        if labels is None:
            return []
        if isinstance(labels,(list,tuple)):
            return list(labels)
        return [labels]

    def emit_switch_statement(self, statement, output):
        expression=getattr(statement,'expression',None)
        output.w(f'switch ({self.expr(expression)}) {{')
        output.enter()
        for case in getattr(statement,'cases',None) or []:
            labels=self.switch_case_labels(case)
            if labels:
                # Multiple source labels share the same statement sequence and
                # therefore intentionally fall through to one generated body.
                for label in labels:
                    output.w(f'case {self.expr(label)}:')
            else:
                output.w('default:')
            output.enter()
            for case_statement in getattr(case,'statements',None) or []:
                self.stmt(case_statement,output)
            output.exit()
        output.exit()
        output.w('}')

    def emit_statement_sequence(self, statements, output):
        for statement in statements or []:
            self.stmt(statement,output)

    def emit_catch_clause(self, clause, output):
        parameter=clause.parameter
        catch_types=list(getattr(parameter,'types',None) or [])
        if not catch_types:
            catch_types=[self.name_of(getattr(parameter,'type',None))]
        catch_types=[name for name in catch_types if name]
        if not catch_types:
            self.errors.append('catch clause has no exception type')
            return
        for catch_name in catch_types:
            raw,include=self.qmap(catch_name)
            if include:
                self.source_includes.add(include)
            output.w(f'catch (const {raw}& {parameter.name}) {{')
            output.enter()
            previous_type=self.local_variable_types.get(parameter.name)
            was_catch_reference=parameter.name in self.catch_reference_variables
            self.local_variable_types[parameter.name]=(
                getattr(parameter,'type',None) or catch_name)
            self.catch_reference_variables.add(parameter.name)
            self.emit_statement_sequence(getattr(clause,'block',None),output)
            if previous_type is None:
                self.local_variable_types.pop(parameter.name,None)
            else:
                self.local_variable_types[parameter.name]=previous_type
            if not was_catch_reference:
                self.catch_reference_variables.discard(parameter.name)
            output.exit()
            output.w('}')

    def emit_try_statement(self, statement, output):
        resources=list(getattr(statement,'resources',None) or [])
        if resources:
            self.errors.append('try-with-resources is not yet supported')
            output.w('/* TODO unsupported try-with-resources */')
            return
        catches=list(getattr(statement,'catches',None) or [])
        finally_block=getattr(statement,'finally_block',None)
        try_block=getattr(statement,'block',None) or []
        if not catches and finally_block is None:
            self.errors.append('try statement has neither catch nor finally')
            output.w('/* TODO invalid try statement */')
            return
        if finally_block is not None:
            self.source_includes.add('lang/jxx.lang.Finally.h')
            finally_index=getattr(self,'finally_counter',0)
            self.finally_counter=finally_index+1
            guard_name=f'jxxFinally{finally_index}'
            output.w('{')
            output.enter()
            output.w(f'auto {guard_name} = ::jxx::lang::makeFinally([&]() {{')
            output.enter()
            self.emit_statement_sequence(finally_block,output)
            output.exit()
            output.w('});')
            output.w('try {')
            output.enter()
        output.w('try {')
        output.enter()
        self.emit_statement_sequence(try_block,output)
        output.exit()
        output.w('}')
        for clause in catches:
            self.emit_catch_clause(clause,output)
        if finally_block is not None:
            output.exit()
            output.w('} catch (...) {')
            output.enter()
            output.w(f'{guard_name}.runAndDismiss();')
            output.w('throw;')
            output.exit()
            output.w('}')
            output.exit()
            output.w('}')

    def stmt(self,s,o):
        if s is None:return
        n=type(s).__name__
        if n=='BlockStatement':
            for x in getattr(s,'statements',None) or []: self.stmt(x,o)
        elif n=='ReturnStatement':
            if s.expression is None:
                o.w('return;')
            elif self.is_this_expression(s.expression):
                # Returning the current object shares the existing allocation;
                # never construct a new shared_ptr directly from raw this.
                o.w(f'return ::jxx::CAST<{self.current_return[11:-1]}>('
                    'this->thisPtr());' if
                    self.current_return.startswith('::jxx::Ptr<') else
                    'return this->thisPtr();')
            elif (self.current_method=='clone' and
                  type(s.expression).__name__=='MemberReference' and
                  not getattr(s.expression,'qualifier',None) and
                  getattr(s.expression,'member',None)=='Object'):
                # `return Object;` is not a valid source expression. These
                # tutorial sources use it as shorthand for a shallow clone.
                # Calling Object::clone remains ambiguous when the generated
                # hierarchy contains multiple Object paths. Construct the
                # concrete current type through jxx::NEW so ownership metadata is
                # initialized, then return it through the Object reference type.
                # Cast through the generated class's direct ClassBase alias.
                # A direct Rectangle -> Object cast can be ambiguous when the
                # converted hierarchy exposes more than one Object path. Super
                # is the unique direct C++ base of the generated class; from
                # that base, the source superclass path to Object is selected.
                o.w(
                    f'return ::jxx::CAST<::jxx::lang::Object>('
                    f'::jxx::CAST<{self.current_class}::Super>('
                    f'::jxx::NEW<{self.current_class}>(*this)));')
            else:
                o.w(f'return {self.return_expression(s.expression)};')
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
                total_dimensions=(len(getattr(s.type,'dimensions',None) or []) +
                                  dims)
                if total_dimensions > 0 or type(getattr(d,'initializer',None)).__name__=='ArrayCreator':
                    self.local_array_variables.add(d.name)
                if self.is_primitive_rank1_array(s.type,dims):
                    self.local_primitive_rank1_arrays.add(d.name)
                if type(getattr(d,'initializer',None)).__name__=='ClassCreator':
                    initial_value=self.class_creator_initializer_expression(
                        s.type,d.initializer)
                else:
                    initial_value=(self.declaration_initializer_expression(
                        s.type,d.initializer) if d.initializer is not None else '')
                init=f' = {initial_value}' if d.initializer is not None else ''
                const='const ' if 'final' in (s.modifiers or set()) else ''
                o.w(f'{const}{t.value()} {d.name}{init};')
        elif n=='TryStatement':
            self.emit_try_statement(s,o)
        elif n=='IfStatement':
            o.w(f'if ({self.expr(s.condition)}) {{');o.enter();self.stmt(s.then_statement,o);o.exit()
            if s.else_statement is not None:o.w('} else {');o.enter();self.stmt(s.else_statement,o);o.exit()
            o.w('}')
        elif n=='ForStatement':
            control=getattr(s,'control',None)
            if type(control).__name__=='EnhancedForControl':
                variable=control.var
                declarator=variable.declarators[0]
                iterable=self.expr(control.iterable)
                index_name=f'jxxIndex_{declarator.name}'
                self.local_variable_types[declarator.name]=variable.type
                iterable_name=(getattr(control.iterable,'member',None)
                               if type(control.iterable).__name__=='MemberReference'
                               else None)
                size_expression=(self.indexed_iterable_size_expression(
                                    iterable_name)
                                 if iterable_name else f'{iterable}->size()')
                element_expression=(self.indexed_iterable_element_expression(
                                        iterable_name,index_name)
                                    if iterable_name else
                                    f'{iterable}->get({index_name})')
                o.w(f'for (::jxx::lang::jint {index_name} = 0; '
                    f'{index_name} < {size_expression}; ++{index_name}) {{')
                o.enter()
                element_type=self.resolve(variable.type).value()
                o.w(f'{element_type} {declarator.name} = '
                    f'{element_expression};')
                self.stmt(s.body,o)
                o.exit();o.w('}')
            else:
                self.traditional_for_statement(s,o)
        elif n=='SwitchStatement':
            self.emit_switch_statement(s,o)
        elif n=='WhileStatement':
            o.w(f'while ({self.expr(s.condition)}) {{');o.enter();self.stmt(s.body,o);o.exit();o.w('}')
        elif n=='DoStatement':
            o.w('do {')
            o.enter()
            self.stmt(s.body,o)
            o.exit()
            o.w(f'}} while ({self.expr(s.condition)});')
        elif n=='SynchronizedStatement':
            lock=self.expr(getattr(s,'lock',None) or getattr(s,'expression',None)); body=getattr(s,'block',None) or []
            o.w(f'{lock}->synchronized([&]() {{');o.enter()
            for x in body:self.stmt(x,o)
            o.exit();o.w('});')
        elif n=='BreakStatement':o.w('break;')
        elif n=='ContinueStatement':o.w('continue;')
        else:
            message=f'unsupported statement: {n}'
            self.errors.append(message)
            o.w(f'/* TODO {message} */')

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
                converted.append((
                    index,parameter,self.resolve(parameter.type).value()))
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
        is_static=(kind=='MethodDeclaration' and 'static' in modifiers)
        prefix=('static ' if is_static else
                ('virtual ' if kind=='MethodDeclaration' else ''))
        body=getattr(member,'body',None)
        if body is None:
            virt=''
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

    @staticmethod
    def enum_constant_names(declaration)->List[str]:
        # javalang stores enum constants on EnumDeclaration.body.constants,
        # not directly on EnumDeclaration. Keep the direct fallback for test
        # doubles and compatibility with alternate AST front ends.
        body=getattr(declaration,'body',None)
        constants=(getattr(body,'constants',None)
                   if body is not None else None)
        if constants is None:
            constants=getattr(declaration,'constants',None)
        return [constant.name for constant in constants or []]

    def emit_nested_enum(self,declaration,owner_name,o):
        constants=self.enum_constant_names(declaration)
        enum_name=declaration.name
        enum_base=f'::jxx::lang::Enum<{enum_name}>'
        o.w(f'class {enum_name} final : public {enum_base} {{')
        o.w('public:');o.enter()
        o.w(f'using JxxSuper = {enum_base};')
        o.w(f'explicit {enum_name}(')
        o.w('    const ::jxx::Ptr<::jxx::lang::String>& name,')
        o.w('    ::jxx::lang::jint ordinal)')
        o.w(f'    : JxxSuper(name, ordinal) {{}}')
        o.w('static ::jxx::Ptr<::jxx::lang::ClassAny> Class() {')
        o.enter()
        o.w(f'return ::jxx::lang::ClassInfo<{enum_name}, JxxSuper>::Class();')
        o.exit();o.w('}')
        o.w(f'static ::jxx::Ptr<::jxx::lang::JxxArray<')
        o.w(f'    ::jxx::Ptr<{enum_name}>, 1U>> values() {{')
        o.enter()
        o.w(f'auto result = ::jxx::NEW<::jxx::lang::JxxArray<')
        o.w(f'    ::jxx::Ptr<{enum_name}>, 1U>>(')
        o.w(f'        static_cast<::jxx::lang::jint>({len(constants)}));')
        for ordinal,constant in enumerate(constants):
            o.w(f'(*result)[{ordinal}] = {constant};')
        o.w('return result;')
        o.exit();o.w('}')
        o.w(f'static ::jxx::Ptr<{enum_name}> valueOf(')
        o.w('    const ::jxx::Ptr<::jxx::lang::String>& name) {')
        o.enter()
        o.w(f'return JxxSuper::valueOf(Class(), name);')
        o.exit();o.w('}')
        for constant in constants:
            o.w(f'inline static const ::jxx::Ptr<{enum_name}> {constant} =')
            o.enter()
            o.w(f'::jxx::NEW<{enum_name}>(')
            o.enter()
            o.w(f'::jxx::NEW<::jxx::lang::String>("{constant}"),')
            o.w(f'static_cast<::jxx::lang::jint>(')
            o.w(f'    {constants.index(constant)}));')
            o.exit();o.exit()
        o.exit();o.w('};')

    def emit_nested_enum_definitions(self,declaration,owner_name,o):
        # Constants are inline static members so nested enum definitions remain
        # header-only and usable by every converted translation unit.
        (void := owner_name)
        (void := declaration)

    def emit_nested_class(self, declaration, owner_name,
                          owner_type_parameters, o):
        names=self.type_parameter_names(declaration)
        previous=set(self.type_parameters)
        self.type_parameters.update(names)
        template=self.template_line(names)
        if template: o.w(template)
        base,iface_types=self.class_base_and_interfaces(declaration)
        self_type=self.specialized_class_name(declaration.name,names)
        class_args=', '.join([self_type, base] + iface_types)
        o.w(f'class {declaration.name} : public ::jxx::lang::ClassBase<{class_args}>')
        o.w('{')
        # Preserve the source member-class access. It must be declared before
        # outer fields whose types refer to it.
        access='private' if 'private' in (getattr(declaration,'modifiers',None) or set()) else 'public'
        o.w(f'{access}:'); o.enter()
        # Source enclosing classes may access private members of their nested
        # classes. C++ does not grant that access automatically, so declare the
        # exact enclosing specialization as a friend while preserving the
        # nested class's source-private API.
        owner_specialization=self.specialized_class_name(
            owner_name,owner_type_parameters)
        o.w(f'friend class {owner_specialization};')
        o.w(f'using JxxSuper = {base};')
        o.w(f'using Super = ::jxx::lang::ClassBase<{class_args}>;')
        nested_members=list(getattr(declaration,'body',None) or [])
        # jxx::NEW performs construction in the runtime helper, outside the
        # enclosing class's friend context. A source-private nested class is
        # itself inaccessible to clients, so exposing only its constructors
        # inside that private nested type does not expose a source API. It does
        # allow jxx::NEW to initialize Object ownership metadata correctly.
        constructors=[member for member in nested_members
                      if type(member).__name__=='ConstructorDeclaration']
        non_constructors=[member for member in nested_members
                          if type(member).__name__!='ConstructorDeclaration']
        if constructors:
            o.exit(); o.w('public:'); o.enter()
            for constructor in constructors:
                self.emit_inline_member(
                    constructor,declaration.name,o)
            o.exit(); o.w(f'{access}:'); o.enter()
        for member in non_constructors:
            kind=type(member).__name__
            if kind=='FieldDeclaration':
                for d in member.declarators:
                    typ=self.resolve(member.type).value()
                    init=' = nullptr' if typ.startswith('::jxx::Ptr<') else ''
                    o.w(f'{typ} {d.name}{init};')
            elif kind=='MethodDeclaration':
                self.emit_inline_member(member,declaration.name,o)
        o.exit(); o.w('};')
        self.type_parameters=previous

    def method_wildcard_arguments(self,method):
        return [argument
                for parameter in getattr(method,'parameters',None) or []
                for argument in getattr(parameter.type,'arguments',None) or []
                if getattr(argument,'type',None) is None]

    def method_wildcard_type_names(self,method)->List[str]:
        return [f'JxxWildcard{index}'
                for index,_ in enumerate(self.method_wildcard_arguments(method))]

    def activate_method_wildcards(self,method):
        arguments=self.method_wildcard_arguments(method)
        names=self.method_wildcard_type_names(method)
        previous=dict(self.active_wildcard_types)
        self.active_wildcard_types.update(
            {id(argument):name for argument,name in zip(arguments,names)})
        return previous,names

    def method_type_parameter_names(self, method)->List[str]:
        return (self.type_parameter_names(method) +
                self.method_wildcard_type_names(method))

    def emit_template_method(self, method, owner_name, o):
        wildcard_previous,wildcard_names=self.activate_method_wildcards(method)
        names=self.type_parameter_names(method)+wildcard_names
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
        self.active_wildcard_types=wildcard_previous

    def class_base_and_interfaces(self, declaration):
        extends=getattr(declaration,'extends',None)
        implemented=list(getattr(declaration,'implements',None) or [])
        # Some accepted tutorial inputs use `implements X` even though X is a
        # source class. Treat a known source class as the superclass when no
        # explicit extends clause exists. Otherwise ClassBase receives that
        # class as an interface and creates a second Object inheritance path.
        if extends is None:
            class_implements=[item for item in implemented
                              if self.source_type_kinds.get(
                                  self.name_of(item))=='class']
            if len(class_implements)==1:
                extends=class_implements[0]
                implemented=[item for item in implemented
                             if item is not extends]
                self.diag.append(
                    f'{declaration.name}: treating implemented source class '
                    f'{self.name_of(extends)} as the superclass')
        base=(self.ref_raw(extends) if extends is not None
              else '::jxx::lang::Object')
        interfaces=[self.ref_raw(item) for item in implemented]
        return base,interfaces

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
            base,iface_types=self.class_base_and_interfaces(t)
            self_type=self.specialized_class_name(t.name,outer_parameters)
            class_args=', '.join([self_type, base] + iface_types)
            o.w(f'class {t.name} : public ::jxx::lang::ClassBase<{class_args}>')
            o.w('{');o.w('public:');o.enter();o.w(f'using JxxSuper = {base};')
            o.w(f'using Super = ::jxx::lang::ClassBase<{class_args}>;')
            o.w(f'using JxxClassInfoMarker = ::jxx::lang::ClassInfo<{class_args}>;')
            # Emit member classes before fields and methods so types such as
            # node<T> are complete and visible at their first use.
            for nested in t.body or []:
                nested_kind=type(nested).__name__
                if nested_kind=='ClassDeclaration':
                    self.emit_nested_class(
                        nested,t.name,outer_parameters,o)
                elif nested_kind=='EnumDeclaration':
                    self.emit_nested_enum(nested,t.name,o)
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
                        virt='virtual ' if n=='MethodDeclaration' else ''
                        pure=(' = 0' if n=='MethodDeclaration' and
                              ('abstract' in mods or m.body is None) else '')
                        o.w(f'{virt}{self.sig(m,t.name)}{pure};')
            o.exit();o.w('};');o.w()
            for nested in t.body or []:
                if type(nested).__name__=='EnumDeclaration':
                    self.emit_nested_enum_definitions(nested,t.name,o)
            self.type_parameters=set()
        if ns:o.exit();o.w(f'}} // namespace {ns}')
        return o.text()

    @staticmethod
    def is_this_expression(expression):
        return type(expression).__name__=='This'

    def this_ptr_expression(self, target_type=None):
        shared='this->thisPtr()'
        if target_type is None:
            return shared
        target_name=self.name_of(target_type)
        if target_name in PRIMITIVES:
            return shared
        return f'::jxx::CAST<{self.ref_raw(target_type)}>({shared})'

    def constructor_argument_expression(self, expression, target_type):
        if self.is_this_expression(expression):
            return self.this_ptr_expression(target_type)
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
        o=Emit();o.w(f'#include "{header_inc}"')
        for include in sorted(self.source_includes):
            if include != header_inc:
                o.w(f'#include "{include}"')
        o.w();ns=('jxx::'+self.package.replace('.', '::')) if self.package else ''
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

    @staticmethod
    def generated_header_main_entries(header_text: str) -> Dict[str,str]:
        """Find classes that actually declare generated jxxMain in the header."""
        entries:Dict[str,str]={}
        class_pattern=re.compile(r'\bclass\s+([A-Za-z_$][\w$]*)[^;{]*\{')
        signature_pattern=re.compile(
            r'\bstatic\s+void\s+jxxMain\s*\([^)]*&\s*'
            r'([A-Za-z_$][\w$]*)\s*\)')
        for match in class_pattern.finditer(header_text):
            depth=1
            index=match.end()
            while index < len(header_text) and depth:
                if header_text[index]=='{':
                    depth+=1
                elif header_text[index]=='}':
                    depth-=1
                index+=1
            body=header_text[match.end():max(match.end(),index-1)]
            signature=signature_pattern.search(body)
            if signature:
                entries[match.group(1)]=signature.group(1)
        return entries

    def main_source(self, owner_name, parameter_name, header_inc,
                    owner_type_parameters:Sequence[str]=())->str:
        o=Emit()
        o.w(f'#include "{header_inc}"')
        o.w('#include <cstdint>')
        o.w('#include "jxx.initialize.h"')
        o.w('#include "jxx.client.initialize.h"')
        o.w('#include "lang/jxx.lang.String.h"')
        o.w('#include "lang/jxx.lang.Throwable.h"')
        o.w('#include "lang/jxx.lang.System.h"')
        o.w()
        ns=('jxx::'+self.package.replace('.', '::')) if self.package else ''
        owner_specialization=owner_name
        if owner_type_parameters:
            default_type='::jxx::lang::Object'
            owner_specialization=(owner_name + '<' +
                ', '.join(default_type for _ in owner_type_parameters) + '>')
        target=(f'::{ns}::{owner_specialization}'
                if ns else owner_specialization)
        o.w('// Native C++ process entry point for this converted source main.')
        o.w('int main(int argc, char** argv) {')
        o.enter();o.w('try {');o.enter()
        o.w('::jxx::initialize();')
        o.w('::jxx::client::initialize();')
        o.w('using JxxString = ::jxx::lang::String;')
        o.w('using JxxStringArray = ::jxx::lang::JxxArray<::jxx::Ptr<JxxString>, 1U>;')
        o.w('const auto sourceArgc = argc > 0 ? argc - 1 : 0;')
        o.w(f'auto {parameter_name} = ::jxx::NEW<JxxStringArray>(')
        o.enter();o.w('static_cast<std::uint32_t>(sourceArgc));');o.exit()
        o.w('for (int index = 1; index < argc; ++index) {');o.enter()
        o.w(f'(*{parameter_name})[static_cast<::jxx::lang::jint>(index - 1)] =')
        o.enter();o.w('::jxx::NEW<JxxString>(argv[index]);');o.exit()
        o.exit();o.w('}')
        o.w(f'{target}::jxxMain({parameter_name});')
        o.w('return 0;');o.exit()
        o.w('} catch (const ::jxx::lang::Throwable& throwable) {');o.enter()
        o.w('throwable.printStackTrace();');o.w('return 1;');o.exit();o.w('}')
        o.exit();o.w('}')
        return o.text()

    def translate(self,src,out_root,stem,source_relative_dir:Optional[Path]=None):
        tree=javalang.parse.parse(src)
        # Bind wrappers to the class that actually owns main. The earlier
        # source-only brace scan could associate a later main with the source
        # filename's first class, producing Final_method_main.cpp that called
        # Final_method::jxxMain even though test1 owns main.
        self.main_entries=self.ast_main_entries(tree)
        # Parsing succeeded, so the AST is authoritative. Do not fall back to
        # the filename-oriented source scanner, which can associate test1.main
        # with an earlier class such as Final_method.
        self.main_owner_names=set(self.main_entries)
        package=tree.package.name if tree.package else ''
        self.current_output_relative_dir=(
            Path(*package.split('.')) if package else
            Path(source_relative_dir or Path()))
        self.gather(tree)
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
        # Resolve declaration-level dependencies first. These types must be
        # complete while the generated header is parsed (bases, interfaces,
        # fields, returns, and parameters).
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
        header_includes=set(self.includes)
        # Discover method-body dependencies after declaration dependencies are
        # known. Non-template class bodies are emitted in the .cpp and therefore
        # keep body-only includes there. Source generic classes are header-only,
        # so every type used by their inline bodies must be complete in the .h.
        self.gather_body_type_includes(tree)
        body_includes=self.includes-header_includes
        has_header_only_generic=any(
            self.type_parameter_names(declaration)
            for declaration in self.class_declarations(tree))
        if has_header_only_generic:
            header_includes.update(body_includes)
            self.source_includes=set()
        else:
            self.source_includes=body_includes
        self.includes=header_includes
        generated_header=self.header(tree)
        hp.write_text(generated_header,encoding='utf-8')
        # The generated header is the final authority for wrapper ownership.
        # A native wrapper must call the class that actually declares jxxMain,
        # never the source basename or the first class in the compilation unit.
        emitted_main_entries=self.generated_header_main_entries(generated_header)
        if emitted_main_entries != self.main_entries:
            self.diag.append(
                'main owner corrected from parsed mapping '
                f'{self.main_entries} to generated declaration '
                f'{emitted_main_entries}')
        self.main_entries=emitted_main_entries
        self.main_owner_names=set(self.main_entries)
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
            # Preserve the source basename for the single executable wrapper
            # while calling the class that actually owns main. For a source
            # Final_method.java whose package-private test1 owns main, this is
            # Final_method_main.cpp calling test1::jxxMain.
            wrapper_name=(f'{stem}_main.cpp' if len(self.main_entries)==1
                          else f'{owner_name}_main.cpp')
            wrapper=output_dir/wrapper_name
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


def normalize_cmake_path(path_value: str) -> str:
    """Normalize a JXX include path for generated CMake files.

    Native Windows drive paths are preserved without passing through
    pathlib.Path.resolve(), which would incorrectly treat them as relative
    paths when the converter runs under WSL/Linux. WSL /mnt/<drive>/ paths
    are converted to Visual Studio drive paths. Other paths retain the
    existing expanduser/resolve behavior.
    """
    path = os.fspath(path_value).strip()

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
                         jxx_include: Optional[str] = None,
                         jxx_library: Optional[str] = None,
                         jxx_root: Optional[str] = None) -> Path:
    root = Path(out_root)
    jxx_include_path = (
        normalize_cmake_path(jxx_include)
        if jxx_include else None)
    jxx_library_path = normalize_cmake_path(jxx_library) if jxx_library else None
    jxx_root_path = normalize_cmake_path(jxx_root) if jxx_root else None
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
    if jxx_root_path:
        root_lines += [
            '# Build JXX through its own CMake project so all third-party',
            '# target dependencies are preserved transitively. Application',
            '# projects must not configure the JXX unit-test subtree.',
            'set(BUILD_TESTING OFF CACHE BOOL "Disable dependency tests" FORCE)',
            'set(JXX_BUILD_TESTS OFF CACHE BOOL "Disable JXX tests" FORCE)',
            f'add_subdirectory("{jxx_root_path}"',
            '                 "${CMAKE_BINARY_DIR}/jxx"',
            '                 EXCLUDE_FROM_ALL)',
            'if(NOT TARGET jxxsdk)',
            '    message(FATAL_ERROR "JXX root did not define target jxxsdk")',
            'endif()',
            'add_library(jxx_sdk ALIAS jxxsdk)',
            ''
        ]
    elif jxx_library_path:
        root_lines += ['add_library(jxx_sdk STATIC IMPORTED GLOBAL)',
                       'set_target_properties(jxx_sdk PROPERTIES',
                       f'    IMPORTED_LOCATION "{jxx_library_path}"']
        if jxx_include_path:
            root_lines.append(f'    INTERFACE_INCLUDE_DIRECTORIES "{jxx_include_path}"')
        root_lines += [')', '']

    if support:
        root_lines.append(f'add_library({support_target} STATIC')
        root_lines.extend(f'    {x.relative_to(root).as_posix()}' for x in support)
        root_lines.append(')')
        root_lines.append(f'target_include_directories({support_target} PUBLIC')
        root_lines.append('    ${CMAKE_CURRENT_SOURCE_DIR}')
        if jxx_include_path:
            root_lines.append(f'    "{jxx_include_path}"')
        root_lines += [')', '']
        if jxx_root_path or jxx_library_path:
            root_lines += [f'target_link_libraries({support_target} PUBLIC jxx_sdk)', '']

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
                   jxx_include: Optional[str] = None,
                   jxx_library: Optional[str] = None,
                   jxx_root: Optional[str] = None) -> str:
    return generate_cmake_files(
        out_root, project, target, jxx_include, jxx_library,
        jxx_root).read_text(encoding='utf-8')

def collect_client_initialization_types(files:Sequence[Path], source_root:Path) -> List[Tuple[str,str]]:
    result=[]; seen=set()
    for path in files:
        try:
            tree=javalang.parse.parse(path.read_text(encoding='utf-8'))
        except Exception:
            continue
        package=tree.package.name if tree.package else ''
        if package:
            relative_dir=Path(*package.split('.')); namespace='::jxx::'+package.replace('.','::')
        else:
            try: relative_dir=path.parent.resolve().relative_to(source_root.resolve())
            except ValueError: relative_dir=Path()
            namespace='::'
        include=(relative_dir/path.with_suffix('.h').name).as_posix()
        if include.startswith('./'): include=include[2:]
        for declaration in getattr(tree,'types',None) or []:
            if type(declaration).__name__ not in {'ClassDeclaration','InterfaceDeclaration'}: continue
            if getattr(declaration,'type_parameters',None): continue
            qualified=(f'{namespace}::{declaration.name}' if namespace!='::' else f'::{declaration.name}')
            if qualified not in seen:
                seen.add(qualified); result.append((qualified,include))
    return sorted(result,key=lambda value:value[0])

def generate_client_initializer(out_root:str, client_types:Sequence[Tuple[str,str]]) -> Tuple[Path,Path]:
    root=Path(out_root); header=root/'jxx.client.initialize.h'; source=root/'jxx.client.initialize.cpp'
    header.write_text('#pragma once\n\nnamespace jxx::client {\n    void initialize();\n} // namespace jxx::client\n',encoding='utf-8')
    lines=['#include "jxx.client.initialize.h"']
    lines.extend(f'#include "{include}"' for include in sorted({include for _,include in client_types}))
    lines += ['','namespace jxx::client {','    void initialize() {','        static const bool initialized = []() {']
    lines.extend(f'            (void){qualified}::Class();' for qualified,_ in client_types)
    lines += ['            return true;','        }();','        (void)initialized;','    }','} // namespace jxx::client','']
    source.write_text('\n'.join(lines),encoding='utf-8')
    return header,source

def build_source_type_index(
        files:Sequence[Path],
        source_root:Optional[Path]=None) -> Dict[str,Tuple[str,str]]:
    """Map source class names to generated qualified types and headers."""
    index:Dict[str,Tuple[str,str]]={}
    package_pattern=re.compile(r'\bpackage\s+([A-Za-z_$][\w$]*(?:\.[A-Za-z_$][\w$]*)*)\s*;')
    type_pattern=re.compile(r'\b(?:class|interface|enum)\s+([A-Za-z_$][\w$]*)')
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
        namespace=('::jxx::'+package.replace('.','::') if package else '::')
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
        for match in type_pattern.finditer(index_source):
            name=match.group(1)
            qualified=(f'{namespace}::{name}' if namespace != '::' else f'::{name}')
            value=(qualified,include)
            qualified_name=f'{package}.{name}' if package else name
            index[qualified_name]=value
            index[name]=value
    return index

def build_source_type_kinds(files:Sequence[Path]) -> Dict[str,str]:
    kinds:Dict[str,str]={}
    package_pattern=re.compile(
        r'\bpackage\s+([A-Za-z_$][\w$]*(?:\.[A-Za-z_$][\w$]*)*)\s*;')
    type_pattern=re.compile(
        r'\b(class|interface|enum)\s+([A-Za-z_$][\w$]*)')
    for path in files:
        try:
            source=path.read_text(encoding='utf-8')
        except OSError:
            continue
        source=re.sub(r'/\*.*?\*/|//[^\r\n]*','',source,flags=re.DOTALL)
        package_match=package_pattern.search(source)
        package=package_match.group(1) if package_match else ''
        for match in type_pattern.finditer(source):
            kind,name=match.groups()
            kinds[name]=kind
            if package:
                kinds[f'{package}.{name}']=kind
    return kinds

def translate_file(path:Path,out_root:str,
                   source_type_index:Optional[Dict[str,Tuple[str,str]]]=None,
                   source_root:Optional[Path]=None,
                   source_type_kinds:Optional[Dict[str,str]]=None,
                   sdk_metadata:Optional[SdkMetadataIndex]=None):
    src=path.read_text(encoding='utf-8')
    t=Translator()
    t.source_type_index=dict(source_type_index or {})
    t.source_type_kinds=dict(source_type_kinds or {})
    t.sdk_metadata=sdk_metadata
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
    warning_count=len(t.diag)
    for diagnostic in t.diag:
        print(f'{path}: WARNING: {diagnostic}',file=sys.stderr)
    hard_errors=list(t.errors)
    # Defense in depth: no generated TODO marker is permitted to count as a
    # successful conversion, even if a future emission path forgets to append
    # a structured Translator error.
    for generated in t.generated_files:
        try:
            generated_text=generated.read_text(encoding='utf-8')
        except OSError as error:
            hard_errors.append(f'cannot validate generated file {generated}: {error}')
            continue
        todo_lines=[line.strip() for line in generated_text.splitlines()
                    if 'TODO' in line]
        for todo in todo_lines:
            message=f'{generated}: generated unresolved marker: {todo}'
            if message not in hard_errors:
                hard_errors.append(message)
    if hard_errors:
        for diagnostic in hard_errors:
            print(f'{path}: ERROR: {diagnostic}',file=sys.stderr)
        raise ConversionFailure(
            f'conversion produced {len(hard_errors)} unresolved error(s)',
            warning_count)
    return warning_count

def print_conversion_summary(started_at,parsed,successes,errors,warnings):
    elapsed=time.perf_counter()-started_at
    print()
    print('Conversion summary')
    print('------------------')
    print(f'Overall time:       {elapsed:.3f} seconds')
    print(f'Total files parsed: {parsed}')
    print(f'Total successes:    {successes}')
    print(f'Total errors:       {errors}')
    print(f'Total warnings:     {warnings}')


def main():
    started_at=time.perf_counter()
    parsed=successes=errors=warnings=0
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
    ap.add_argument('--jxx-library', help=('Compiled JXX SDK library file; accepts Windows, WSL, or native POSIX paths.'))
    ap.add_argument('--jxx-root', help=('JXX project root containing CMakeLists.txt, sdk/, and 3rdparty/. Generated CMake adds this project as a subdirectory so jxxsdk retains all transitive dependencies.'))
    a=ap.parse_args()
    if a.jxx_root and a.jxx_library:
        ap.error('--jxx-root and --jxx-library are mutually exclusive')
    if javalang is None: raise SystemExit('Install dependency: pip install javalang')
    if bool(a.input)==bool(a.src_dir): ap.error('provide either input or --dir')
    sdk_metadata=None
    try:
        if a.jxx_include:
            sdk_metadata=SdkMetadataIndex.scan(a.jxx_include)
            print(f'JXX SDK metadata: scanned {sdk_metadata.headers_scanned} headers; '
                  f'indexed {sdk_metadata.declarations_scanned} method declarations')
        if a.src_dir:
            source_root=Path(a.src_dir)
            files=sorted(source_root.rglob('*.java'))
            if not files:
                raise RuntimeError(f'No .java files found under {source_root}')
            source_type_index=build_source_type_index(files,source_root)
            source_type_kinds=build_source_type_kinds(files)
        else:
            source_root=Path(a.input).parent
            files=[Path(a.input)]
            sibling_files=sorted(source_root.glob('*.java'))
            source_type_index=build_source_type_index(
                sibling_files,source_root)
            source_type_kinds=build_source_type_kinds(sibling_files)

        for path in files:
            parsed+=1
            try:
                warnings+=translate_file(
                    path,a.out,source_type_index,source_root,
                    source_type_kinds,sdk_metadata)
                successes+=1
            except Exception as error:
                errors+=1
                warnings+=getattr(error,'warnings',0)
                print(f'{path}: ERROR: {type(error).__name__}: {error}',
                      file=sys.stderr)

        if successes:
            client_types=collect_client_initialization_types(files,source_root)
            client_header,client_source=generate_client_initializer(a.out,client_types)
            print(f'Wrote {client_header}')
            print(f'Wrote {client_source}')
        if a.cmake:
            try:
                generate_cmake_files(
                    a.out,a.cmake_project,a.cmake_target,a.jxx_include,
                    a.jxx_library,a.jxx_root)
                for generated in sorted(Path(a.out).rglob('CMakeLists.txt')):
                    print(f'Wrote {generated}')
            except Exception as error:
                errors+=1
                print(f'CMake generation: ERROR: {type(error).__name__}: {error}',
                      file=sys.stderr)
    except Exception as error:
        errors+=1
        print(f'Conversion setup: ERROR: {type(error).__name__}: {error}',
              file=sys.stderr)
    finally:
        print_conversion_summary(
            started_at,parsed,successes,errors,warnings)
    return 0 if errors==0 else 1

if __name__=='__main__': sys.exit(main())
