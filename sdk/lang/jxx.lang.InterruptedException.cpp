#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::InterruptedException::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::InterruptedException::InterruptedException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::InterruptedException::JxxSuper(message) {}

jxx::lang::InterruptedException::InterruptedException(const char* message)
    : jxx::lang::InterruptedException::JxxSuper(message) {
}

const char* jxx::lang::InterruptedException::typeName() const noexcept { return "InterruptedException"; }
