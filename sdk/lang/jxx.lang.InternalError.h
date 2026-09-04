#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.VirtualMachineError.h"
namespace jxx::lang {
class InternalError : public VirtualMachineError {
public:
    using JxxSuper = VirtualMachineError;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<InternalError, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    InternalError() = default;

    InternalError(const InternalError&) = default;
    InternalError(InternalError&&) noexcept = default;
    InternalError& operator=(const InternalError&) = default;
    InternalError& operator=(InternalError&&) noexcept = default;
    ~InternalError() override = default;

public:
	using VirtualMachineError::VirtualMachineError;
protected:
    JXX_OBJECT_CLONE(InternalError)

    const char* typeName() const noexcept override { return "InternalError"; }
};
}
