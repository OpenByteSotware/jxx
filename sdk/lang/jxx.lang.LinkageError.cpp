#include "lang/jxx.lang.LinkageError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::LinkageError::Class()
{
    return JxxClassInfoMarker::Class();
}
