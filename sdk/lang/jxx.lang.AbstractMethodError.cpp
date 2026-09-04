#include "lang/jxx.lang.AbstractMethodError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::AbstractMethodError::Class()
{
    return JxxClassInfoMarker::Class();
}
