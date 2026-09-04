#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.IncompatibleClassChangeError.h"
namespace jxx::lang {
class NoSuchFieldError : public IncompatibleClassChangeError {
public:
    using JxxSuper = IncompatibleClassChangeError;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<NoSuchFieldError, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    NoSuchFieldError() = default;

    NoSuchFieldError(const NoSuchFieldError&) = default;
    NoSuchFieldError(NoSuchFieldError&&) noexcept = default;
    NoSuchFieldError& operator=(const NoSuchFieldError&) = default;
    NoSuchFieldError& operator=(NoSuchFieldError&&) noexcept = default;
    ~NoSuchFieldError() override = default;

public:
	using IncompatibleClassChangeError::IncompatibleClassChangeError;
protected:
    JXX_OBJECT_CLONE(NoSuchFieldError)

    const char* typeName() const noexcept override { return "NoSuchFieldError"; }
};
}
