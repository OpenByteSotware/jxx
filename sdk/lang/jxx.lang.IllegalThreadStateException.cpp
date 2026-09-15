#include "lang/jxx.lang.IllegalThreadStateException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::IllegalThreadStateException::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::IllegalThreadStateException::IllegalThreadStateException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::IllegalThreadStateException::JxxSuper(message) {}

jxx::lang::IllegalThreadStateException::IllegalThreadStateException(const char* message)
    : jxx::lang::IllegalThreadStateException::JxxSuper(message) {
}
