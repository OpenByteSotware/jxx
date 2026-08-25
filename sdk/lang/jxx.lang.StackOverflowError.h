#pragma once
#include "lang/jxx.lang.VirtualMachineError.h"
namespace jxx::lang {
class StackOverflowError : public VirtualMachineError {
public:
	using VirtualMachineError::VirtualMachineError;
protected:
    JXX_OBJECT_CLONE(StackOverflowError)

    const char* typeName() const noexcept override { return "StackOverflowError"; }
};
}
