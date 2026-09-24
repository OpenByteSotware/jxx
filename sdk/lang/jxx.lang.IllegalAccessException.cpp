#include "lang/jxx.lang.IllegalAccessException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::IllegalAccessException::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::IllegalAccessException::IllegalAccessException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::IllegalAccessException::JxxSuper(message) {}

jxx::lang::IllegalAccessException::IllegalAccessException(const char* message)
    : jxx::lang::IllegalAccessException::JxxSuper(message) {
}

const char* jxx::lang::IllegalAccessException::typeName() const noexcept { return "IllegalAccessException"; }
