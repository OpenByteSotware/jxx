#include "lang/jxx.lang.NumberFormatException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::NumberFormatException::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::NumberFormatException::NumberFormatException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::NumberFormatException::JxxSuper(message) {}

jxx::lang::NumberFormatException::NumberFormatException(const char* message)
    : jxx::lang::NumberFormatException::JxxSuper(message) {
}
