#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::UnsupportedOperationException::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::UnsupportedOperationException::UnsupportedOperationException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::UnsupportedOperationException::JxxSuper(message) {}

jxx::lang::UnsupportedOperationException::UnsupportedOperationException(const char* message)
    : jxx::lang::UnsupportedOperationException::JxxSuper(message) {
}

const char* jxx::lang::UnsupportedOperationException::typeName() const noexcept { return "UnsupportedOperationException"; }
