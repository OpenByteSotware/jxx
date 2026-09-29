#pragma once
#include "ext/net/ssl/jxx.ext.net.ssl.X509ExtendedKeyManager.h"
#include "security/jxx.security.KeyStore.h"
namespace jxx::ext::net::ssl::internal {
class OpenSslKeyStoreKeyManager final
 : public ::jxx::lang::ClassBase<OpenSslKeyStoreKeyManager,::jxx::ext::net::ssl::X509ExtendedKeyManager> {
public:
 using JxxSuper=::jxx::ext::net::ssl::X509ExtendedKeyManager;
 using Super=::jxx::lang::ClassBase<OpenSslKeyStoreKeyManager,JxxSuper>;
 explicit OpenSslKeyStoreKeyManager(const ::jxx::Ptr<::jxx::security::KeyStore>& store,const ::jxx::Ptr<::jxx::security::KeyStore::CharArray>& password);
 ::jxx::Ptr<::jxx::lang::String> chooseClientAlias(const ::jxx::Ptr<StringArray>&,const ::jxx::Ptr<PrincipalArray>&,const ::jxx::Ptr<::jxx::net::Socket>&) override;
 ::jxx::Ptr<::jxx::lang::String> chooseServerAlias(const ::jxx::Ptr<::jxx::lang::String>&,const ::jxx::Ptr<PrincipalArray>&,const ::jxx::Ptr<::jxx::net::Socket>&) override;
 ::jxx::Ptr<CertificateArray> getCertificateChain(const ::jxx::Ptr<::jxx::lang::String>&) override;
 ::jxx::Ptr<StringArray> getClientAliases(const ::jxx::Ptr<::jxx::lang::String>&,const ::jxx::Ptr<PrincipalArray>&) override;
 ::jxx::Ptr<::jxx::security::PrivateKey> getPrivateKey(const ::jxx::Ptr<::jxx::lang::String>&) override;
 ::jxx::Ptr<StringArray> getServerAliases(const ::jxx::Ptr<::jxx::lang::String>&,const ::jxx::Ptr<PrincipalArray>&) override;
 ::jxx::Ptr<::jxx::lang::String> chooseEngineClientAlias(const ::jxx::Ptr<StringArray>&,const ::jxx::Ptr<PrincipalArray>&,const ::jxx::Ptr<::jxx::ext::net::ssl::SSLEngine>&) override;
 ::jxx::Ptr<::jxx::lang::String> chooseEngineServerAlias(const ::jxx::Ptr<::jxx::lang::String>&,const ::jxx::Ptr<PrincipalArray>&,const ::jxx::Ptr<::jxx::ext::net::ssl::SSLEngine>&) override;
private:
 ::jxx::Ptr<::jxx::security::KeyStore> store_;
 ::jxx::Ptr<::jxx::security::KeyStore::CharArray> password_;
};}
