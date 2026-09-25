#include "ext/net/ssl/jxx.ext.net.ssl.SSLServerSocket.h"

namespace jxx::ext::net::ssl {

SSLServerSocket::SSLServerSocket(::jxx::lang::jint port)
    : Super(port) {
}

SSLServerSocket::SSLServerSocket(
    ::jxx::lang::jint port,
    ::jxx::lang::jint backlog)
    : Super(port, backlog) {
}

SSLServerSocket::SSLServerSocket(
    ::jxx::lang::jint port,
    ::jxx::lang::jint backlog,
    const ::jxx::Ptr<::jxx::net::InetAddress>& address)
    : Super(port, backlog, address) {
}

} // namespace jxx::ext::net::ssl
