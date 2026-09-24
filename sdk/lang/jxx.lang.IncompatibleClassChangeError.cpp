#include "lang/jxx.lang.IncompatibleClassChangeError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::IncompatibleClassChangeError::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::IncompatibleClassChangeError::IncompatibleClassChangeError(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::IncompatibleClassChangeError::JxxSuper(message) {}

jxx::lang::IncompatibleClassChangeError::IncompatibleClassChangeError(const char* message)
    : jxx::lang::IncompatibleClassChangeError::JxxSuper(message) {
}

const char* jxx::lang::IncompatibleClassChangeError::typeName() const noexcept { return "IncompatibleClassChangeError"; }
