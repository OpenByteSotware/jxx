#include "lang/jxx.lang.ClassCircularityError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::ClassCircularityError::Class()
{
    return JxxClassInfoMarker::Class();
}
