#include "lang/jxx.lang.NoSuchFieldError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::NoSuchFieldError::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::NoSuchFieldError::NoSuchFieldError(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::NoSuchFieldError::JxxSuper(message) {}

jxx::lang::NoSuchFieldError::NoSuchFieldError(const char* message)
    : jxx::lang::NoSuchFieldError::JxxSuper(message) {
}
