#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocket.h"

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

} // namespace jxx::ext::net::ssl::internal
