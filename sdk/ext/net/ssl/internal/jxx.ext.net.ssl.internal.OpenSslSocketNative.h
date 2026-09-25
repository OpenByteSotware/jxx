#pragma once
#include <openssl/bio.h>
#include <openssl/ssl.h>
namespace jxx::ext::net::ssl::internal {
class OpenSslSocketNative final {
public:
    OpenSslSocketNative() = default;
    ~OpenSslSocketNative();
    OpenSslSocketNative(const OpenSslSocketNative&) = delete;
    OpenSslSocketNative& operator=(const OpenSslSocketNative&) = delete;
    SSL_CTX* context = nullptr;
    BIO* connection = nullptr;
};
}
