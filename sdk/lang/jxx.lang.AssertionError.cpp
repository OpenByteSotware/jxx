#include "lang/jxx.lang.AssertionError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::AssertionError::Class()
{
    return JxxClassInfoMarker::Class();
}
