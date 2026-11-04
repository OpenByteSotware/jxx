#!/usr/bin/env python3
"""Fresh, in-memory JXX SDK header metadata scanner.

The scanner intentionally extracts only the information needed by the source
converter: method return category/type and simple overload arity. It scans the
SDK include root on every converter invocation and writes no persistent cache.
"""
from __future__ import annotations

import os
import re
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, List, Optional, Tuple

_JXX_PRIMITIVES = {
    'jbyte': 'byte', 'jshort': 'short', 'jint': 'int', 'jlong': 'long',
    'jfloat': 'float', 'jdouble': 'double', 'jbool': 'boolean',
    'jboolean': 'boolean', 'jchar': 'char',
}


@dataclass(frozen=True)
class TypeMetadata:
    source_name: str
    simple_name: str
    cpp_type: str
    include_path: str
    package_name: str

@dataclass(frozen=True)
class MethodMetadata:
    owner: str
    name: str
    return_type: str
    return_kind: str       # primitive, reference, void, value
    primitive_name: str = ''
    pointee: str = ''
    parameter_count: int = -1
    is_static: bool = False

def normalize_sdk_include_root(include_root) -> Path:
    """Resolve Windows, WSL, and native POSIX SDK include paths."""
    raw = os.fspath(include_root).strip()
    # When running under WSL/POSIX, translate C:/... or C:\\... to
    # /mnt/c/... before pathlib checks the directory.
    windows_match = re.match(r'^([A-Za-z]):[\\/](.*)$', raw)
    if windows_match and os.name != 'nt':
        drive = windows_match.group(1).lower()
        remainder = windows_match.group(2).replace('\\', '/')
        wsl_path = Path('/mnt') / drive / Path(remainder)
        if wsl_path.is_dir():
            return wsl_path
    return Path(raw).expanduser()


class SdkMetadataIndex:
    def __init__(self) -> None:
        self.methods: Dict[str, Dict[str, List[MethodMetadata]]] = {}
        self.types: Dict[str, TypeMetadata] = {}
        self.types_by_simple: Dict[str, List[TypeMetadata]] = {}
        self.types_by_package: Dict[str, Dict[str, TypeMetadata]] = {}
        self.headers_scanned = 0
        self.declarations_scanned = 0
        self.types_scanned = 0
        self.include_root: Optional[Path] = None

    @classmethod
    def scan(cls, include_root: Path) -> 'SdkMetadataIndex':
        index = cls()
        root = normalize_sdk_include_root(include_root)
        if not root.is_dir():
            raise ValueError(
                f'JXX SDK include root does not exist: {include_root} '
                f'(resolved as {root})')
        index.include_root = root
        for header in sorted(root.rglob('*.h')):
            index._scan_header(header)
        return index

    @staticmethod
    def _source_package_from_include(include_path: str) -> Optional[str]:
        path=Path(include_path)
        name=path.name
        match=re.match(r'^jxx\.(.+)\.([A-Za-z_]\w*)\.h$',name)
        if not match:
            return None
        mapped_package=match.group(1)
        # JXX ext is the package-directory mapping for source javax.* APIs.
        if mapped_package=='ext':
            return 'javax'
        if mapped_package.startswith('ext.'):
            return 'javax.'+mapped_package[len('ext.'):]
        return 'java.'+mapped_package

    def _index_type_header(self, path: Path) -> None:
        if self.include_root is None:
            return
        include_path=path.relative_to(self.include_root).as_posix()
        package=self._source_package_from_include(include_path)
        if package is None:
            return
        filename=path.name
        simple=filename[:-2].split('.')[-1]
        # Headers following public package naming are authoritative. Internal
        # helpers normally use internal package paths and therefore remain
        # addressable only through their exact source-style package.
        mapped=package
        if mapped.startswith('javax.'):
            cpp_package='ext::'+mapped[len('javax.'):].replace('.','::')
        elif mapped=='javax':
            cpp_package='ext'
        else:
            cpp_package=mapped[len('java.'):].replace('.','::')
        cpp_type=f'::jxx::{cpp_package}::{simple}'
        source_name=f'{package}.{simple}'
        metadata=TypeMetadata(source_name,simple,cpp_type,include_path,package)
        self.types[source_name]=metadata
        self.types_by_simple.setdefault(simple,[]).append(metadata)
        self.types_by_package.setdefault(package,{})[simple]=metadata
        self.types_scanned+=1

    def resolve_type(self, source_name: str) -> Optional[TypeMetadata]:
        return self.types.get(source_name)

    def resolve_implicit_lang(self, simple_name: str) -> Optional[TypeMetadata]:
        return self.types_by_package.get('java.lang',{}).get(simple_name)

    def resolve_wildcard_type(self, packages, simple_name: str) \
            -> Tuple[Optional[TypeMetadata], List[TypeMetadata]]:
        matches=[self.types_by_package.get(package,{}).get(simple_name)
                 for package in packages]
        matches=[entry for entry in matches if entry is not None]
        return (matches[0] if len(matches)==1 else None, matches)

    def lookup(self, owner: str, method: str,
               argument_count: Optional[int] = None) -> Optional[MethodMetadata]:
        keys = [owner, owner.split('::')[-1], owner.split('.')[-1]]
        candidates: List[MethodMetadata] = []
        for key in keys:
            candidates.extend(self.methods.get(key, {}).get(method, ()))
            if candidates:
                break
        if not candidates:
            return None
        if argument_count is not None:
            exact = [entry for entry in candidates
                     if entry.parameter_count == argument_count]
            if len(exact) == 1:
                return exact[0]
            if exact:
                candidates = exact
        categories = {(entry.return_kind, entry.primitive_name, entry.pointee)
                      for entry in candidates}
        return candidates[0] if len(categories) == 1 else None

    def _add(self, metadata: MethodMetadata, qualified_owner: str) -> None:
        for key in {metadata.owner, qualified_owner}:
            self.methods.setdefault(key, {}).setdefault(
                metadata.name, []).append(metadata)
        self.declarations_scanned += 1

    def _scan_header(self, path: Path) -> None:
        try:
            text = path.read_text(encoding='utf-8', errors='replace')
        except OSError:
            return
        self.headers_scanned += 1
        self._index_type_header(path)
        text = re.sub(r'/\*.*?\*/|//[^\r\n]*', ' ', text, flags=re.DOTALL)
        namespace = self._namespace(text)
        class_pattern = re.compile(
            r'\b(?:class|struct)\s+(?:\w+\s+)*([A-Za-z_]\w*)'
            r'(?:\s+final)?(?:\s*:[^{]+)?\s*\{')
        for match in class_pattern.finditer(text):
            owner = match.group(1)
            body = self._balanced_body(text, match.end() - 1)
            if body is None:
                continue
            qualified = f'{namespace}::{owner}' if namespace else owner
            for statement in self._statements(body):
                metadata = self._parse_method(owner, statement)
                if metadata is not None:
                    self._add(metadata, qualified)

    @staticmethod
    def _namespace(text: str) -> str:
        match = re.search(r'\bnamespace\s+([A-Za-z_]\w*(?:::\w+)*)\s*\{', text)
        return match.group(1) if match else ''

    @staticmethod
    def _balanced_body(text: str, opening: int) -> Optional[str]:
        depth = 0
        for index in range(opening, len(text)):
            char = text[index]
            if char == '{':
                depth += 1
            elif char == '}':
                depth -= 1
                if depth == 0:
                    return text[opening + 1:index]
        return None

    @staticmethod
    def _statements(body: str):
        start = 0
        paren = angle = bracket = brace = 0
        for index, char in enumerate(body):
            if char == '(': paren += 1
            elif char == ')': paren = max(0, paren - 1)
            elif char == '<': angle += 1
            elif char == '>': angle = max(0, angle - 1)
            elif char == '[': bracket += 1
            elif char == ']': bracket = max(0, bracket - 1)
            elif char == '{': brace += 1
            elif char == '}': brace = max(0, brace - 1)
            elif char == ';' and paren == angle == bracket == brace == 0:
                statement = ' '.join(body[start:index].split())
                start = index + 1
                if '(' in statement and ')' in statement:
                    yield statement

    @staticmethod
    def _parameter_count(parameters: str) -> int:
        text = parameters.strip()
        if not text or text == 'void':
            return 0
        depth = 0
        count = 1
        for char in text:
            if char in '<([': depth += 1
            elif char in '>)]': depth = max(0, depth - 1)
            elif char == ',' and depth == 0: count += 1
        return count

    @staticmethod
    def _classify(return_type: str):
        compact = ' '.join(return_type.split())
        if compact == 'void':
            return 'void', '', ''
        primitive_match = re.search(r'(?:::)?jxx::lang::(j\w+)\b', compact)
        if primitive_match and primitive_match.group(1) in _JXX_PRIMITIVES:
            return 'primitive', _JXX_PRIMITIVES[primitive_match.group(1)], ''
        ptr_match = re.search(r'(?:::)?jxx::Ptr\s*<\s*(.+)\s*>', compact)
        if ptr_match:
            return 'reference', '', ptr_match.group(1).strip()
        return 'value', '', ''

    @classmethod
    def _parse_method(cls, owner: str, statement: str) -> Optional[MethodMetadata]:
        if statement.startswith(('using ', 'typedef ', 'static_assert')):
            return None
        # Remove access labels and common declaration modifiers.
        cleaned = re.sub(r'^(?:public|protected|private)\s*:\s*', '', statement)
        cleaned = re.sub(r'\b(?:virtual|inline|constexpr|explicit|friend)\b', ' ', cleaned)
        match = re.match(
            r'\s*(?P<static>static\s+)?(?P<return>.+?)\s+'
            r'(?P<name>[A-Za-z_]\w*)\s*\((?P<params>.*)\)'
            r'(?:\s+const)?(?:\s+noexcept)?(?:\s+override)?(?:\s*=\s*0)?\s*$',
            ' '.join(cleaned.split()))
        if not match:
            return None
        name = match.group('name')
        if name == owner or name.startswith('~'):
            return None
        return_type = match.group('return').strip()
        kind, primitive, pointee = cls._classify(return_type)
        return MethodMetadata(
            owner=owner, name=name, return_type=return_type,
            return_kind=kind, primitive_name=primitive, pointee=pointee,
            parameter_count=cls._parameter_count(match.group('params')),
            is_static=bool(match.group('static')))
