#include "util/jxx.util.concurrent.RejectedExecutionException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::util::concurrent::RejectedExecutionException::Class()
{
    return JxxClassInfoMarker::Class();
}
