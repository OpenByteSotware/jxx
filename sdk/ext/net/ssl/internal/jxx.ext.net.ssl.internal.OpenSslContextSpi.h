#pragma once

#include <memory>

#include "ext/net/ssl/jxx.ext.net.ssl.SSLContextSpi.h"

namespace jxx::ext::net::ssl::internal {

class OpenSslContextConfig;
class OpenSslSessionContext;

class OpenSslContextSpi final
    : public ::jxx::lang::ClassBase<
          OpenSslContextSpi,
          ::jxx::ext::net::ssl::SSLContextSpi> {
public:
    using JxxSuper = ::jxx::ext::net::ssl::SSLContextSpi;
    using Super = ::jxx::lang::ClassBase<OpenSslContextSpi, JxxSuper>;

    explicit OpenSslContextSpi(
        const ::jxx::Ptr<::jxx::lang::String>& protocol);

    void engineInit(
        const ::jxx::Ptr<KeyManagerArray>& keyManagers,
        const ::jxx::Ptr<TrustManagerArray>& trustManagers,
        const ::jxx::Ptr<::jxx::security::SecureRandom>& secureRandom) override;

    ::jxx::Ptr<::jxx::ext::net::ssl::SSLSocketFactory>
    engineGetSocketFactory() override;
    ::jxx::Ptr<::jxx::ext::net::ssl::SSLServerSocketFactory>
    engineGetServerSocketFactory() override;
    ::jxx::Ptr<::jxx::ext::net::ssl::SSLSessionContext>
    engineGetClientSessionContext() override;
    ::jxx::Ptr<::jxx::ext::net::ssl::SSLSessionContext>
    engineGetServerSessionContext() override;
    ::jxx::Ptr<::jxx::ext::net::ssl::SSLEngine>
    engineCreateSSLEngine() override;
    ::jxx::Ptr<::jxx::ext::net::ssl::SSLEngine>
    engineCreateSSLEngine(
        const ::jxx::Ptr<::jxx::lang::String>& peerHost,
        ::jxx::lang::jint peerPort) override;
    ::jxx::Ptr<::jxx::ext::net::ssl::SSLParameters>
    engineGetDefaultSSLParameters() override;
    ::jxx::Ptr<::jxx::ext::net::ssl::SSLParameters>
    engineGetSupportedSSLParameters() override;

private:
    ::jxx::Ptr<::jxx::lang::String> protocol_;
    ::jxx::Ptr<OpenSslSessionContext> clientSessionContext_;
    ::jxx::Ptr<OpenSslSessionContext> serverSessionContext_;
    std::shared_ptr<OpenSslContextConfig> config_;
};

} // namespace jxx::ext::net::ssl::internal
