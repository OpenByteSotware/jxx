#pragma once

#include "security/jxx.security.GeneralSecurityException.h"

namespace jxx::security {

class UnrecoverableKeyException
    : public ::jxx::lang::ClassBase<
          UnrecoverableKeyException,
          GeneralSecurityException> {
public:
    using JxxSuper = GeneralSecurityException;
    using Super = ::jxx::lang::ClassBase<UnrecoverableKeyException, JxxSuper>;

    UnrecoverableKeyException();
    explicit UnrecoverableKeyException(
        const ::jxx::Ptr<::jxx::lang::String>& message);
    explicit UnrecoverableKeyException(const char* message);
    ~UnrecoverableKeyException() override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::security
