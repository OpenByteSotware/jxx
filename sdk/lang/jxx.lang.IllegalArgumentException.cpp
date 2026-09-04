#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::IllegalArgumentException::Class()
{
    return JxxClassInfoMarker::Class();
}
