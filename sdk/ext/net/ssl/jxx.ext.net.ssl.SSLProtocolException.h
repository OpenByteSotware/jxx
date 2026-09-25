#pragma once

#include "ext/net/ssl/jxx.ext.net.ssl.SSLException.h"

namespace jxx::ext::net::ssl {

class SSLProtocolException
    : public ::jxx::lang::ClassBase<
          SSLProtocolException,
          SSLException> {
public:
    using JxxSuper = SSLException;
    using Super = ::jxx::lang::ClassBase<SSLProtocolException, JxxSuper>;

    SSLProtocolException();
    explicit SSLProtocolException(const ::jxx::Ptr<::jxx::lang::String>& message);
    explicit SSLProtocolException(const char* message);
    ~SSLProtocolException() override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::ext::net::ssl
