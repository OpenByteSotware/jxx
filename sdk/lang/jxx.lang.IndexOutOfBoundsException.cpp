#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::IndexOutOfBoundsException::Class()
{
    return JxxClassInfoMarker::Class();
}
