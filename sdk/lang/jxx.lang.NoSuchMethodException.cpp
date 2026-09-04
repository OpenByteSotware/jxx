#include "lang/jxx.lang.NoSuchMethodException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::NoSuchMethodException::Class()
{
    return JxxClassInfoMarker::Class();
}
