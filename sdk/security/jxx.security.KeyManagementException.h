#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "security/jxx.security.GeneralSecurityException.h"

namespace jxx::security {

class KeyManagementException
    : public ::jxx::lang::ClassBase<
          KeyManagementException,
          GeneralSecurityException> {
public:
    using JxxSuper = GeneralSecurityException;
    using Super = ::jxx::lang::ClassBase<
        KeyManagementException,
        JxxSuper>;

    KeyManagementException();
    explicit KeyManagementException(
        const ::jxx::Ptr<::jxx::lang::String>& message);
    explicit KeyManagementException(const char* message);
    ~KeyManagementException() override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object>
    cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::security
