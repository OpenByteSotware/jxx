#include "util/jar/jxx.util.jar.JarFile.h"
#include "util/jar/jxx.util.jar.Manifest.h"
#include "util/jar/internal/jxx.util.jar.internal.JarEntryEnumeration.h"
namespace jxx::util::jar {::jxx::Ptr<::jxx::lang::String> JarFile::MANIFEST_NAME=::jxx::NEW<::jxx::lang::String>("META-INF/MANIFEST.MF");JarFile::JarFile(const ::jxx::Ptr<::jxx::lang::String>&n):Super(n){}JarFile::JarFile(const ::jxx::Ptr<::jxx::io::File>&f):Super(f){}JarFile::JarFile(const ::jxx::Ptr<::jxx::io::File>&f,::jxx::lang::jbool):Super(f){}JarFile::JarFile(const ::jxx::Ptr<::jxx::io::File>&f,::jxx::lang::jbool,::jxx::lang::jint mode):Super(f,mode){}::jxx::Ptr<Manifest> JarFile::getManifest(){auto e=JxxSuper::getEntry(MANIFEST_NAME);if(!e)return nullptr;return ::jxx::NEW<Manifest>(JxxSuper::getInputStream(e));}::jxx::Ptr<JarEntry> JarFile::getJarEntry(const ::jxx::Ptr<::jxx::lang::String>&n)const{auto e=JxxSuper::getEntry(n);return e?::jxx::NEW<JarEntry>(e):nullptr;}::jxx::Ptr<::jxx::util::Enumeration<JarEntry>> JarFile::entries() {
    return ::jxx::CAST<::jxx::util::Enumeration<JarEntry>>(
        ::jxx::NEW<::jxx::util::jar::internal::JarEntryEnumeration>(
            JxxSuper::entries()));
}}
