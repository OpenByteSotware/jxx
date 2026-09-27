#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "security/jxx.security.GeneralSecurityException.h"

namespace jxx::security {

class NoSuchAlgorithmException
    : public ::jxx::lang::ClassBase<
          NoSuchAlgorithmException,
          GeneralSecurityException> {
public:
    using JxxSuper = GeneralSecurityException;
    using Super = ::jxx::lang::ClassBase<
        NoSuchAlgorithmException,
        JxxSuper>;

    NoSuchAlgorithmException();
    explicit NoSuchAlgorithmException(
        const ::jxx::Ptr<::jxx::lang::String>& message);
    explicit NoSuchAlgorithmException(const char* message);
    ~NoSuchAlgorithmException() override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object>
    cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::security
