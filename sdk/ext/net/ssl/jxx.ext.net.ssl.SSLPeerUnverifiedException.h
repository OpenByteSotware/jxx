#pragma once

#include "ext/net/ssl/jxx.ext.net.ssl.SSLException.h"

namespace jxx::ext::net::ssl {

class SSLPeerUnverifiedException
    : public ::jxx::lang::ClassBase<
          SSLPeerUnverifiedException,
          SSLException> {
public:
    using JxxSuper = SSLException;
    using Super = ::jxx::lang::ClassBase<SSLPeerUnverifiedException, JxxSuper>;

    SSLPeerUnverifiedException();
    explicit SSLPeerUnverifiedException(const ::jxx::Ptr<::jxx::lang::String>& message);
    explicit SSLPeerUnverifiedException(const char* message);
    ~SSLPeerUnverifiedException() override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::ext::net::ssl
