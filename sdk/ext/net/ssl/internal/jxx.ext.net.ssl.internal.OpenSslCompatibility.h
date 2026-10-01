#pragma once

#include <string>

#include <openssl/opensslv.h>
#include <openssl/ssl.h>

namespace jxx::ext::net::ssl::internal {

constexpr bool hasTls13Api() noexcept {
#if defined(TLS1_3_VERSION) && OPENSSL_VERSION_NUMBER >= 0x10101000L && !defined(LIBRESSL_VERSION_NUMBER)
    return true;
#else
    return false;
#endif
}

int highestSupportedTlsVersion() noexcept;
bool setTls13CipherSuites(SSL_CTX* context, const std::string& suites);
STACK_OF(SSL_CIPHER)* supportedCipherStack(SSL* ssl, bool& owned) noexcept;
X509* retainedPeerCertificate(SSL* ssl) noexcept;
bool beginRenegotiation(SSL* ssl) noexcept;

} // namespace jxx::ext::net::ssl::internal
