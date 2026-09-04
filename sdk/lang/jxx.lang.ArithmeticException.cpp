#include "lang/jxx.lang.ArithmeticException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::ArithmeticException::Class()
{
    return JxxClassInfoMarker::Class();
}
