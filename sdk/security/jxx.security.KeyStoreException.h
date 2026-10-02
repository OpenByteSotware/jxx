#pragma once

#include "security/jxx.security.GeneralSecurityException.h"

namespace jxx::security {

class KeyStoreException
    : public ::jxx::lang::ClassBase<
          KeyStoreException,
          GeneralSecurityException> {
public:
    using JxxSuper = GeneralSecurityException;
    using Super = ::jxx::lang::ClassBase<KeyStoreException, JxxSuper>;

    KeyStoreException();
    explicit KeyStoreException(
        const ::jxx::Ptr<::jxx::lang::String>& message);
    explicit KeyStoreException(const char* message);
    ~KeyStoreException() override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::security
