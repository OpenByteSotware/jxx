#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.VirtualMachineError.h"
namespace jxx::lang {
class OutOfMemoryError : public VirtualMachineError {
public:
    using JxxSuper = VirtualMachineError;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<OutOfMemoryError, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    OutOfMemoryError() = default;

    OutOfMemoryError(const OutOfMemoryError&) = default;
    OutOfMemoryError(OutOfMemoryError&&) noexcept = default;
    OutOfMemoryError& operator=(const OutOfMemoryError&) = default;
    OutOfMemoryError& operator=(OutOfMemoryError&&) noexcept = default;
    ~OutOfMemoryError() override = default;

public:
	using VirtualMachineError::VirtualMachineError;
protected:
    JXX_OBJECT_CLONE(OutOfMemoryError)

    const char* typeName() const noexcept override { return "OutOfMemoryError"; }
};
}
