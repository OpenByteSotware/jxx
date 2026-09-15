#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.Error.h"
#include "lang/jxx_types.h"

namespace jxx::lang {

class AssertionError : public Error {
public:
    using JxxSuper = Error;
    using JxxClassInfoMarker = ClassInfo<AssertionError, JxxSuper>;

    static ::jxx::Ptr<ClassAny> Class();

    AssertionError();
    explicit AssertionError(jbool detailMessage);
    explicit AssertionError(jchar detailMessage);
    explicit AssertionError(jdouble detailMessage);
    explicit AssertionError(jfloat detailMessage);
    explicit AssertionError(jint detailMessage);
    explicit AssertionError(jlong detailMessage);
    explicit AssertionError(const ::jxx::Ptr<Object>& detailMessage);
    explicit AssertionError(const char* detailMessage);
    AssertionError(
        const ::jxx::Ptr<String>& message,
        const ::jxx::Ptr<Throwable>& cause);

    AssertionError(const AssertionError&) = default;
    AssertionError(AssertionError&&) noexcept = default;
    AssertionError& operator=(const AssertionError&) = default;
    AssertionError& operator=(AssertionError&&) noexcept = default;
    ~AssertionError() override = default;

protected:
    JXX_OBJECT_CLONE(AssertionError)
    const char* typeName() const noexcept override;
};

} // namespace jxx::lang
