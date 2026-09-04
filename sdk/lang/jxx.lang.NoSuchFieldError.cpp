#include "lang/jxx.lang.NoSuchFieldError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::NoSuchFieldError::Class()
{
    return JxxClassInfoMarker::Class();
}
