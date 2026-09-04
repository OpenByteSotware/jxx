#include "util/jxx.util.NoSuchElementException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::util::NoSuchElementException::Class()
{
    return JxxClassInfoMarker::Class();
}
