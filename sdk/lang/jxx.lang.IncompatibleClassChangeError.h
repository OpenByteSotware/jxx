#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.LinkageError.h"
namespace jxx::lang {
class IncompatibleClassChangeError : public LinkageError {
public:
    using JxxSuper = LinkageError;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<IncompatibleClassChangeError, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    IncompatibleClassChangeError() = default;

    IncompatibleClassChangeError(const IncompatibleClassChangeError&) = default;
    IncompatibleClassChangeError(IncompatibleClassChangeError&&) noexcept = default;
    IncompatibleClassChangeError& operator=(const IncompatibleClassChangeError&) = default;
    IncompatibleClassChangeError& operator=(IncompatibleClassChangeError&&) noexcept = default;
    ~IncompatibleClassChangeError() override = default;

public:
	using jxx::lang::LinkageError::LinkageError;
protected:
    JXX_OBJECT_CLONE(IncompatibleClassChangeError)

    const char* typeName() const noexcept override { return "IncompatibleClassChangeError"; }
};
}
