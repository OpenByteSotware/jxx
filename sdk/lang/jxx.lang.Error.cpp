#include "lang/jxx.lang.Error.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::Error::Class()
{
    return JxxClassInfoMarker::Class();
}
