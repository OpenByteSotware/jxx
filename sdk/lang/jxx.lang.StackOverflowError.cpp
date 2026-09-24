#include "lang/jxx.lang.StackOverflowError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::StackOverflowError::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::StackOverflowError::StackOverflowError(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::StackOverflowError::JxxSuper(message) {}

jxx::lang::StackOverflowError::StackOverflowError(const char* message)
    : jxx::lang::StackOverflowError::JxxSuper(message) {
}

const char* jxx::lang::StackOverflowError::typeName() const noexcept { return "StackOverflowError"; }
