#include "lang/jxx.lang.OutOfMemoryError.h"
#include "lang/jxx.lang.ClassInfo.h"


jxx::Ptr<jxx::lang::ClassAny> jxx::lang::OutOfMemoryError::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::OutOfMemoryError::OutOfMemoryError(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::OutOfMemoryError::JxxSuper(message) {}

jxx::lang::OutOfMemoryError::OutOfMemoryError(const char* message)
    : jxx::lang::OutOfMemoryError::JxxSuper(message) {
}
