#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util {

class FormattableFlags final
    : public ::jxx::lang::ClassBase<FormattableFlags, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<FormattableFlags, JxxSuper>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    static constexpr ::jxx::lang::jint LEFT_JUSTIFY = 1;
    static constexpr ::jxx::lang::jint UPPERCASE = 2;
    static constexpr ::jxx::lang::jint ALTERNATE = 4;

private:
    FormattableFlags() = delete;
};

} // namespace jxx::util
