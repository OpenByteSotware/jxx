#include "lang/jxx.lang.ArrayExceptionSupport.h"

#include "lang/jxx.lang.ArrayIndexOutOfBoundsException.h"
#include "lang/jxx.lang.NegativeArraySizeException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::lang::array_detail {

[[noreturn]] void throwNegativeArraySize()
{
    throw ::jxx::lang::NegativeArraySizeException();
}

[[noreturn]] void throwIndex(::jxx::lang::jint index)
{
    throw ::jxx::lang::ArrayIndexOutOfBoundsException(index);
}

[[noreturn]] void throwNullRow()
{
    throw ::jxx::lang::NullPointerException();
}

} // namespace jxx::lang::array_detail
