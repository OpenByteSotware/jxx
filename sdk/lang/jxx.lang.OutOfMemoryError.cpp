#include "lang/jxx.lang.OutOfMemoryError.h"
#include "lang/jxx.lang.ClassInfo.h"


jxx::Ptr<jxx::lang::ClassAny> jxx::lang::OutOfMemoryError::Class()
{
    return JxxClassInfoMarker::Class();
}
