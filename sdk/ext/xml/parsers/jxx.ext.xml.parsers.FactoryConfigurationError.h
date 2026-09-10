#pragma once

#include "lang/jxx.lang.ClassInfoMarker.h"
#include "lang/jxx.lang.Error.h"

namespace jxx::ext::xml::parsers {

class FactoryConfigurationError
    : public ::jxx::lang::Error {
public:
    using JxxSuper = ::jxx::lang::Error;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<FactoryConfigurationError, JxxSuper>;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    FactoryConfigurationError();
    explicit FactoryConfigurationError(
        const ::jxx::Ptr<::jxx::lang::String>& message);

    FactoryConfigurationError(const FactoryConfigurationError&) = default;
    FactoryConfigurationError(FactoryConfigurationError&&) noexcept = default;
    FactoryConfigurationError& operator=(const FactoryConfigurationError&) = default;
    FactoryConfigurationError& operator=(FactoryConfigurationError&&) noexcept = default;
    ~FactoryConfigurationError() override;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::ext::xml::parsers
