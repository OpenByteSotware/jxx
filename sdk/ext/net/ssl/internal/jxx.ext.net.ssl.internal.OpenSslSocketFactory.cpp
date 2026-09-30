

#include <string>
#include <vector>
#include <openssl/ssl.h>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocket.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslContextConfig.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslCipherSuites.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "net/jxx.net.InetAddress.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "io/jxx.io.InputStream.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocketFactory.h"

namespace jxx::ext::net::ssl::internal {
namespace {

void validatePort(::jxx::lang::jint port) {
    if (port < 0 || port > 65535)
        throw ::jxx::lang::IllegalArgumentException();
}

void validateHost(
    const ::jxx::Ptr<::jxx::lang::String>& host) {
    if (host == nullptr)
        throw ::jxx::lang::NullPointerException();
}

void validateAddress(
    const ::jxx::Ptr<::jxx::net::InetAddress>& address) {
    if (address == nullptr)
        throw ::jxx::lang::NullPointerException();
}

} // namespace

OpenSslSocketFactory::OpenSslSocketFactory(const std::shared_ptr<OpenSslContextConfig>& config):config_(config){}

::jxx::Ptr<OpenSslSocketFactory::StringArray>
OpenSslSocketFactory::getDefaultCipherSuites() const {
    return clientDefaultCipherSuites();
}

::jxx::Ptr<OpenSslSocketFactory::StringArray>
OpenSslSocketFactory::getSupportedCipherSuites() const {
    return clientSupportedCipherSuites();
}

::jxx::Ptr<::jxx::net::Socket>
OpenSslSocketFactory::createSocket() {
    return ::jxx::NEW<OpenSslSocket>(config_);
}

::jxx::Ptr<::jxx::net::Socket>
OpenSslSocketFactory::createSocket(
    const ::jxx::Ptr<::jxx::lang::String>& host,
    ::jxx::lang::jint port) {
    validateHost(host);
    validatePort(port);
    return ::jxx::NEW<OpenSslSocket>(host, port, config_);
}

::jxx::Ptr<::jxx::net::Socket>
OpenSslSocketFactory::createSocket(
    const ::jxx::Ptr<::jxx::lang::String>& host,
    ::jxx::lang::jint port,
    const ::jxx::Ptr<::jxx::net::InetAddress>& local,
    ::jxx::lang::jint localPort) {
    validateHost(host);
    validatePort(port);
    validatePort(localPort);
    const auto socket = ::jxx::NEW<OpenSslSocket>(config_);
    socket->bind(::jxx::NEW<::jxx::net::InetSocketAddress>(local, localPort));
    socket->connect(::jxx::NEW<::jxx::net::InetSocketAddress>(host, port));
    return socket;
}

::jxx::Ptr<::jxx::net::Socket>
OpenSslSocketFactory::createSocket(
    const ::jxx::Ptr<::jxx::net::InetAddress>& host,
    ::jxx::lang::jint port) {
    validateAddress(host);
    validatePort(port);
    return createSocket(host->getHostAddress(), port);
}

::jxx::Ptr<::jxx::net::Socket>
OpenSslSocketFactory::createSocket(
    const ::jxx::Ptr<::jxx::net::InetAddress>& host,
    ::jxx::lang::jint port,
    const ::jxx::Ptr<::jxx::net::InetAddress>& local,
    ::jxx::lang::jint localPort) {
    validateAddress(host);
    validatePort(port);
    validatePort(localPort);
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
        throw ::jxx::lang::NullPointerException();
    if (port < 0 || port > 65535)
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
    if (socket == nullptr)
        throw ::jxx::lang::NullPointerException();
    std::vector<unsigned char> alreadyConsumed;
    if (consumed != nullptr) {
        constexpr ::jxx::lang::jint bufferSize = 8192;
        const auto buffer = ::jxx::NEW<
            ::jxx::lang::JxxArray<
                ::jxx::lang::jbyte,
                1U>>(bufferSize);

        for (;;) {
            const auto available = consumed->available();
            if (available <= 0) break;

            const auto requested =
                available < buffer->length
                    ? available
                    : buffer->length;
            const auto count = consumed->read(
                buffer,
                0,
                requested);

            if (count < 0) break;
            if (count == 0) break;

            alreadyConsumed.reserve(
                alreadyConsumed.size() +
                static_cast<std::size_t>(count));

            for (::jxx::lang::jint index = 0;
                 index < count;
                 ++index)
            {
                alreadyConsumed.push_back(
                    static_cast<unsigned char>(
                        (*buffer)[index]));
            }
        }
    }
    const auto address = socket->getInetAddress();
    const auto host = address == nullptr
        ? ::jxx::NEW<::jxx::lang::String>("")
        : address->getHostAddress();
    const auto layered = ::jxx::NEW<OpenSslSocket>(
        socket, host, socket->getPort(), autoClose,
        alreadyConsumed, config_);
    layered->setUseClientMode(true);
    return layered;
}

} // namespace jxx::ext::net::ssl::internal
