#include "lang/jxx.lang.InternalError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::InternalError::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::InternalError::InternalError(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::InternalError::JxxSuper(message) {}

jxx::lang::InternalError::InternalError(const char* message)
    : jxx::lang::InternalError::JxxSuper(message) {
}
