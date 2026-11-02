#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "security/jxx.security.KeyException.h"

namespace jxx::security {

class KeyManagementException
    : public ::jxx::lang::ClassBase<
          KeyManagementException,
          KeyException> {
public:
    using JxxSuper = KeyException;
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
