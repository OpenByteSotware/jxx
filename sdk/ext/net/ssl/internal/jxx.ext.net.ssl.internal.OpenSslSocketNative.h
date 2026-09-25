#pragma once
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <memory>
namespace jxx::ext::net::ssl::internal {
class OpenSslManagerBridge;
class OpenSslSocketNative final {
public:
    OpenSslSocketNative() = default;
    ~OpenSslSocketNative();
    OpenSslSocketNative(const OpenSslSocketNative&) = delete;
    OpenSslSocketNative& operator=(const OpenSslSocketNative&) = delete;
    SSL_CTX* context = nullptr;
    BIO* connection = nullptr;
    std::unique_ptr<OpenSslManagerBridge> managerBridge;
};
}
