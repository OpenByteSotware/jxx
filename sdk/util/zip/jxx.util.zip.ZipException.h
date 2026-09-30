#pragma once

#include <string>

#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.ClassInfoMarker.h"

namespace jxx::util::zip {

class ZipException
    : public ::jxx::lang::ClassBase<
          ZipException,
          ::jxx::io::IOException> {
public:
    using JxxSuper = ::jxx::io::IOException;
    using Super = ::jxx::lang::ClassBase<
        ZipException,
        JxxSuper>;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<
            ZipException,
            JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    ZipException();

    explicit ZipException(
        const ::jxx::Ptr<::jxx::lang::String>& message);

    explicit ZipException(
        const char* message);

    explicit ZipException(
        const std::string& message);

    ZipException(const ZipException&) = default;
    ZipException(ZipException&&) noexcept = default;
    ZipException& operator=(const ZipException&) = default;
    ZipException& operator=(ZipException&&) noexcept = default;
    ~ZipException() override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::util::zip
