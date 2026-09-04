#include "lang/jxx.lang.UnknownError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::UnknownError::Class()
{
    return JxxClassInfoMarker::Class();
}
