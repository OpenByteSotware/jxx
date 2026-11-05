#include "util/jar/internal/jxx.util.jar.internal.JarEntryEnumeration.h"

#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::util::jar::internal {
JarEntryEnumeration::JarEntryEnumeration(
    const ::jxx::Ptr<::jxx::util::Enumeration<
        ::jxx::util::zip::ZipEntry>>& entries)
    : entries_(entries) {
    if (entries_ == nullptr) throw ::jxx::lang::NullPointerException();
}
::jxx::lang::jbool JarEntryEnumeration::hasMoreElements() {
    return entries_->hasMoreElements();
}
::jxx::Ptr<::jxx::util::jar::JarEntry>
JarEntryEnumeration::nextElement() {
    const auto entry = entries_->nextElement();
    return entry == nullptr ? nullptr : ::jxx::NEW<::jxx::util::jar::JarEntry>(entry);
}
} // namespace jxx::util::jar::internal
