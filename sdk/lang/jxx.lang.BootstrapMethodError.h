#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.LinkageError.h"

namespace jxx::lang {

class BootstrapMethodError : public LinkageError {
public:
    using JxxSuper = LinkageError;
    using JxxClassInfoMarker = ClassInfo<BootstrapMethodError, JxxSuper>;

    static ::jxx::Ptr<ClassAny> Class();

    BootstrapMethodError();
    explicit BootstrapMethodError(const ::jxx::Ptr<String>& message);
    explicit BootstrapMethodError(const char* message);
    explicit BootstrapMethodError(const ::jxx::Ptr<Throwable>& cause);
    BootstrapMethodError(
        const ::jxx::Ptr<String>& message,
        const ::jxx::Ptr<Throwable>& cause);

    BootstrapMethodError(const BootstrapMethodError&) = default;
    BootstrapMethodError(BootstrapMethodError&&) noexcept = default;
    BootstrapMethodError& operator=(const BootstrapMethodError&) = default;
    BootstrapMethodError& operator=(BootstrapMethodError&&) noexcept = default;
    ~BootstrapMethodError() override = default;

protected:
    JXX_OBJECT_CLONE(BootstrapMethodError)
    const char* typeName() const noexcept override;
};

} // namespace jxx::lang
