#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslCompatibility.h"

namespace jxx::ext::net::ssl::internal {

int highestSupportedTlsVersion() noexcept {
#if defined(TLS1_3_VERSION) && OPENSSL_VERSION_NUMBER >= 0x10101000L && !defined(LIBRESSL_VERSION_NUMBER)
    return TLS1_3_VERSION;
#else
    return TLS1_2_VERSION;
#endif
}

bool setTls13CipherSuites(
    SSL_CTX* context,
    const std::string& suites) {
#if defined(TLS1_3_VERSION) && OPENSSL_VERSION_NUMBER >= 0x10101000L && !defined(LIBRESSL_VERSION_NUMBER)
    return context != nullptr &&
        SSL_CTX_set_ciphersuites(context, suites.c_str()) == 1;
#else
    (void)context;
    (void)suites;
    return false;
#endif
}

STACK_OF(SSL_CIPHER)* supportedCipherStack(
    SSL* ssl,
    bool& owned) noexcept {
#if OPENSSL_VERSION_NUMBER >= 0x10100000L && !defined(LIBRESSL_VERSION_NUMBER)
    owned = true;
    return ssl == nullptr ? nullptr : SSL_get1_supported_ciphers(ssl);
#else
    owned = false;
    return ssl == nullptr ? nullptr : SSL_get_ciphers(ssl);
#endif
}

X509* retainedPeerCertificate(SSL* ssl) noexcept {
    return ssl == nullptr ? nullptr : SSL_get_peer_certificate(ssl);
}

bool beginRenegotiation(SSL* ssl) noexcept {
#if !defined(OPENSSL_NO_RENEGOTIATION)
    return ssl != nullptr && SSL_renegotiate(ssl) == 1;
#else
    (void)ssl;
    return false;
#endif
}

} // namespace jxx::ext::net::ssl::internal
