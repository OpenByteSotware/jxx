#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.Throwable.h"

namespace jxx::lang {

class Error : public Throwable {
public:
    using JxxSuper = Throwable;
    using JxxClassInfoMarker = ClassInfo<Error, JxxSuper>;

    static ::jxx::Ptr<ClassAny> Class();

    Error();
    explicit Error(const ::jxx::Ptr<String>& message);
    explicit Error(const char* message);
    Error(
        const ::jxx::Ptr<String>& message,
        const ::jxx::Ptr<Throwable>& cause);
    explicit Error(const ::jxx::Ptr<Throwable>& cause);

    Error(const Error&) = default;
    Error(Error&&) noexcept = default;
    Error& operator=(const Error&) = default;
    Error& operator=(Error&&) noexcept = default;
    ~Error() override = default;

protected:
    Error(
        const ::jxx::Ptr<String>& message,
        const ::jxx::Ptr<Throwable>& cause,
        jbool enableSuppression,
        jbool writableStackTrace);

    JXX_OBJECT_CLONE(Error)
    const char* typeName() const noexcept override;
};

} // namespace jxx::lang
