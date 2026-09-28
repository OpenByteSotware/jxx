#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslDefaultKeyManager.h"
namespace jxx::ext::net::ssl::internal {
::jxx::Ptr<::jxx::lang::String> OpenSslDefaultKeyManager::chooseClientAlias(const ::jxx::Ptr<StringArray>&,const ::jxx::Ptr<PrincipalArray>&,const ::jxx::Ptr<::jxx::net::Socket>&){return nullptr;}
::jxx::Ptr<::jxx::lang::String> OpenSslDefaultKeyManager::chooseServerAlias(const ::jxx::Ptr<::jxx::lang::String>&,const ::jxx::Ptr<PrincipalArray>&,const ::jxx::Ptr<::jxx::net::Socket>&){return nullptr;}
::jxx::Ptr<OpenSslDefaultKeyManager::CertificateArray> OpenSslDefaultKeyManager::getCertificateChain(const ::jxx::Ptr<::jxx::lang::String>&){return nullptr;}
::jxx::Ptr<OpenSslDefaultKeyManager::StringArray> OpenSslDefaultKeyManager::getClientAliases(const ::jxx::Ptr<::jxx::lang::String>&,const ::jxx::Ptr<PrincipalArray>&){return nullptr;}
::jxx::Ptr<::jxx::security::PrivateKey> OpenSslDefaultKeyManager::getPrivateKey(const ::jxx::Ptr<::jxx::lang::String>&){return nullptr;}
::jxx::Ptr<OpenSslDefaultKeyManager::StringArray> OpenSslDefaultKeyManager::getServerAliases(const ::jxx::Ptr<::jxx::lang::String>&,const ::jxx::Ptr<PrincipalArray>&){return nullptr;}
::jxx::Ptr<::jxx::lang::String> OpenSslDefaultKeyManager::chooseEngineClientAlias(const ::jxx::Ptr<StringArray>&,const ::jxx::Ptr<PrincipalArray>&,const ::jxx::Ptr<::jxx::ext::net::ssl::SSLEngine>&){return nullptr;}
::jxx::Ptr<::jxx::lang::String> OpenSslDefaultKeyManager::chooseEngineServerAlias(const ::jxx::Ptr<::jxx::lang::String>&,const ::jxx::Ptr<PrincipalArray>&,const ::jxx::Ptr<::jxx::ext::net::ssl::SSLEngine>&){return nullptr;}
} // namespace jxx::ext::net::ssl::internal
