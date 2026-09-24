#include "lang/jxx.lang.ClassCastException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::ClassCastException::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::ClassCastException::ClassCastException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::ClassCastException::JxxSuper(message) {}

jxx::lang::ClassCastException::ClassCastException(const char* message)
    : jxx::lang::ClassCastException::JxxSuper(message) {
}

const char* jxx::lang::ClassCastException::typeName() const noexcept { return "ClassCastException"; }
