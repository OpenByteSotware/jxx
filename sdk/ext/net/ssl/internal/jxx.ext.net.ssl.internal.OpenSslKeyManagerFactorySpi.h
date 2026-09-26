#pragma once
#include "ext/net/ssl/jxx.ext.net.ssl.KeyManagerFactorySpi.h"
namespace jxx::ext::net::ssl::internal {
class OpenSslKeyManagerFactorySpi final : public ::jxx::lang::ClassBase<OpenSslKeyManagerFactorySpi,::jxx::ext::net::ssl::KeyManagerFactorySpi>{public:OpenSslKeyManagerFactorySpi()=default;::jxx::Ptr<KeyManagerArray>engineGetKeyManagers()override;void engineInit(const ::jxx::Ptr<::jxx::security::KeyStore>&,const ::jxx::Ptr<CharArray>&)override;void engineInit(const ::jxx::Ptr<::jxx::ext::net::ssl::ManagerFactoryParameters>&)override;private: ::jxx::lang::jbool initialized_=false;};}
