#include "lang/jxx.lang.UnsupportedClassVersionError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::UnsupportedClassVersionError::Class()
{
    return JxxClassInfoMarker::Class();
}
