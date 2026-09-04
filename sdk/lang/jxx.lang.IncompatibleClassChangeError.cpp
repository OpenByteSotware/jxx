#include "lang/jxx.lang.IncompatibleClassChangeError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::IncompatibleClassChangeError::Class()
{
    return JxxClassInfoMarker::Class();
}
