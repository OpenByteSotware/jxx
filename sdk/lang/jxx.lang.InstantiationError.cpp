#include "lang/jxx.lang.InstantiationError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::InstantiationError::Class()
{
    return JxxClassInfoMarker::Class();
}
