#include "lang/jxx.lang.Number.h"

namespace jxx::lang
{
    jxx::lang::jbyte Number::byteValue() const
    {
        return static_cast<jxx::lang::jbyte>(intValue());
    }

    jxx::lang::jshort Number::shortValue() const
    {
        return static_cast<jxx::lang::jshort>(intValue());
    }
}
