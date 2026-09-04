#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.VirtualMachineError.h"
namespace jxx::lang {
class UnknownError : public VirtualMachineError {
public:
    using JxxSuper = VirtualMachineError;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<UnknownError, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    UnknownError() = default;

    UnknownError(const UnknownError&) = default;
    UnknownError(UnknownError&&) noexcept = default;
    UnknownError& operator=(const UnknownError&) = default;
    UnknownError& operator=(UnknownError&&) noexcept = default;
    ~UnknownError() override = default;

public:
	using VirtualMachineError::VirtualMachineError;
protected:
    JXX_OBJECT_CLONE(UnknownError)

    const char* typeName() const noexcept override { return "UnknownError"; }
};
}
