#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.LinkageError.h"

namespace jxx::lang {

class ExceptionInInitializerError : public LinkageError {
public:
    using JxxSuper = LinkageError;
    using JxxClassInfoMarker =
        ClassInfo<ExceptionInInitializerError, JxxSuper>;

    static ::jxx::Ptr<ClassAny> Class();

    ExceptionInInitializerError();
    explicit ExceptionInInitializerError(
        const ::jxx::Ptr<Throwable>& exception);
    explicit ExceptionInInitializerError(
        const ::jxx::Ptr<String>& message);
    explicit ExceptionInInitializerError(const char* message);

    ExceptionInInitializerError(
        const ExceptionInInitializerError&) = default;
    ExceptionInInitializerError(
        ExceptionInInitializerError&&) noexcept = default;
    ExceptionInInitializerError& operator=(
        const ExceptionInInitializerError&) = default;
    ExceptionInInitializerError& operator=(
        ExceptionInInitializerError&&) noexcept = default;
    ~ExceptionInInitializerError() override = default;

    ::jxx::Ptr<Throwable> getException() const;
    ::jxx::Ptr<Throwable> getCause() const;

protected:
    JXX_OBJECT_CLONE(ExceptionInInitializerError)
    const char* typeName() const noexcept override;

private:
    ::jxx::Ptr<Throwable> exception_;
};

} // namespace jxx::lang
