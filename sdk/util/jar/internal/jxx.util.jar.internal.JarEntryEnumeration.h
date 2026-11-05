#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Enumeration.h"
#include "util/jar/jxx.util.jar.JarEntry.h"
#include "util/zip/jxx.util.zip.ZipEntry.h"

namespace jxx::util::jar::internal {

class JarEntryEnumeration final
    : public ::jxx::lang::ClassBase<
          JarEntryEnumeration,
          ::jxx::lang::Object,
          ::jxx::util::Enumeration<::jxx::util::jar::JarEntry>> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        JarEntryEnumeration, JxxSuper,
        ::jxx::util::Enumeration<::jxx::util::jar::JarEntry>>;

    explicit JarEntryEnumeration(
        const ::jxx::Ptr<::jxx::util::Enumeration<
            ::jxx::util::zip::ZipEntry>>& entries);
    ::jxx::lang::jbool hasMoreElements() override;
    ::jxx::Ptr<::jxx::util::jar::JarEntry> nextElement() override;

private:
    ::jxx::Ptr<::jxx::util::Enumeration<::jxx::util::zip::ZipEntry>> entries_;
};

} // namespace jxx::util::jar::internal
