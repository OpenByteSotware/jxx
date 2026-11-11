#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.RuntimeException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"

namespace jxx::util {

class IllformedLocaleException final : public ::jxx::lang::RuntimeException {
public:
    using JxxSuper = ::jxx::lang::RuntimeException;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<IllformedLocaleException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    IllformedLocaleException();
    explicit IllformedLocaleException(
        const ::jxx::Ptr<::jxx::lang::String>& message);
    IllformedLocaleException(
        const ::jxx::Ptr<::jxx::lang::String>& message,
        ::jxx::lang::jint errorIndex);

    ::jxx::lang::jint getErrorIndex() const noexcept;

    JXX_OBJECT_CLONE(IllformedLocaleException)

protected:
    const char* typeName() const noexcept override {
        return "IllformedLocaleException";
    }

private:
    ::jxx::lang::jint errorIndex_ = -1;
};

} // namespace jxx::util
