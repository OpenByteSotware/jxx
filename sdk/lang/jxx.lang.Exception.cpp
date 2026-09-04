#include "lang/jxx.lang.Exception.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::Exception::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::InvocationTargetException::Class()
{
    return JxxClassInfoMarker::Class();
}
