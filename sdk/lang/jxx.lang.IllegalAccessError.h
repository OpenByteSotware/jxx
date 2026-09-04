#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.IncompatibleClassChangeError.h"
namespace jxx::lang {
class IllegalAccessError : public IncompatibleClassChangeError {
public:
    using JxxSuper = IncompatibleClassChangeError;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<IllegalAccessError, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    IllegalAccessError() = default;

    IllegalAccessError(const IllegalAccessError&) = default;
    IllegalAccessError(IllegalAccessError&&) noexcept = default;
    IllegalAccessError& operator=(const IllegalAccessError&) = default;
    IllegalAccessError& operator=(IllegalAccessError&&) noexcept = default;
    ~IllegalAccessError() override = default;

public:
	using IncompatibleClassChangeError::IncompatibleClassChangeError;
protected:
    JXX_OBJECT_CLONE(IllegalAccessError)

    const char* typeName() const noexcept override { return "IllegalAccessError"; }
};
}
