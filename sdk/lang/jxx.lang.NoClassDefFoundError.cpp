#include "lang/jxx.lang.NoClassDefFoundError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::NoClassDefFoundError::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::NoClassDefFoundError::NoClassDefFoundError(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::NoClassDefFoundError::JxxSuper(message) {}

jxx::lang::NoClassDefFoundError::NoClassDefFoundError(const char* message)
    : jxx::lang::NoClassDefFoundError::JxxSuper(message) {
}

const char* jxx::lang::NoClassDefFoundError::typeName() const noexcept { return "NoClassDefFoundError"; }
