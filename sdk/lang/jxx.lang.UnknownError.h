#pragma once
#include "lang/jxx.lang.VirtualMachineError.h"
namespace jxx::lang {
class UnknownError : public VirtualMachineError {
public:
	using VirtualMachineError::VirtualMachineError;
protected:
    JXX_OBJECT_CLONE(UnknownError)

    const char* typeName() const noexcept override { return "UnknownError"; }
};
}
