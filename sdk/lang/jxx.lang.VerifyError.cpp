#include "lang/jxx.lang.VerifyError.h"
#include "lang/jxx.lang.ClassInfo.h"


jxx::Ptr<jxx::lang::ClassAny> jxx::lang::VerifyError::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::VerifyError::VerifyError(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::VerifyError::JxxSuper(message) {}

jxx::lang::VerifyError::VerifyError(const char* message)
    : jxx::lang::VerifyError::JxxSuper(message) {
}
