#include "lang/jxx.lang.VerifyError.h"
#include "lang/jxx.lang.ClassInfo.h"


jxx::Ptr<jxx::lang::ClassAny> jxx::lang::VerifyError::Class()
{
    return JxxClassInfoMarker::Class();
}
