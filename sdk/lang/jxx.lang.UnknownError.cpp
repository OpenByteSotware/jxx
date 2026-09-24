#include "lang/jxx.lang.UnknownError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::UnknownError::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::UnknownError::UnknownError(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::UnknownError::JxxSuper(message) {}

jxx::lang::UnknownError::UnknownError(const char* message)
    : jxx::lang::UnknownError::JxxSuper(message) {
}

const char* jxx::lang::UnknownError::typeName() const noexcept { return "UnknownError"; }
