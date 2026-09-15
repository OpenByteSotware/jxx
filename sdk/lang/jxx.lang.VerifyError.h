#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.LinkageError.h"
namespace jxx::lang {
class VerifyError : public LinkageError {
public:
    using JxxSuper = LinkageError;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<VerifyError, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    VerifyError() = default;

    VerifyError(const VerifyError&) = default;
    VerifyError(VerifyError&&) noexcept = default;
    VerifyError& operator=(const VerifyError&) = default;
    VerifyError& operator=(VerifyError&&) noexcept = default;
    ~VerifyError() override = default;

public:

    explicit VerifyError(
        const ::jxx::Ptr<::jxx::lang::String>& message);

    explicit VerifyError(const char* message);
protected:
    JXX_OBJECT_CLONE(VerifyError)

    const char* typeName() const noexcept override { return "VerifyError"; }
};
}
