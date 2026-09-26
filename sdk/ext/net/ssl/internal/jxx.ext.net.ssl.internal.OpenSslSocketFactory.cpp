#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocketFactory.h"

#include <openssl/ssl.h>

#include <string>
#include <vector>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocket.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslContextConfig.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "net/jxx.net.InetAddress.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "io/jxx.io.InputStream.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::ext::net::ssl::internal {
OpenSslSocketFactory::OpenSslSocketFactory(const std::shared_ptr<OpenSslContextConfig>& config):config_(config){}
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
    return ::jxx::NEW<OpenSslSocket>(config_);
}

::jxx::Ptr<::jxx::net::Socket>
OpenSslSocketFactory::createSocket(
    const ::jxx::Ptr<::jxx::lang::String>& host,
    ::jxx::lang::jint port) {
    return ::jxx::NEW<OpenSslSocket>(host, port, config_);
}

::jxx::Ptr<::jxx::net::Socket>
OpenSslSocketFactory::createSocket(
    const ::jxx::Ptr<::jxx::lang::String>& host,
    ::jxx::lang::jint port,
    const ::jxx::Ptr<::jxx::net::InetAddress>& local,
    ::jxx::lang::jint localPort) {
    const auto socket = ::jxx::NEW<OpenSslSocket>(config_);
    socket->bind(::jxx::NEW<::jxx::net::InetSocketAddress>(local, localPort));
    socket->connect(::jxx::NEW<::jxx::net::InetSocketAddress>(host, port));
    return socket;
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
    const ::jxx::Ptr<::jxx::net::InetAddress>& local,
    ::jxx::lang::jint localPort) {
    const auto socket = ::jxx::NEW<OpenSslSocket>(config_);
    socket->bind(::jxx::NEW<::jxx::net::InetSocketAddress>(local, localPort));
    socket->connect(::jxx::NEW<::jxx::net::InetSocketAddress>(host, port));
    return socket;
}

::jxx::Ptr<::jxx::net::Socket>
OpenSslSocketFactory::createSocket(
    const ::jxx::Ptr<::jxx::net::Socket>& socket,
    const ::jxx::Ptr<::jxx::lang::String>& host,
    ::jxx::lang::jint port,
    ::jxx::lang::jbool autoClose) {
    if (socket == nullptr || host == nullptr)
        throw ::jxx::lang::IllegalArgumentException();
    return ::jxx::NEW<OpenSslSocket>(
        socket, host, port, autoClose,
        std::vector<unsigned char>(), config_);
}

::jxx::Ptr<::jxx::net::Socket>
OpenSslSocketFactory::createSocket(
    const ::jxx::Ptr<::jxx::net::Socket>& socket,
    const ::jxx::Ptr<::jxx::io::InputStream>& consumed,
    ::jxx::lang::jbool autoClose) {
    if (socket == nullptr || consumed == nullptr)
        throw ::jxx::lang::IllegalArgumentException();
    std::vector<unsigned char> alreadyConsumed;
    const auto available = consumed->available();
    if (available < 0)
        throw ::jxx::lang::IllegalArgumentException();
    if (available > 0) {
        const auto buffer = ::jxx::NEW<
            ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(available);
        ::jxx::lang::jint remaining = available;
        while (remaining > 0) {
            const auto count = consumed->read(
                buffer,
                0,
                remaining < buffer->length
                    ? remaining
                    : buffer->length);
            if (count < 0) break;
            if (count == 0) break;
            remaining -= count;
            for (::jxx::lang::jint index = 0; index < count; ++index)
                alreadyConsumed.push_back(
                    static_cast<unsigned char>((*buffer)[index]));
        }
    }
    const auto address = socket->getInetAddress();
    const auto host = address == nullptr
        ? ::jxx::NEW<::jxx::lang::String>("")
        : address->getHostAddress();
    return ::jxx::NEW<OpenSslSocket>(
        socket, host, socket->getPort(), autoClose,
        alreadyConsumed, config_);
}

} // namespace jxx::ext::net::ssl::internal
