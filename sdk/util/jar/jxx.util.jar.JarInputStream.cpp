#include "util/jar/jxx.util.jar.JarInputStream.h"

#include "util/jar/jxx.util.jar.JarEntry.h"
#include "util/jar/jxx.util.jar.Manifest.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"

namespace jxx::util::jar {
JarInputStream::JarInputStream(const ::jxx::Ptr<::jxx::io::InputStream>& input)
    : Super(input) { initializeManifest_(); }
JarInputStream::JarInputStream(const ::jxx::Ptr<::jxx::io::InputStream>& input,
    ::jxx::lang::jbool) : Super(input) { initializeManifest_(); }
void JarInputStream::initializeManifest_() {
    auto first = JxxSuper::getNextEntry();
    if (first == nullptr) return;
    const auto name = first->getName();
    if (name != nullptr && name->equalsIgnoreCase(
            ::jxx::NEW<::jxx::lang::String>("META-INF/MANIFEST.MF"))) {
        const auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
        for (;;) {
            const auto value = read();
            if (value < 0) break;
            bytes->write(value);
        }
        const auto input = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(
            bytes->toByteArray());
        manifest_ = ::jxx::NEW<Manifest>(input);
        JxxSuper::closeEntry();
    } else {
        pending_ = ::jxx::NEW<JarEntry>(first);
    }
}
::jxx::Ptr<Manifest> JarInputStream::getManifest() const { return manifest_; }
::jxx::Ptr<JarEntry> JarInputStream::getNextJarEntry() {
    if (pending_ != nullptr) {
        const auto result = pending_;
        pending_ = nullptr;
        return result;
    }
    const auto entry = JxxSuper::getNextEntry();
    return entry == nullptr ? nullptr : ::jxx::NEW<JarEntry>(entry);
}
::jxx::Ptr<::jxx::util::zip::ZipEntry> JarInputStream::getNextEntry() {
    return ::jxx::CAST<::jxx::util::zip::ZipEntry>(getNextJarEntry());
}
} // namespace jxx::util::jar
