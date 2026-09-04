#include "lang/jxx.lang.NoSuchMethodError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::NoSuchMethodError::Class()
{
    return JxxClassInfoMarker::Class();
}
