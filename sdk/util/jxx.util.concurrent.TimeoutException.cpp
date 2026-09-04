#include "util/jxx.util.concurrent.TimeoutException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::util::concurrent::TimeoutException::Class()
{
    return JxxClassInfoMarker::Class();
}
