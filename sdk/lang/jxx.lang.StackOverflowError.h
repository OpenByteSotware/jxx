#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.VirtualMachineError.h"
namespace jxx::lang {
class StackOverflowError : public VirtualMachineError {
public:
    using JxxSuper = VirtualMachineError;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<StackOverflowError, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    StackOverflowError() = default;

    StackOverflowError(const StackOverflowError&) = default;
    StackOverflowError(StackOverflowError&&) noexcept = default;
    StackOverflowError& operator=(const StackOverflowError&) = default;
    StackOverflowError& operator=(StackOverflowError&&) noexcept = default;
    ~StackOverflowError() override = default;

public:
	using VirtualMachineError::VirtualMachineError;
protected:
    JXX_OBJECT_CLONE(StackOverflowError)

    const char* typeName() const noexcept override { return "StackOverflowError"; }
};
}
