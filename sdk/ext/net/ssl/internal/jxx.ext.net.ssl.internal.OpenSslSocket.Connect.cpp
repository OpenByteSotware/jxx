#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocket.h"

#include "io/jxx.io.IOException.h"

#include "lang/jxx.lang.IllegalArgumentException.h"

#include "net/jxx.net.InetAddress.h"

#include "net/jxx.net.InetSocketAddress.h"

#include "lang/jxx.lang.IllegalStateException.h"
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
    std::lock_guard<std::recursive_mutex> tlsLock(tlsMutex_);
    if (endpoint == nullptr)
        throw ::jxx::lang::IllegalArgumentException("endpoint is null");
    if (timeout < 0)
        throw ::jxx::lang::IllegalArgumentException("timeout is negative");
    if (closed_)
        throw ::jxx::io::IOException("SSL socket is closed");
    if (session_ != nullptr || handshakeInProgress_)
        throw ::jxx::lang::IllegalStateException(
            "SSL socket handshake has already started");
    if (connected_ || transport_ != nullptr)
        throw ::jxx::lang::IllegalStateException(
            "SSL socket is already connected");

    const auto internetEndpoint =
        ::jxx::CAST<::jxx::net::InetSocketAddress>(endpoint);
    if (internetEndpoint == nullptr)
        throw ::jxx::lang::IllegalArgumentException(
            "unsupported socket address");

    if (pendingTransport_ == nullptr)
        pendingTransport_ = ::jxx::NEW<::jxx::net::Socket>();
    pendingTransport_->setSoTimeout(soTimeout_);
    pendingTransport_->connect(endpoint, timeout);

    transport_ = pendingTransport_;
    pendingTransport_ = nullptr;
    host_ = internetEndpoint->getHostString();
    port_ = internetEndpoint->getPort();
    if (host_ == nullptr || host_->utf8().empty()) {
        const auto address = internetEndpoint->getAddress();
        if (address != nullptr) host_ = address->getHostAddress();
    }
    if (host_ == nullptr || host_->utf8().empty()) {
        transport_->close();
        transport_ = nullptr;
        throw ::jxx::lang::IllegalArgumentException(
            "endpoint has no usable peer host");
    }
    connected_ = true;
}

} // namespace jxx::ext::net::ssl::internal
