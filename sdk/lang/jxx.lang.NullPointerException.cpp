#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::NullPointerException::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::NullPointerException::NullPointerException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::NullPointerException::JxxSuper(message) {}

jxx::lang::NullPointerException::NullPointerException(const char* message)
    : jxx::lang::NullPointerException::JxxSuper(message) {
}

const char* jxx::lang::NullPointerException::typeName() const noexcept { return "NullPointerException"; }
