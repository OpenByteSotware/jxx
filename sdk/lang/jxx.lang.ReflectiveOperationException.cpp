#include "lang/jxx.lang.ReflectiveOperationException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::ReflectiveOperationException::Class()
{
    return JxxClassInfoMarker::Class();
}
