#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::IllegalStateException::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::IllegalStateException::IllegalStateException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::IllegalStateException::JxxSuper(message) {}

jxx::lang::IllegalStateException::IllegalStateException(const char* message)
    : jxx::lang::IllegalStateException::JxxSuper(message) {
}

const char* jxx::lang::IllegalStateException::typeName() const noexcept { return "IllegalStateException"; }
