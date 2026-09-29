#include <openssl/ssl.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslServerSocketFactory.h"

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslServerSocket.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslCipherSuites.h"
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::ext::net::ssl::internal {
namespace {
void validate(::jxx::lang::jint port, ::jxx::lang::jint backlog) {
    if (port < 0 || port > 65535 || backlog < 1)
        throw ::jxx::lang::IllegalArgumentException();
}
} // namespace
OpenSslServerSocketFactory::OpenSslServerSocketFactory(
    const std::shared_ptr<OpenSslContextConfig>& config) : config_(config) {}
::jxx::Ptr<OpenSslServerSocketFactory::StringArray>
OpenSslServerSocketFactory::getDefaultCipherSuites() const {
    return serverDefaultCipherSuites();
}
::jxx::Ptr<OpenSslServerSocketFactory::StringArray>
OpenSslServerSocketFactory::getSupportedCipherSuites() const {
    return serverSupportedCipherSuites();
}
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
