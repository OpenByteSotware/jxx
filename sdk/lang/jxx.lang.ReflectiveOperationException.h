#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.Exception.h"

namespace jxx::lang {

class ReflectiveOperationException : public Exception {
public:
    using JxxSuper = Exception;
    using JxxClassInfoMarker =
        ClassInfo<ReflectiveOperationException, JxxSuper>;

    static ::jxx::Ptr<ClassAny> Class();

    ReflectiveOperationException();
    explicit ReflectiveOperationException(
        const ::jxx::Ptr<String>& message);
    explicit ReflectiveOperationException(const char* message);
    ReflectiveOperationException(
        const ::jxx::Ptr<String>& message,
        const ::jxx::Ptr<Throwable>& cause);
    explicit ReflectiveOperationException(
        const ::jxx::Ptr<Throwable>& cause);

    ReflectiveOperationException(
        const ReflectiveOperationException&) = default;
    ReflectiveOperationException(
        ReflectiveOperationException&&) noexcept = default;
    ReflectiveOperationException& operator=(
        const ReflectiveOperationException&) = default;
    ReflectiveOperationException& operator=(
        ReflectiveOperationException&&) noexcept = default;
    ~ReflectiveOperationException() override = default;

protected:
    JXX_OBJECT_CLONE(ReflectiveOperationException)
    const char* typeName() const noexcept override;
};

} // namespace jxx::lang
