#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.IncompatibleClassChangeError.h"
namespace jxx::lang {
class NoSuchMethodError : public IncompatibleClassChangeError {
public:
    using JxxSuper = IncompatibleClassChangeError;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<NoSuchMethodError, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    NoSuchMethodError() = default;

    NoSuchMethodError(const NoSuchMethodError&) = default;
    NoSuchMethodError(NoSuchMethodError&&) noexcept = default;
    NoSuchMethodError& operator=(const NoSuchMethodError&) = default;
    NoSuchMethodError& operator=(NoSuchMethodError&&) noexcept = default;
    ~NoSuchMethodError() override = default;

public:
	using IncompatibleClassChangeError::IncompatibleClassChangeError;
protected:
    JXX_OBJECT_CLONE(NoSuchMethodError)

    const char* typeName() const noexcept override { return "NoSuchMethodError"; }
};
}
