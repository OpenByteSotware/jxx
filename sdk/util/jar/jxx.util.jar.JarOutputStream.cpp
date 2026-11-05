#include "util/jar/jxx.util.jar.JarOutputStream.h"

#include "lang/jxx.lang.NullPointerException.h"
#include "util/jar/jxx.util.jar.JarEntry.h"
#include "util/jar/jxx.util.jar.Manifest.h"
#include "io/jxx.io.ByteArrayOutputStream.h"

namespace jxx::util::jar {
JarOutputStream::JarOutputStream(
    const ::jxx::Ptr<::jxx::io::OutputStream>& output) : Super(output) {}
JarOutputStream::JarOutputStream(
    const ::jxx::Ptr<::jxx::io::OutputStream>& output,
    const ::jxx::Ptr<Manifest>& manifest) : Super(output) {
    if (manifest == nullptr) throw ::jxx::lang::NullPointerException();
    const auto entry = ::jxx::NEW<JarEntry>(
        ::jxx::NEW<::jxx::lang::String>("META-INF/MANIFEST.MF"));
    JxxSuper::putNextEntry(entry);
    const auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    manifest->write(bytes);
    JxxSuper::write(bytes->toByteArray());
    JxxSuper::closeEntry();
}
void JarOutputStream::putNextEntry(
    const ::jxx::Ptr<::jxx::util::zip::ZipEntry>& entry) {
    JxxSuper::putNextEntry(entry);
}
} // namespace jxx::util::jar
