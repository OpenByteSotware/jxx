#include "lang/jxx.lang.NoSuchMethodError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::NoSuchMethodError::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::NoSuchMethodError::NoSuchMethodError(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::NoSuchMethodError::JxxSuper(message) {}

jxx::lang::NoSuchMethodError::NoSuchMethodError(const char* message)
    : jxx::lang::NoSuchMethodError::JxxSuper(message) {
}
