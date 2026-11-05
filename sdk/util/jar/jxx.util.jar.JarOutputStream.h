#pragma once
#include "util/zip/jxx.util.zip.ZipOutputStream.h"
namespace jxx::util::jar {class Manifest;class JarOutputStream:public ::jxx::lang::ClassBase<JarOutputStream,::jxx::util::zip::ZipOutputStream>{public:using JxxSuper=::jxx::util::zip::ZipOutputStream;using Super=::jxx::lang::ClassBase<JarOutputStream,JxxSuper>;explicit JarOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>&);JarOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>&,const ::jxx::Ptr<Manifest>&);void putNextEntry(const ::jxx::Ptr<::jxx::util::zip::ZipEntry>&);};}
