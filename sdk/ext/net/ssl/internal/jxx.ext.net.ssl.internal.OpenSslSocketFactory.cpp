#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocketFactory.h"

#include <openssl/ssl.h>

#include <string>
#include <vector>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocket.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "net/jxx.net.InetAddress.h"

namespace jxx::ext::net::ssl::internal {
namespace {

::jxx::Ptr<OpenSslSocketFactory::StringArray> cipherSuites() {
    SSL_CTX* context = SSL_CTX_new(TLS_client_method());
    if (context == nullptr) {
        return ::jxx::NEW<OpenSslSocketFactory::StringArray>(0);
    }

    STACK_OF(SSL_CIPHER)* ciphers = SSL_CTX_get_ciphers(context);
    const int count = ciphers == nullptr ? 0 : sk_SSL_CIPHER_num(ciphers);
    const auto result =
        ::jxx::NEW<OpenSslSocketFactory::StringArray>(count);

    for (int index = 0; index < count; ++index) {
        const SSL_CIPHER* cipher = sk_SSL_CIPHER_value(ciphers, index);
        const char* name = cipher == nullptr ? nullptr : SSL_CIPHER_get_name(cipher);
        (*result)[index] = ::jxx::NEW<::jxx::lang::String>(
            name == nullptr ? "" : name);
    }

    SSL_CTX_free(context);
    return result;
}

} // namespace

::jxx::Ptr<OpenSslSocketFactory::StringArray>
OpenSslSocketFactory::getDefaultCipherSuites() const {
    return cipherSuites();
}

::jxx::Ptr<OpenSslSocketFactory::StringArray>
OpenSslSocketFactory::getSupportedCipherSuites() const {
    return cipherSuites();
}

::jxx::Ptr<::jxx::net::Socket>
OpenSslSocketFactory::createSocket() {
    throw ::jxx::lang::UnsupportedOperationException(
        ::jxx::NEW<::jxx::lang::String>(
            "Unconnected OpenSSL sockets are not implemented"));
}

::jxx::Ptr<::jxx::net::Socket>
OpenSslSocketFactory::createSocket(
    const ::jxx::Ptr<::jxx::lang::String>& host,
    ::jxx::lang::jint port) {
    return ::jxx::NEW<OpenSslSocket>(host, port);
}

::jxx::Ptr<::jxx::net::Socket>
OpenSslSocketFactory::createSocket(
    const ::jxx::Ptr<::jxx::lang::String>& host,
    ::jxx::lang::jint port,
    const ::jxx::Ptr<::jxx::net::InetAddress>&,
    ::jxx::lang::jint) {
    return createSocket(host, port);
}

::jxx::Ptr<::jxx::net::Socket>
OpenSslSocketFactory::createSocket(
    const ::jxx::Ptr<::jxx::net::InetAddress>& host,
    ::jxx::lang::jint port) {
    return createSocket(host->getHostAddress(), port);
}

::jxx::Ptr<::jxx::net::Socket>
OpenSslSocketFactory::createSocket(
    const ::jxx::Ptr<::jxx::net::InetAddress>& host,
    ::jxx::lang::jint port,
    const ::jxx::Ptr<::jxx::net::InetAddress>&,
    ::jxx::lang::jint) {
    return createSocket(host, port);
}

::jxx::Ptr<::jxx::net::Socket>
OpenSslSocketFactory::createSocket(
    const ::jxx::Ptr<::jxx::net::Socket>&,
    const ::jxx::Ptr<::jxx::lang::String>& host,
    ::jxx::lang::jint port,
    ::jxx::lang::jbool) {
    return createSocket(host, port);
}

} // namespace jxx::ext::net::ssl::internal
