#pragma once
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpsParameters.h"
namespace jxx::ext::net::ssl { class SSLParameters; }
namespace jxx::com::sun::net::httpserver::internal {
class DefaultHttpsParameters final : public ::jxx::lang::ClassBase<DefaultHttpsParameters, ::jxx::com::sun::net::httpserver::HttpsParameters> {
public:
    using JxxSuper = ::jxx::com::sun::net::httpserver::HttpsParameters;
    using Super = ::jxx::lang::ClassBase<DefaultHttpsParameters, JxxSuper>;
    DefaultHttpsParameters(const ::jxx::Ptr<::jxx::net::InetSocketAddress>& clientAddress, const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsConfigurator>& configurator);
    ::jxx::Ptr<::jxx::net::InetSocketAddress> getClientAddress() override;
    ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsConfigurator> getHttpsConfigurator() override;
    void setSSLParameters(const ::jxx::Ptr<::jxx::ext::net::ssl::SSLParameters>& parameters) override;
    ::jxx::Ptr<::jxx::ext::net::ssl::SSLParameters> getAppliedSSLParameters() const;
private:
    ::jxx::Ptr<::jxx::net::InetSocketAddress> clientAddress_;
    ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsConfigurator> configurator_;
    ::jxx::Ptr<::jxx::ext::net::ssl::SSLParameters> parameters_;
}; }
