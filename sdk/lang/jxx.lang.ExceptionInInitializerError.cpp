#include "lang/jxx.lang.ExceptionInInitializerError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::ExceptionInInitializerError::Class()
{
    return JxxClassInfoMarker::Class();
}
