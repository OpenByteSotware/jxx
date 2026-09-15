#include "lang/jxx.lang.ClassCircularityError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::ClassCircularityError::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::ClassCircularityError::ClassCircularityError(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::ClassCircularityError::JxxSuper(message) {}

jxx::lang::ClassCircularityError::ClassCircularityError(const char* message)
    : jxx::lang::ClassCircularityError::JxxSuper(message) {
}
