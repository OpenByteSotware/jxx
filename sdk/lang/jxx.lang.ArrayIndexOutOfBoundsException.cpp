#include "lang/jxx.lang.ArrayIndexOutOfBoundsException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::ArrayIndexOutOfBoundsException::Class()
{
    return JxxClassInfoMarker::Class();
}
