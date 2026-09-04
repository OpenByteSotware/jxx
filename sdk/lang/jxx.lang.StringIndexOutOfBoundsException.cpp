#include "lang/jxx.lang.StringIndexOutOfBoundsException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::StringIndexOutOfBoundsException::Class()
{
    return JxxClassInfoMarker::Class();
}
