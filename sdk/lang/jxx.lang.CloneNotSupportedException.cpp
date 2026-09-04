#include "lang/jxx.lang.CloneNotSupportedException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::CloneNotSupportedException::Class()
{
    return JxxClassInfoMarker::Class();
}
