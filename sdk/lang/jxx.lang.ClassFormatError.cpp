#include "lang/jxx.lang.ClassFormatError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::ClassFormatError::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::ClassFormatError::ClassFormatError(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::ClassFormatError::JxxSuper(message) {}

jxx::lang::ClassFormatError::ClassFormatError(const char* message)
    : jxx::lang::ClassFormatError::JxxSuper(message) {
}
