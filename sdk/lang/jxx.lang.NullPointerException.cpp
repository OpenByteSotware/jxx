#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::NullPointerException::Class()
{
    return JxxClassInfoMarker::Class();
}
