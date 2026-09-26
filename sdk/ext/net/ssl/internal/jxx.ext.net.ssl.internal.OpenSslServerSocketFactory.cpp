#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslServerSocketFactory.h"

#include <openssl/ssl.h>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslServerSocket.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::ext::net::ssl::internal {
namespace {
::jxx::Ptr<OpenSslServerSocketFactory::StringArray> cipherSuites() {
    SSL_CTX* context = SSL_CTX_new(TLS_server_method());
    if (context == nullptr) throw ::jxx::io::IOException("SSL_CTX_new failed");
    STACK_OF(SSL_CIPHER)* ciphers = SSL_CTX_get_ciphers(context);
    const int count = ciphers == nullptr ? 0 : sk_SSL_CIPHER_num(ciphers);
    const auto result = ::jxx::NEW<OpenSslServerSocketFactory::StringArray>(count);
    for (int index = 0; index < count; ++index) {
        const SSL_CIPHER* cipher = sk_SSL_CIPHER_value(ciphers, index);
        const char* name = cipher == nullptr ? nullptr : SSL_CIPHER_get_name(cipher);
        (*result)[index] = ::jxx::NEW<::jxx::lang::String>(name == nullptr ? "" : name);
    }
    SSL_CTX_free(context);
    return result;
}
void validate(::jxx::lang::jint port, ::jxx::lang::jint backlog) {
    if (port < 0 || port > 65535 || backlog < 1)
        throw ::jxx::lang::IllegalArgumentException();
}
} // namespace
OpenSslServerSocketFactory::OpenSslServerSocketFactory(
    const std::shared_ptr<OpenSslContextConfig>& config) : config_(config) {}
::jxx::Ptr<OpenSslServerSocketFactory::StringArray>
OpenSslServerSocketFactory::getDefaultCipherSuites() const { return cipherSuites(); }
::jxx::Ptr<OpenSslServerSocketFactory::StringArray>
OpenSslServerSocketFactory::getSupportedCipherSuites() const { return cipherSuites(); }
::jxx::Ptr<::jxx::net::ServerSocket>
OpenSslServerSocketFactory::createServerSocket() {
    return ::jxx::NEW<OpenSslServerSocket>(0, 50, nullptr, config_);
}
::jxx::Ptr<::jxx::net::ServerSocket>
OpenSslServerSocketFactory::createServerSocket(::jxx::lang::jint port) {
    validate(port, 50); return ::jxx::NEW<OpenSslServerSocket>(port, 50, nullptr, config_);
}
::jxx::Ptr<::jxx::net::ServerSocket>
OpenSslServerSocketFactory::createServerSocket(::jxx::lang::jint port, ::jxx::lang::jint backlog) {
    validate(port, backlog); return ::jxx::NEW<OpenSslServerSocket>(port, backlog, nullptr, config_);
}
::jxx::Ptr<::jxx::net::ServerSocket>
OpenSslServerSocketFactory::createServerSocket(
    ::jxx::lang::jint port, ::jxx::lang::jint backlog,
    const ::jxx::Ptr<::jxx::net::InetAddress>& address) {
    validate(port, backlog); return ::jxx::NEW<OpenSslServerSocket>(port, backlog, address, config_);
}
} // namespace jxx::ext::net::ssl::internal
