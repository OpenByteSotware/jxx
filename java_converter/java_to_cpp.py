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
import argparse, os, sys
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
UNSUPPORTED=('java.lang.annotation','java.lang.instrument','java.lang.invoke','java.lang.reflect')

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
        self.imports:Dict[str,str]={}; self.includes:Set[str]=set(); self.diag:List[str]=[]
        self.package=''; self.current_class=''; self.current_return='void'

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
                Transpiler.name_of(x)
                for x in name
            )

        return str(name)
    def qmap(self,name:str)->Tuple[str,Optional[str]]:
        if name in self.imports: name=self.imports[name]
        if name=='String': name='java.lang.String'
        if name=='Object': name='java.lang.Object'
        if name=='Class': name='java.lang.ClassAny'
        p=name.split('.')
        if len(p)==1:
            return p[0],None
        typ=p[-1]; pkg=p[:-1]
        if pkg[0]=='java': mapped=pkg[1:]; cpp=['jxx']+mapped; prefix='jxx'
        elif pkg[0]=='javax': mapped=['ext']+pkg[1:]; cpp=['jxx']+mapped; prefix='jxx'
        else: mapped=pkg; cpp=pkg; prefix=None
        raw='::'+'::'.join(cpp+[typ])
        dotted='.'.join(mapped); folder='/'.join(mapped)
        fn=(f'{prefix}.{dotted}.{typ}.h' if prefix else f'{".".join(pkg+[typ])}.h')
        return raw,(f'{folder}/{fn}' if folder else fn)

    def ref_raw(self,t)->str:
        name=self.name_of(t); raw,inc=self.qmap(name)
        if inc: self.includes.add(inc)
        args=[]
        for a in getattr(t,'arguments',None) or []:
            at=getattr(a,'type',None)
            if at is None: args.append('::jxx::Ptr<::jxx::lang::Object>')
            else: args.append(self.resolve(at).value())
        if args: raw += '<'+', '.join(args)+'>'
        return raw

    def resolve(self,t,extra_dims=0)->CType:
        if t is None: return CType('void',void=True)
        tn=type(t).__name__; name=self.name_of(t)
        dims=len(getattr(t,'dimensions',None) or [])+extra_dims
        if tn=='BasicType' or name in PRIMITIVES:
            base=CType(PRIMITIVES.get(name,name),primitive=True,void=(name=='void'))
        else: base=CType(self.ref_raw(t))
        if not dims: return base
        elem=base.value() if not base.primitive else base.raw
        for _ in range(dims): elem=f'::jxx::Ptr<::jxx::lang::JxxArray<{elem}>>'
        return CType(elem,primitive=True)

    def gather(self,tree):
        self.package=tree.package.name if tree.package else ''
        for imp in tree.imports or []:
            p=imp.path
            if any(p==x or p.startswith(x+'.') for x in UNSUPPORTED):
                self.diag.append(f'unsupported package: {p}'); continue
            if imp.wildcard: continue
            self.imports[p.split('.')[-1]]=p
            _,inc=self.qmap(p)
            if inc: self.includes.add(inc)
        self.includes.update({'lang/jxx.lang.Object.h','lang/jxx.lang.ClassInfo.h','lang/jxx.lang.JxxArray.h'})

    def sig(self,m,cname,definition=False):
        ctor=type(m).__name__=='ConstructorDeclaration'
        ps=[]
        for p in m.parameters or []:
            extra=(1 if getattr(p,'varargs',False) else 0)+len(getattr(p,'dimensions',None) or [])
            ps.append(f'{self.resolve(p.type,extra).param()} {p.name}')
        qualified=f'{cname}::{m.name}' if definition else m.name
        if ctor: qualified=f'{cname}::{cname}' if definition else cname; return f'{qualified}({", ".join(ps)})'
        ret=self.resolve(m.return_type,len(getattr(m,'dimensions',None) or [])).value()
        return f'{ret} {qualified}({", ".join(ps)})'

    def expr(self,e):
        if e is None:return ''
        n=type(e).__name__
        if n=='Literal': return 'nullptr' if e.value in (None,'null') else e.value
        if n=='This': return 'this'
        if n=='MemberReference':
            q=getattr(e,'qualifier',None); base=f'{q}->{e.member}' if q else e.member
            return ''.join(getattr(e,'prefix_operators',None) or [])+base+''.join(getattr(e,'postfix_operators',None) or [])
        if n=='MethodInvocation':
            q=getattr(e,'qualifier',None); head=f'{q}->{e.member}' if q else e.member
            return f'{head}({", ".join(self.expr(x) for x in e.arguments or [])})'
        if n=='ClassCreator': return f'::jxx::NEW<{self.ref_raw(e.type)}>({", ".join(self.expr(x) for x in e.arguments or [])})'
        if n=='Cast':
            t=self.resolve(e.type); x=self.expr(e.expression)
            return f'static_cast<{t.raw}>({x})' if t.primitive else f'::jxx::CAST<{t.raw}>({x})'
        if n=='BinaryOperation':
            if e.operator=='instanceof': return f'::jxx::INSTANCEOF<{self.ref_raw(e.operandr)}>({self.expr(e.operandl)})'
            return f'({self.expr(e.operandl)} {e.operator} {self.expr(e.operandr)})'
        if n=='Assignment': return f'({self.expr(e.expressionl)} {e.type or "="} {self.expr(e.value)})'
        if n=='TernaryExpression': return f'({self.expr(e.condition)} ? {self.expr(e.if_true)} : {self.expr(e.if_false)})'
        if n=='ArraySelector': return f'[{self.expr(e.index)}]'
        return str(e)

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
            t=self.resolve(s.type)
            for d in s.declarators:
                init=f' = {self.expr(d.initializer)}' if d.initializer is not None else ''
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

    def header(self,tree)->str:
        o=Emit();o.w('#pragma once')
        for x in sorted(self.includes):o.w(f'#include "{x}"')
        o.w(); ns=self.package.replace('.','::')
        if ns:o.w(f'namespace {ns} {{');o.enter()
        for t in tree.types or []:
            iface=type(t).__name__=='InterfaceDeclaration'; self.current_class=t.name
            if iface:
                o.w(f'class {t.name} : public ::jxx::lang::InterfaceBase<{t.name}> {{');o.w('public:');o.enter();o.w(f'virtual ~{t.name}() = default;')
                for m in t.body or []:
                    if type(m).__name__=='MethodDeclaration':o.w(f'virtual {self.sig(m,t.name)} = 0;')
                o.exit();o.w('};');o.w();continue
            ext=getattr(t,'extends',None); base=self.ref_raw(ext) if ext is not None else '::jxx::lang::Object'
            impls=getattr(t,'implements',None) or []
            iface_types=[self.ref_raw(i) for i in impls]
            class_args=', '.join([t.name, base] + iface_types)
            o.w(f'class {t.name} : public ::jxx::lang::ClassBase<{class_args}>')
            o.w('{');o.w('public:');o.enter();o.w(f'using JxxSuper = {base};')
            o.w(f'using Super = ::jxx::lang::ClassBase<{class_args}>;')
            o.w(f'using JxxClassInfoMarker = ::jxx::lang::ClassInfo<{class_args}>;')
            for m in t.body or []:
                n=type(m).__name__
                if n=='FieldDeclaration':
                    mods=m.modifiers or set(); prefix='inline static ' if 'static' in mods else ''
                    for d in m.declarators:
                        typ=self.resolve(m.type).value(); init=f' = {self.expr(d.initializer)}' if d.initializer is not None else (' = nullptr' if typ.startswith('::jxx::Ptr<') else '')
                        o.w(f'{prefix}{typ} {d.name}{init};')
                elif n in ('MethodDeclaration','ConstructorDeclaration'):
                    virt='virtual ' if n=='MethodDeclaration' and ('abstract' in (m.modifiers or set()) or m.body is None) else ''
                    pure=' = 0' if virt else ''
                    o.w(f'{virt}{self.sig(m,t.name)}{pure};')
            o.exit();o.w('};');o.w()
        if ns:o.exit();o.w(f'}} // namespace {ns}')
        return o.text()

    def source(self,tree,header_inc)->str:
        o=Emit();o.w(f'#include "{header_inc}"');o.w();ns=self.package.replace('.','::')
        if ns:o.w(f'namespace {ns} {{');o.enter()
        for t in tree.types or []:
            if type(t).__name__!='ClassDeclaration':continue
            for m in t.body or []:
                if type(m).__name__ not in ('MethodDeclaration','ConstructorDeclaration') or getattr(m,'body',None) is None:continue
                o.w(self.sig(m,t.name,True)+' {');o.enter()
                old=self.current_return
                self.current_return='void' if type(m).__name__=='ConstructorDeclaration' else self.resolve(m.return_type).value()
                for s in m.body or []:self.stmt(s,o)
                self.current_return=old;o.exit();o.w('}');o.w()
        if ns:o.exit();o.w(f'}} // namespace {ns}')
        return o.text()

    def translate(self,src,out_root,stem):
        tree=javalang.parse.parse(src);self.gather(tree)
        pkg=tree.package.name if tree.package else ''; rel=Path(*pkg.split('.')) if pkg else Path()
        hp=Path(out_root)/'include'/rel/f'{stem}.h';cp=Path(out_root)/'src'/rel/f'{stem}.cpp'
        hp.parent.mkdir(parents=True,exist_ok=True);cp.parent.mkdir(parents=True,exist_ok=True)
        # resolve declarations before includes are emitted
        for t in tree.types or []:
            if getattr(t,'extends',None) is not None:self.ref_raw(t.extends)
            for i in getattr(t,'implements',None) or []:self.ref_raw(i)
            for m in t.body or []:
                if hasattr(m,'type'):self.resolve(m.type)
                if hasattr(m,'return_type') and m.return_type is not None:self.resolve(m.return_type)
                for p in getattr(m,'parameters',None) or []:self.resolve(p.type)
        hp.write_text(self.header(tree),encoding='utf-8')
        cp.write_text(self.source(tree,os.path.relpath(hp,cp.parent).replace(os.sep,'/')),encoding='utf-8')
        return hp,cp

def generate_cmake(out_root:str, project:str='TranspiledProject', target:str='transpiled')->str:
    root=Path(out_root)
    sources=sorted(x.relative_to(root).as_posix() for x in (root/'src').rglob('*.cpp'))
    lines=['cmake_minimum_required(VERSION 3.16)',f'project({project} LANGUAGES CXX)','',
           'set(CMAKE_CXX_STANDARD 17)','set(CMAKE_CXX_STANDARD_REQUIRED ON)',
           'set(CMAKE_CXX_EXTENSIONS OFF)','']
    if sources:
        lines.append(f'add_library({target} STATIC')
        lines.extend(f'    {x}' for x in sources)
        lines.append(')')
    else:
        lines += [f'file(GLOB_RECURSE {target}_SOURCES CONFIGURE_DEPENDS src/*.cpp)',
                  f'add_library({target} STATIC ${{{target}_SOURCES}})']
    lines += [f'target_include_directories({target} PUBLIC ${{CMAKE_CURRENT_SOURCE_DIR}}/include)','']
    return '\n'.join(lines)

def translate_file(path:Path,out_root:str):
    src=path.read_text(encoding='utf-8')
    t=Translator(); h,c=t.translate(src,out_root,path.stem)
    print(f'Wrote {h}\nWrote {c}')
    for d in t.diag: print(f'{path}: WARNING: {d}',file=sys.stderr)

def main():
    ap=argparse.ArgumentParser(description='Java 8 to JXX C++17 translator')
    ap.add_argument('input',nargs='?',help='single Java source file')
    ap.add_argument('--dir',dest='src_dir',help='recursively convert every .java file')
    ap.add_argument('--out',required=True,help='mimic output root containing include/ and src/')
    ap.add_argument('--cmake',action='store_true',help='generate CMakeLists.txt in output root')
    ap.add_argument('--cmake-project',default='TranspiledProject')
    ap.add_argument('--cmake-target',default='transpiled')
    a=ap.parse_args()
    if javalang is None: raise SystemExit('Install dependency: pip install javalang')
    if bool(a.input)==bool(a.src_dir): ap.error('provide either input or --dir')
    if a.src_dir:
        source_root=Path(a.src_dir)
        files=sorted(source_root.rglob('*.java'))
        if not files: raise SystemExit(f'No .java files found under {source_root}')
        for path in files: translate_file(path,a.out)
    else:
        translate_file(Path(a.input),a.out)
    if a.cmake:
        cmake=Path(a.out)/'CMakeLists.txt'
        cmake.write_text(generate_cmake(a.out,a.cmake_project,a.cmake_target),encoding='utf-8')
        print(f'Wrote {cmake}')
if __name__=='__main__': main()
