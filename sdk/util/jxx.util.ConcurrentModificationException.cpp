#include "util/jxx.util.ConcurrentModificationException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::util::ConcurrentModificationException::Class()
{
    return JxxClassInfoMarker::Class();
}
