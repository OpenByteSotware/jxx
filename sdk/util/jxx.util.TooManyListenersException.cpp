#include "util/jxx.util.TooManyListenersException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::util::TooManyListenersException::Class()
{
    return JxxClassInfoMarker::Class();
}
