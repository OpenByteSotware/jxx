#include "lang/jxx.lang.NoClassDefFoundError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::NoClassDefFoundError::Class()
{
    return JxxClassInfoMarker::Class();
}
