#pragma once
#include "com/sun/net/httpserver/spi/jxx.com.sun.net.httpserver.spi.HttpServerProvider.h"
namespace jxx::com::sun::net::httpserver::internal {
class DefaultHttpServerProvider final : public ::jxx::lang::ClassBase<DefaultHttpServerProvider, ::jxx::com::sun::net::httpserver::spi::HttpServerProvider> {
public:
    using JxxSuper = ::jxx::com::sun::net::httpserver::spi::HttpServerProvider;
    using Super = ::jxx::lang::ClassBase<DefaultHttpServerProvider, JxxSuper>;
    DefaultHttpServerProvider() = default;
    ~DefaultHttpServerProvider() override = default;
    ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpServer> createHttpServer(const ::jxx::Ptr<::jxx::net::InetSocketAddress>& address, ::jxx::lang::jint backlog) override;
    ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsServer> createHttpsServer(const ::jxx::Ptr<::jxx::net::InetSocketAddress>& address, ::jxx::lang::jint backlog) override;
};
}
