#include "lang/jxx.lang.NoSuchFieldException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::NoSuchFieldException::Class()
{
    return JxxClassInfoMarker::Class();
}
