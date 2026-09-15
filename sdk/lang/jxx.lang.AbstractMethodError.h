#pragma once
#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.IncompatibleClassChangeError.h"
namespace jxx::lang {
class AbstractMethodError : public IncompatibleClassChangeError {
public:
    using JxxSuper = IncompatibleClassChangeError;
    using JxxClassInfoMarker = jxx::lang::ClassInfo<AbstractMethodError, JxxSuper>;

    static jxx::Ptr<jxx::lang::ClassAny> Class();

    AbstractMethodError() = default;

    AbstractMethodError(const AbstractMethodError&) = default;
    AbstractMethodError(AbstractMethodError&&) noexcept = default;
    AbstractMethodError& operator=(const AbstractMethodError&) = default;
    AbstractMethodError& operator=(AbstractMethodError&&) noexcept = default;
    ~AbstractMethodError() override = default;

public:

    explicit AbstractMethodError(
        const ::jxx::Ptr<::jxx::lang::String>& message);

    explicit AbstractMethodError(const char* message);
protected:
    JXX_OBJECT_CLONE(AbstractMethodError)

    const char* typeName() const noexcept override { return "AbstractMethodError"; }
};
}
