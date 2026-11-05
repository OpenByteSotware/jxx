#pragma once
#include "util/zip/jxx.util.zip.ZipEntry.h"
namespace jxx::security { class CodeSigner; }
namespace jxx::security::cert { class Certificate; }
namespace jxx::util::jar { class Attributes; class JarEntry:public ::jxx::lang::ClassBase<JarEntry,::jxx::util::zip::ZipEntry>{public:using JxxSuper=::jxx::util::zip::ZipEntry;using Super=::jxx::lang::ClassBase<JarEntry,JxxSuper>;explicit JarEntry(const ::jxx::Ptr<::jxx::lang::String>&);explicit JarEntry(const ::jxx::Ptr<::jxx::util::zip::ZipEntry>&);explicit JarEntry(const ::jxx::Ptr<JarEntry>&);::jxx::Ptr<Attributes> getAttributes()const;::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::security::cert::Certificate>,1U>> getCertificates()const;::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::security::CodeSigner>,1U>> getCodeSigners()const;};}
