#pragma once

#include "security/jxx.security.GeneralSecurityException.h"

namespace jxx::security {

class NoSuchProviderException
    : public ::jxx::lang::ClassBase<
          NoSuchProviderException,
          GeneralSecurityException> {
public:
    using JxxSuper = GeneralSecurityException;
    using Super = ::jxx::lang::ClassBase<
        NoSuchProviderException,
        JxxSuper>;

    NoSuchProviderException();
    explicit NoSuchProviderException(
        const ::jxx::Ptr<::jxx::lang::String>& message);
    explicit NoSuchProviderException(const char* message);
    ~NoSuchProviderException() override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object>
    cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::security
