#include "lang/jxx.lang.InstantiationError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::InstantiationError::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::InstantiationError::InstantiationError(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::InstantiationError::JxxSuper(message) {}

jxx::lang::InstantiationError::InstantiationError(const char* message)
    : jxx::lang::InstantiationError::JxxSuper(message) {
}
