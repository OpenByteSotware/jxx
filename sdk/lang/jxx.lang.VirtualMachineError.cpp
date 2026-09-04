#include "lang/jxx.lang.VirtualMachineError.h"
#include "lang/jxx.lang.ClassInfo.h"

jxx::Ptr<jxx::lang::ClassAny> jxx::lang::VirtualMachineError::Class()
{
    return JxxClassInfoMarker::Class();
}
