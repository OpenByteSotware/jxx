#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.Error.h"

namespace jxx::lang {
class String;
class Throwable;
}

namespace jxx::util {

class ServiceConfigurationError final : public ::jxx::lang::Error {
public:
    using JxxSuper = ::jxx::lang::Error;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<ServiceConfigurationError, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    explicit ServiceConfigurationError(
        const ::jxx::Ptr<::jxx::lang::String>& message);
    ServiceConfigurationError(
        const ::jxx::Ptr<::jxx::lang::String>& message,
        const ::jxx::Ptr<::jxx::lang::Throwable>& cause);

    ServiceConfigurationError(const ServiceConfigurationError&) = default;
    ServiceConfigurationError(ServiceConfigurationError&&) noexcept = default;
    ServiceConfigurationError& operator=(const ServiceConfigurationError&) = default;
    ServiceConfigurationError& operator=(ServiceConfigurationError&&) noexcept = default;
    ~ServiceConfigurationError() override = default;

protected:
    JXX_OBJECT_CLONE(ServiceConfigurationError)
    const char* typeName() const noexcept override;
};

} // namespace jxx::util
