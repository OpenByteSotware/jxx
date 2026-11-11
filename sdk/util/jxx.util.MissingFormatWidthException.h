#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.IllegalFormatException.h"

namespace jxx::util {

class MissingFormatWidthException final : public IllegalFormatException {
public:
    using JxxSuper = IllegalFormatException;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<MissingFormatWidthException, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    explicit MissingFormatWidthException(
        const ::jxx::Ptr<::jxx::lang::String>& formatSpecifier);

    ::jxx::Ptr<::jxx::lang::String> getFormatSpecifier() const;

    JXX_OBJECT_CLONE(MissingFormatWidthException)

protected:
    const char* typeName() const noexcept override {
        return "MissingFormatWidthException";
    }

private:
    ::jxx::Ptr<::jxx::lang::String> formatSpecifier_;
};

} // namespace jxx::util
