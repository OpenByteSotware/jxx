#include "lang/jxx.lang.ClassFormatError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::ClassFormatError::Class()
{
    return JxxClassInfoMarker::Class();
}
