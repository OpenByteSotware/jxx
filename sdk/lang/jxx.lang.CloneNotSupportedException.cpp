#include "lang/jxx.lang.CloneNotSupportedException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::CloneNotSupportedException::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::CloneNotSupportedException::CloneNotSupportedException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::CloneNotSupportedException::JxxSuper(message) {}

jxx::lang::CloneNotSupportedException::CloneNotSupportedException(const char* message)
    : jxx::lang::CloneNotSupportedException::JxxSuper(message) {
}
