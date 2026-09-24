#include "lang/jxx.lang.ArithmeticException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::ArithmeticException::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::ArithmeticException::ArithmeticException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::ArithmeticException::JxxSuper(message) {}

jxx::lang::ArithmeticException::ArithmeticException(const char* message)
    : jxx::lang::ArithmeticException::JxxSuper(message) {
}

const char* jxx::lang::ArithmeticException::typeName() const noexcept { return "ArithmeticException"; }
