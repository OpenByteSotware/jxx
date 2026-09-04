#include "lang/jxx.lang.InstantiationException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::InstantiationException::Class()
{
    return JxxClassInfoMarker::Class();
}
