#include "lang/jxx.lang.SecurityException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::SecurityException::Class()
{
    return JxxClassInfoMarker::Class();
}
