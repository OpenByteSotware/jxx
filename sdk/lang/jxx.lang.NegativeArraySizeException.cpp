#include "lang/jxx.lang.NegativeArraySizeException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::NegativeArraySizeException::Class()
{
    return JxxClassInfoMarker::Class();
}
