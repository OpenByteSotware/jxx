#include "lang/jxx.lang.BootstrapMethodError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::BootstrapMethodError::Class()
{
    return JxxClassInfoMarker::Class();
}
