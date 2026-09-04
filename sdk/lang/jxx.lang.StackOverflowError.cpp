#include "lang/jxx.lang.StackOverflowError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::StackOverflowError::Class()
{
    return JxxClassInfoMarker::Class();
}
