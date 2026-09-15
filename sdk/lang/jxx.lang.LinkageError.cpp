#include "lang/jxx.lang.LinkageError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::LinkageError::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::LinkageError::LinkageError(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::LinkageError::JxxSuper(message) {}

jxx::lang::LinkageError::LinkageError(const char* message)
    : jxx::lang::LinkageError::JxxSuper(message) {
}
