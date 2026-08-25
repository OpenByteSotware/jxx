#pragma once
#include "lang/jxx.lang.VirtualMachineError.h"
namespace jxx::lang {
class InternalError : public VirtualMachineError {
public:
	using VirtualMachineError::VirtualMachineError;
protected:
    JXX_OBJECT_CLONE(InternalError)

    const char* typeName() const noexcept override { return "InternalError"; }
};
}
