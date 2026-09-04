#include "lang/jxx.lang.RuntimeException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::RuntimeException::Class()
{
    return JxxClassInfoMarker::Class();
}
