#pragma once

#include <memory>

#include "ext/net/ssl/jxx.ext.net.ssl.SSLServerSocketFactory.h"

namespace jxx::ext::net::ssl::internal {

class OpenSslContextConfig;

class OpenSslServerSocketFactory final
    : public ::jxx::lang::ClassBase<
          OpenSslServerSocketFactory,
          ::jxx::ext::net::ssl::SSLServerSocketFactory> {
public:
    using JxxSuper = ::jxx::ext::net::ssl::SSLServerSocketFactory;
    using Super = ::jxx::lang::ClassBase<
        OpenSslServerSocketFactory,
        JxxSuper>;
    using StringArray = JxxSuper::StringArray;

    explicit OpenSslServerSocketFactory(
        const std::shared_ptr<OpenSslContextConfig>& config);

    ::jxx::Ptr<StringArray>
    getDefaultCipherSuites() const override;

    ::jxx::Ptr<StringArray>
    getSupportedCipherSuites() const override;

    ::jxx::Ptr<::jxx::net::ServerSocket>
    createServerSocket() override;

    ::jxx::Ptr<::jxx::net::ServerSocket>
    createServerSocket(::jxx::lang::jint port) override;

    ::jxx::Ptr<::jxx::net::ServerSocket>
    createServerSocket(
        ::jxx::lang::jint port,
        ::jxx::lang::jint backlog) override;

    ::jxx::Ptr<::jxx::net::ServerSocket>
    createServerSocket(
        ::jxx::lang::jint port,
        ::jxx::lang::jint backlog,
        const ::jxx::Ptr<::jxx::net::InetAddress>& address) override;

private:
    std::shared_ptr<OpenSslContextConfig> config_;
};

} // namespace jxx::ext::net::ssl::internal
