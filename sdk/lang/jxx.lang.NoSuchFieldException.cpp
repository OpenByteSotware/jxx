#include "lang/jxx.lang.NoSuchFieldException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::NoSuchFieldException::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::NoSuchFieldException::NoSuchFieldException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::NoSuchFieldException::JxxSuper(message) {}

jxx::lang::NoSuchFieldException::NoSuchFieldException(const char* message)
    : jxx::lang::NoSuchFieldException::JxxSuper(message) {
}

const char* jxx::lang::NoSuchFieldException::typeName() const noexcept { return "NoSuchFieldException"; }
