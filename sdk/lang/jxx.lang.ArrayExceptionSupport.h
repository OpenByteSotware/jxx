#pragma once

#include "lang/jxx_types.h"

namespace jxx::lang::array_detail {

[[noreturn]] void throwNegativeArraySize();
[[noreturn]] void throwIndex(::jxx::lang::jint index);
[[noreturn]] void throwNullRow();

} // namespace jxx::lang::array_detail
