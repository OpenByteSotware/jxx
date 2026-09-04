#include "lang/jxx.lang.IllegalAccessError.h"
#include "lang/jxx.lang.ClassInfo.h"


jxx::Ptr<jxx::lang::ClassAny> jxx::lang::IllegalAccessError::Class()
{
    return JxxClassInfoMarker::Class();
}
