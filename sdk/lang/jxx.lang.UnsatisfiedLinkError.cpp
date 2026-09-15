#include "lang/jxx.lang.UnsatisfiedLinkError.h"
#include "lang/jxx.lang.ClassInfo.h"


jxx::Ptr<jxx::lang::ClassAny> jxx::lang::UnsatisfiedLinkError::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::UnsatisfiedLinkError::UnsatisfiedLinkError(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::UnsatisfiedLinkError::JxxSuper(message) {}

jxx::lang::UnsatisfiedLinkError::UnsatisfiedLinkError(const char* message)
    : jxx::lang::UnsatisfiedLinkError::JxxSuper(message) {
}
