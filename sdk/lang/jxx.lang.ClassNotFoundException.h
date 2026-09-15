#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.ReflectiveOperationException.h"

namespace jxx::lang {

class ClassNotFoundException : public ReflectiveOperationException {
public:
    using JxxSuper = ReflectiveOperationException;
    using JxxClassInfoMarker =
        ClassInfo<ClassNotFoundException, JxxSuper>;

    static ::jxx::Ptr<ClassAny> Class();

    ClassNotFoundException();
    explicit ClassNotFoundException(const ::jxx::Ptr<String>& message);
    explicit ClassNotFoundException(const char* message);
    ClassNotFoundException(
        const ::jxx::Ptr<String>& message,
        const ::jxx::Ptr<Throwable>& exception);

    ClassNotFoundException(const ClassNotFoundException&) = default;
    ClassNotFoundException(ClassNotFoundException&&) noexcept = default;
    ClassNotFoundException& operator=(const ClassNotFoundException&) = default;
    ClassNotFoundException& operator=(ClassNotFoundException&&) noexcept = default;
    ~ClassNotFoundException() override = default;

    ::jxx::Ptr<Throwable> getException() const;
    ::jxx::Ptr<Throwable> getCause() const;

protected:
    JXX_OBJECT_CLONE(ClassNotFoundException)
    const char* typeName() const noexcept override;

private:
    ::jxx::Ptr<Throwable> exception_;
};

} // namespace jxx::lang
