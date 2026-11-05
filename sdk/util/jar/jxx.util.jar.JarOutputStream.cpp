#include "util/jar/jxx.util.jar.JarOutputStream.h"
#include "util/jar/jxx.util.jar.Manifest.h"
namespace jxx::util::jar {JarOutputStream::JarOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>&o):Super(o){}JarOutputStream::JarOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>&o,const ::jxx::Ptr<Manifest>&):Super(o){}void JarOutputStream::putNextEntry(const ::jxx::Ptr<::jxx::util::zip::ZipEntry>&e){JxxSuper::putNextEntry(e);}}
