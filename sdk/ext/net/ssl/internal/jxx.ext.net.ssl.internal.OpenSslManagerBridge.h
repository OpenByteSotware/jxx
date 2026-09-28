#pragma once

#include <memory>

#include <openssl/ssl.h>
#include <openssl/x509_vfy.h>

#include "security/jxx.security.AlgorithmConstraints.h"

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslContextConfig.h"

namespace jxx::ext::net::ssl {
class SSLEngine;
class SSLSocket;
}

namespace jxx::ext::net::ssl::internal {

class OpenSslManagerBridge final {
public:
    explicit OpenSslManagerBridge(
        const std::shared_ptr<OpenSslContextConfig>& config,
        const ::jxx::Ptr<::jxx::security::AlgorithmConstraints>&
            algorithmConstraints = nullptr);

    void setSocket(
        const ::jxx::Ptr<
            ::jxx::ext::net::ssl::SSLSocket>& socket);

    void setEngine(
        const ::jxx::Ptr<
            ::jxx::ext::net::ssl::SSLEngine>& engine);

    int verifyPeer(
        X509_STORE_CTX* storeContext)
        noexcept;

    int selectServerIdentity(
        SSL* ssl)
        noexcept;

    int selectClientCertificate(
        SSL* ssl,
        X509** certificate,
        EVP_PKEY** privateKey)
        noexcept;

private:
    std::shared_ptr<OpenSslContextConfig>
    config_;

    ::jxx::Ptr<::jxx::security::AlgorithmConstraints>
    algorithmConstraints_;

    ::jxx::Ptr<
        ::jxx::ext::net::ssl::SSLSocket>
    socket_;

    ::jxx::Ptr<
        ::jxx::ext::net::ssl::SSLEngine>
    engine_;
};

int openSslVerifyCallback(
    X509_STORE_CTX* storeContext,
    void* argument)
    noexcept;

int openSslClientCertificateCallback(
    SSL* ssl,
    X509** certificate,
    EVP_PKEY** privateKey)
    noexcept;

int openSslManagerBridgeExDataIndex();

} // namespace jxx::ext::net::ssl::internal
