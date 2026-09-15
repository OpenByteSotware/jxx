#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::IndexOutOfBoundsException::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::IndexOutOfBoundsException::IndexOutOfBoundsException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::IndexOutOfBoundsException::JxxSuper(message) {}

jxx::lang::IndexOutOfBoundsException::IndexOutOfBoundsException(const char* message)
    : jxx::lang::IndexOutOfBoundsException::JxxSuper(message) {
}
