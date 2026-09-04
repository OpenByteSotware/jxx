#include "lang/jxx.lang.IllegalAccessException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::IllegalAccessException::Class()
{
    return JxxClassInfoMarker::Class();
}
