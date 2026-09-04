#include "util/jxx.util.concurrent.BrokenBarrierException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::util::concurrent::BrokenBarrierException::Class()
{
    return JxxClassInfoMarker::Class();
}
