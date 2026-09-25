#pragma once

#include "ext/net/ssl/jxx.ext.net.ssl.SSLException.h"

namespace jxx::ext::net::ssl {

class SSLHandshakeException
    : public ::jxx::lang::ClassBase<
          SSLHandshakeException,
          SSLException> {
public:
    using JxxSuper = SSLException;
    using Super = ::jxx::lang::ClassBase<SSLHandshakeException, JxxSuper>;

    SSLHandshakeException();
    explicit SSLHandshakeException(const ::jxx::Ptr<::jxx::lang::String>& message);
    explicit SSLHandshakeException(const char* message);
    ~SSLHandshakeException() override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::ext::net::ssl
