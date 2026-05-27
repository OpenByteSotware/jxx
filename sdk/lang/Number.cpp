#include "lang/jxx.lang.Number.h"

jbyte Number::byteValue() const
{
    return static_cast<jbyte>(intValue());
}

jshort Number::shortValue() const
{
    return static_cast<jshort>(intValue());
}
