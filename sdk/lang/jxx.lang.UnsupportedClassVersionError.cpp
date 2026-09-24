#include "lang/jxx.lang.UnsupportedClassVersionError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::UnsupportedClassVersionError::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::UnsupportedClassVersionError::UnsupportedClassVersionError(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::UnsupportedClassVersionError::JxxSuper(message) {}

jxx::lang::UnsupportedClassVersionError::UnsupportedClassVersionError(const char* message)
    : jxx::lang::UnsupportedClassVersionError::JxxSuper(message) {
}

const char* jxx::lang::UnsupportedClassVersionError::typeName() const noexcept { return "UnsupportedClassVersionError"; }
