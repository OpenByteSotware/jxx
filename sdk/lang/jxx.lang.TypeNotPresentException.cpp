#include "lang/jxx.lang.TypeNotPresentException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::TypeNotPresentException::Class()
{
    return JxxClassInfoMarker::Class();
}
