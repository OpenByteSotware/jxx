#include "lang/jxx.lang.EnumConstantNotPresentException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::EnumConstantNotPresentException::Class()
{
    return JxxClassInfoMarker::Class();
}
