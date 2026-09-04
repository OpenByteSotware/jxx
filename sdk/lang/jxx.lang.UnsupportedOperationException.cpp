#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::UnsupportedOperationException::Class()
{
    return JxxClassInfoMarker::Class();
}
