#include "lang/jxx.lang.AbstractMethodError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::AbstractMethodError::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::AbstractMethodError::AbstractMethodError(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::AbstractMethodError::JxxSuper(message) {}

jxx::lang::AbstractMethodError::AbstractMethodError(const char* message)
    : jxx::lang::AbstractMethodError::JxxSuper(message) {
}

const char* jxx::lang::AbstractMethodError::typeName() const noexcept { return "AbstractMethodError"; }
