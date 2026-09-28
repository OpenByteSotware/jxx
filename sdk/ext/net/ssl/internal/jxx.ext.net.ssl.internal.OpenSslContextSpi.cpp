#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslContextSpi.h"

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslContextConfig.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslProtocolPolicy.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslEngine.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslServerSocketFactory.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSessionContext.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocketFactory.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "security/jxx.security.KeyManagementException.h"

namespace jxx::ext::net::ssl::internal {

OpenSslContextSpi::OpenSslContextSpi(
    const ::jxx::Ptr<::jxx::lang::String>& protocol)
    : protocol_(protocol)
    , clientSessionContext_(::jxx::NEW<OpenSslSessionContext>())
    , serverSessionContext_(::jxx::NEW<OpenSslSessionContext>())
    , config_(std::make_shared<OpenSslContextConfig>(
          protocol_, nullptr, nullptr, nullptr,
          clientSessionContext_, serverSessionContext_)) {
    if (protocol_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

void OpenSslContextSpi::engineInit(
    const ::jxx::Ptr<KeyManagerArray>& keyManagers,
    const ::jxx::Ptr<TrustManagerArray>& trustManagers,
    const ::jxx::Ptr<::jxx::security::SecureRandom>& secureRandom) {
    try {
        config_ = std::make_shared<OpenSslContextConfig>(
            protocol_, keyManagers, trustManagers, secureRandom,
            clientSessionContext_, serverSessionContext_);
    } catch (...) {
        throw ::jxx::security::KeyManagementException(
            "Unable to initialize SSL context");
    }
}

::jxx::Ptr<::jxx::ext::net::ssl::SSLSocketFactory>
OpenSslContextSpi::engineGetSocketFactory() {
    return ::jxx::NEW<OpenSslSocketFactory>(config_);
}
::jxx::Ptr<::jxx::ext::net::ssl::SSLServerSocketFactory>
OpenSslContextSpi::engineGetServerSocketFactory() {
    return ::jxx::NEW<OpenSslServerSocketFactory>(config_);
}
::jxx::Ptr<::jxx::ext::net::ssl::SSLSessionContext>
OpenSslContextSpi::engineGetClientSessionContext() {
    return clientSessionContext_;
}
::jxx::Ptr<::jxx::ext::net::ssl::SSLSessionContext>
OpenSslContextSpi::engineGetServerSessionContext() {
    return serverSessionContext_;
}
::jxx::Ptr<::jxx::ext::net::ssl::SSLEngine>
OpenSslContextSpi::engineCreateSSLEngine() {
    return ::jxx::NEW<OpenSslEngine>(config_, nullptr, -1);
}
::jxx::Ptr<::jxx::ext::net::ssl::SSLEngine>
OpenSslContextSpi::engineCreateSSLEngine(
    const ::jxx::Ptr<::jxx::lang::String>& peerHost,
    ::jxx::lang::jint peerPort) {
    if (peerHost == nullptr) throw ::jxx::lang::NullPointerException();
    if (peerPort < 0 || peerPort > 65535)
        throw ::jxx::lang::IllegalArgumentException();
    return ::jxx::NEW<OpenSslEngine>(config_, peerHost, peerPort);
}

namespace {
::jxx::Ptr<::jxx::ext::net::ssl::SSLParameters> parameters(
    const ::jxx::Ptr<::jxx::ext::net::ssl::SSLSocketFactory>& factory,
    const ::jxx::Ptr<::jxx::lang::String>& protocol,
    bool supported) {
    const auto result = ::jxx::NEW<::jxx::ext::net::ssl::SSLParameters>();
    result->setCipherSuites(supported
        ? factory->getSupportedCipherSuites()
        : factory->getDefaultCipherSuites());
    result->setProtocols(contextProtocols(protocol));
    return result;
}
} // namespace

::jxx::Ptr<::jxx::ext::net::ssl::SSLParameters>
OpenSslContextSpi::engineGetDefaultSSLParameters() {
    return parameters(engineGetSocketFactory(), protocol_, false);
}
::jxx::Ptr<::jxx::ext::net::ssl::SSLParameters>
OpenSslContextSpi::engineGetSupportedSSLParameters() {
    return parameters(engineGetSocketFactory(), protocol_, true);
}

} // namespace jxx::ext::net::ssl::internal
