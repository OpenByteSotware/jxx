#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.LinkageError.h"
namespace jxx::lang {
class UnsatisfiedLinkError : public LinkageError {
public:
    using JxxSuper = LinkageError;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<UnsatisfiedLinkError, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    UnsatisfiedLinkError() = default;

    UnsatisfiedLinkError(const UnsatisfiedLinkError&) = default;
    UnsatisfiedLinkError(UnsatisfiedLinkError&&) noexcept = default;
    UnsatisfiedLinkError& operator=(const UnsatisfiedLinkError&) = default;
    UnsatisfiedLinkError& operator=(UnsatisfiedLinkError&&) noexcept = default;
    ~UnsatisfiedLinkError() override = default;

public:

    explicit UnsatisfiedLinkError(
        const ::jxx::Ptr<::jxx::lang::String>& message);

    explicit UnsatisfiedLinkError(const char* message);
protected:
    JXX_OBJECT_CLONE(UnsatisfiedLinkError)

    const char* typeName() const noexcept override;
};
}
