#include "lang/jxx.lang.IllegalAccessError.h"
#include "lang/jxx.lang.ClassInfo.h"


jxx::Ptr<jxx::lang::ClassAny> jxx::lang::IllegalAccessError::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::IllegalAccessError::IllegalAccessError(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::IllegalAccessError::JxxSuper(message) {}

jxx::lang::IllegalAccessError::IllegalAccessError(const char* message)
    : jxx::lang::IllegalAccessError::JxxSuper(message) {
}
