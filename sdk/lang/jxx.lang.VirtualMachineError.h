#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.Error.h"
namespace jxx::lang {
class VirtualMachineError : public Error {
public:
    using JxxSuper = Error;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<VirtualMachineError, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    VirtualMachineError() = default;

    VirtualMachineError(const VirtualMachineError&) = default;
    VirtualMachineError(VirtualMachineError&&) noexcept = default;
    VirtualMachineError& operator=(const VirtualMachineError&) = default;
    VirtualMachineError& operator=(VirtualMachineError&&) noexcept = default;
    ~VirtualMachineError() override = default;

public:
	using Error::Error;
protected:
    JXX_OBJECT_CLONE(VirtualMachineError)

    const char* typeName() const noexcept override { return "VirtualMachineError"; }
};
}
