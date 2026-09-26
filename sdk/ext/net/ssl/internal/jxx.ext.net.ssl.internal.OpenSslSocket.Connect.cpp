#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocket.h"

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.Socket.h"
#include "net/jxx.net.SocketAddress.h"

namespace jxx::ext::net::ssl::internal {

void OpenSslSocket::bind(
    const ::jxx::Ptr<::jxx::net::SocketAddress>& bindpoint) {
    if (session_ != nullptr)
        throw ::jxx::lang::IllegalStateException(
            "SSL socket is already handshaking or connected");

    if (transport_ != nullptr) {
        transport_->bind(bindpoint);
        return;
    }

    if (pendingTransport_ == nullptr)
        pendingTransport_ = ::jxx::NEW<::jxx::net::Socket>();

    pendingTransport_->bind(bindpoint);
}

void OpenSslSocket::connect(
    const ::jxx::Ptr<::jxx::net::SocketAddress>& endpoint) {
    connect(endpoint, 0);
}

void OpenSslSocket::connect(
    const ::jxx::Ptr<::jxx::net::SocketAddress>& endpoint,
    ::jxx::lang::jint timeout) {
    if (endpoint == nullptr)
        throw ::jxx::lang::IllegalArgumentException(
            "endpoint is null");
    if (timeout < 0)
        throw ::jxx::lang::IllegalArgumentException(
            "timeout is negative");
    if (session_ != nullptr)
        throw ::jxx::lang::IllegalStateException(
            "SSL socket handshake has already started");
    if (transport_ != nullptr)
        throw ::jxx::lang::IllegalStateException(
            "SSL socket is already connected");

    const auto inetEndpoint =
        ::jxx::CAST<::jxx::net::InetSocketAddress>(endpoint);
    if (inetEndpoint == nullptr)
        throw ::jxx::lang::IllegalArgumentException(
            "unsupported socket address");

    if (pendingTransport_ == nullptr)
        pendingTransport_ = ::jxx::NEW<::jxx::net::Socket>();

    pendingTransport_->connect(endpoint, timeout);
    transport_ = pendingTransport_;
    pendingTransport_ = nullptr;

    port_ = inetEndpoint->getPort();
    host_ = inetEndpoint->getHostString();
    if (host_ == nullptr || host_->utf8().empty()) {
        const auto address = inetEndpoint->getAddress();
        if (address != nullptr)
            host_ = address->getHostAddress();
    }
}

} // namespace jxx::ext::net::ssl::internal
