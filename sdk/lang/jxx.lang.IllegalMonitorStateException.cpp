#include "lang/jxx.lang.IllegalMonitorStateException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::IllegalMonitorStateException::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::IllegalMonitorStateException::IllegalMonitorStateException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::IllegalMonitorStateException::JxxSuper(message) {}

jxx::lang::IllegalMonitorStateException::IllegalMonitorStateException(const char* message)
    : jxx::lang::IllegalMonitorStateException::JxxSuper(message) {
}

const char* jxx::lang::IllegalMonitorStateException::typeName() const noexcept { return "IllegalMonitorStateException"; }
