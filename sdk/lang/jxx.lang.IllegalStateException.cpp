#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::IllegalStateException::Class()
{
    return JxxClassInfoMarker::Class();
}
