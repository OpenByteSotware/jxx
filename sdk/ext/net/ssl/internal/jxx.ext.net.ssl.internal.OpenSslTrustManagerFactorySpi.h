#pragma once
#include "ext/net/ssl/jxx.ext.net.ssl.TrustManagerFactorySpi.h"
namespace jxx::ext::net::ssl::internal {
class OpenSslTrustManagerFactorySpi final : public ::jxx::lang::ClassBase<OpenSslTrustManagerFactorySpi,::jxx::ext::net::ssl::TrustManagerFactorySpi>{public:OpenSslTrustManagerFactorySpi()=default;::jxx::Ptr<TrustManagerArray>engineGetTrustManagers()override;void engineInit(const ::jxx::Ptr<::jxx::security::KeyStore>&)override;void engineInit(const ::jxx::Ptr<::jxx::ext::net::ssl::ManagerFactoryParameters>&)override;private: ::jxx::lang::jbool initialized_=false;};}
