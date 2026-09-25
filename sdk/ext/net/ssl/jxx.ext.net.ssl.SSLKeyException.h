#pragma once

#include "ext/net/ssl/jxx.ext.net.ssl.SSLException.h"

namespace jxx::ext::net::ssl {

class SSLKeyException
    : public ::jxx::lang::ClassBase<
          SSLKeyException,
          SSLException> {
public:
    using JxxSuper = SSLException;
    using Super = ::jxx::lang::ClassBase<SSLKeyException, JxxSuper>;

    SSLKeyException();
    explicit SSLKeyException(const ::jxx::Ptr<::jxx::lang::String>& message);
    explicit SSLKeyException(const char* message);
    ~SSLKeyException() override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::ext::net::ssl
