#include "lang/jxx.lang.ClassCastException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::ClassCastException::Class()
{
    return JxxClassInfoMarker::Class();
}
