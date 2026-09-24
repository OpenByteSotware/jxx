#include "lang/jxx.lang.VirtualMachineError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::VirtualMachineError::Class()
{
    return JxxClassInfoMarker::Class();
}

jxx::lang::VirtualMachineError::VirtualMachineError(const ::jxx::Ptr<::jxx::lang::String>& message)
    : jxx::lang::VirtualMachineError::JxxSuper(message) {}

jxx::lang::VirtualMachineError::VirtualMachineError(const char* message)
    : jxx::lang::VirtualMachineError::JxxSuper(message) {
}

const char* jxx::lang::VirtualMachineError::typeName() const noexcept { return "VirtualMachineError"; }
