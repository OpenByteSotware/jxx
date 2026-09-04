#include "util/jxx.util.concurrent.CancellationException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::util::concurrent::CancellationException::Class()
{
    return JxxClassInfoMarker::Class();
}
