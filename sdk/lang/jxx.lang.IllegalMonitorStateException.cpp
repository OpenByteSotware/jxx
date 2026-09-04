#include "lang/jxx.lang.IllegalMonitorStateException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::IllegalMonitorStateException::Class()
{
    return JxxClassInfoMarker::Class();
}
