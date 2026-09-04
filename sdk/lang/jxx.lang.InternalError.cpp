#include "lang/jxx.lang.InternalError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::InternalError::Class()
{
    return JxxClassInfoMarker::Class();
}
