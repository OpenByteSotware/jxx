#include "lang/jxx.lang.InstantiationException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::InstantiationException::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::InstantiationException::InstantiationException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::InstantiationException::JxxSuper(message) {}

jxx::lang::InstantiationException::InstantiationException(const char* message)
    : jxx::lang::InstantiationException::JxxSuper(message) {
}

const char* jxx::lang::InstantiationException::typeName() const noexcept { return "InstantiationException"; }
