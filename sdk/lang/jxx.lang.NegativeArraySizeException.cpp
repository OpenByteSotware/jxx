#include "lang/jxx.lang.NegativeArraySizeException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::NegativeArraySizeException::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::NegativeArraySizeException::NegativeArraySizeException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::NegativeArraySizeException::JxxSuper(message) {}

jxx::lang::NegativeArraySizeException::NegativeArraySizeException(const char* message)
    : jxx::lang::NegativeArraySizeException::JxxSuper(message) {
}

const char* jxx::lang::NegativeArraySizeException::typeName() const noexcept { return "NegativeArraySizeException"; }
