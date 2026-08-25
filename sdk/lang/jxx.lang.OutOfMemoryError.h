#pragma once
#include "lang/jxx.lang.VirtualMachineError.h"
namespace jxx::lang {
class OutOfMemoryError : public VirtualMachineError {
public:
	using VirtualMachineError::VirtualMachineError;
protected:
    JXX_OBJECT_CLONE(OutOfMemoryError)

    const char* typeName() const noexcept override { return "OutOfMemoryError"; }
};
}
