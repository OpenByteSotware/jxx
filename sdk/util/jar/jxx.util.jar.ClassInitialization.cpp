#include "util/jar/jxx.util.jar.ClassInitialization.h"
#include <mutex>
#include "util/jar/jxx.util.jar.Attributes.h"
#include "util/jar/jxx.util.jar.JarEntry.h"
#include "util/jar/jxx.util.jar.JarException.h"
#include "util/jar/jxx.util.jar.JarFile.h"
#include "util/jar/jxx.util.jar.JarInputStream.h"
#include "util/jar/jxx.util.jar.JarOutputStream.h"
#include "util/jar/jxx.util.jar.Manifest.h"
#include "util/jar/jxx.util.jar.Pack200.h"
namespace jxx::util::jar {void initializeClasses(){static std::once_flag f;std::call_once(f,[]{(void)Attributes::Class();(void)Attributes::Name::Class();(void)JarEntry::Class();(void)JarException::Class();(void)JarFile::Class();(void)JarInputStream::Class();(void)JarOutputStream::Class();(void)Manifest::Class();(void)Pack200::Class();(void)Pack200::Packer::Class();(void)Pack200::Unpacker::Class();});}void initialize(){initializeClasses();}}
