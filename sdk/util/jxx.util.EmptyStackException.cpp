#include "util/jxx.util.EmptyStackException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::util::EmptyStackException::Class()
{
    return JxxClassInfoMarker::Class();
}
