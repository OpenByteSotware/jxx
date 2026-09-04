#include "lang/jxx.lang.UnsatisfiedLinkError.h"
#include "lang/jxx.lang.ClassInfo.h"


jxx::Ptr<jxx::lang::ClassAny> jxx::lang::UnsatisfiedLinkError::Class()
{
    return JxxClassInfoMarker::Class();
}
