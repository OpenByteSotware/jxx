#pragma once
#include "lang/jxx.lang.Error.h"
namespace jxx::lang {
class VirtualMachineError : public Error {
public:
	using Error::Error;
protected:
    JXX_OBJECT_CLONE(VirtualMachineError)

    const char* typeName() const noexcept override { return "VirtualMachineError"; }
};
}
