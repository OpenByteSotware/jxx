#include "lang/jxx.lang.IllegalThreadStateException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::IllegalThreadStateException::Class()
{
    return JxxClassInfoMarker::Class();
}
