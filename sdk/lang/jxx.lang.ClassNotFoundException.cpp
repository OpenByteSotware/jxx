#include "lang/jxx.lang.ClassNotFoundException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::ClassNotFoundException::Class()
{
    return JxxClassInfoMarker::Class();
}
