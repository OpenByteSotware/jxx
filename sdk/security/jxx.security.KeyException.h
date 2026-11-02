#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "security/jxx.security.GeneralSecurityException.h"

namespace jxx::security {

class KeyException
    : public ::jxx::lang::ClassBase<KeyException, GeneralSecurityException> {
public:
    using JxxSuper = GeneralSecurityException;
    using Super = ::jxx::lang::ClassBase<KeyException, JxxSuper>;

    KeyException();
    explicit KeyException(const ::jxx::Ptr<::jxx::lang::String>& message);
    KeyException(
        const ::jxx::Ptr<::jxx::lang::String>& message,
        const ::jxx::Ptr<::jxx::lang::Throwable>& cause);
    explicit KeyException(const ::jxx::Ptr<::jxx::lang::Throwable>& cause);
    explicit KeyException(const char* message);
    ~KeyException() override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::security
