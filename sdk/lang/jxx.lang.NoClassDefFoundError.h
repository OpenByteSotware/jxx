#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.LinkageError.h"
namespace jxx::lang {
class NoClassDefFoundError : public LinkageError {
public:
    using JxxSuper = LinkageError;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<NoClassDefFoundError, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    NoClassDefFoundError() = default;

    NoClassDefFoundError(const NoClassDefFoundError&) = default;
    NoClassDefFoundError(NoClassDefFoundError&&) noexcept = default;
    NoClassDefFoundError& operator=(const NoClassDefFoundError&) = default;
    NoClassDefFoundError& operator=(NoClassDefFoundError&&) noexcept = default;
    ~NoClassDefFoundError() override = default;

public:
	using jxx::lang::LinkageError::LinkageError;
protected:
    JXX_OBJECT_CLONE(NoClassDefFoundError)

    const char* typeName() const noexcept override { return "NoClassDefFoundError"; }
};
}
