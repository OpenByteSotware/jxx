#include "lang/jxx.lang.NoSuchMethodException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::NoSuchMethodException::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::NoSuchMethodException::NoSuchMethodException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::NoSuchMethodException::JxxSuper(message) {}

jxx::lang::NoSuchMethodException::NoSuchMethodException(const char* message)
    : jxx::lang::NoSuchMethodException::JxxSuper(message) {
}
