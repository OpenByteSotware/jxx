#include "lang/jxx.lang.SecurityException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::SecurityException::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::SecurityException::SecurityException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::SecurityException::JxxSuper(message) {}

jxx::lang::SecurityException::SecurityException(const char* message)
    : jxx::lang::SecurityException::JxxSuper(message) {
}

const char* jxx::lang::SecurityException::typeName() const noexcept { return "SecurityException"; }
