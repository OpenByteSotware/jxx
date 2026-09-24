#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.IncompatibleClassChangeError.h"
namespace jxx::lang {
class InstantiationError : public IncompatibleClassChangeError {
public:
    using JxxSuper = IncompatibleClassChangeError;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<InstantiationError, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    InstantiationError() = default;

    InstantiationError(const InstantiationError&) = default;
    InstantiationError(InstantiationError&&) noexcept = default;
    InstantiationError& operator=(const InstantiationError&) = default;
    InstantiationError& operator=(InstantiationError&&) noexcept = default;
    ~InstantiationError() override = default;

public:

    explicit InstantiationError(
        const ::jxx::Ptr<::jxx::lang::String>& message);

    explicit InstantiationError(const char* message);
protected:
    JXX_OBJECT_CLONE(InstantiationError)

    const char* typeName() const noexcept override;
};
}
