#pragma once
#include <vector>
#include "ext/net/ssl/jxx.ext.net.ssl.X509ExtendedKeyManager.h"
namespace jxx::ext::net::ssl::internal {
class OpenSslCompositeKeyManager final : public ::jxx::lang::ClassBase<OpenSslCompositeKeyManager,::jxx::ext::net::ssl::X509ExtendedKeyManager>{
public:
 using Manager=::jxx::Ptr<::jxx::ext::net::ssl::X509ExtendedKeyManager>;
 explicit OpenSslCompositeKeyManager(const std::vector<Manager>& managers);
 ::jxx::Ptr<::jxx::lang::String> chooseClientAlias(const ::jxx::Ptr<StringArray>&,const ::jxx::Ptr<PrincipalArray>&,const ::jxx::Ptr<::jxx::net::Socket>&) override;
 ::jxx::Ptr<::jxx::lang::String> chooseServerAlias(const ::jxx::Ptr<::jxx::lang::String>&,const ::jxx::Ptr<PrincipalArray>&,const ::jxx::Ptr<::jxx::net::Socket>&) override;
 ::jxx::Ptr<CertificateArray> getCertificateChain(const ::jxx::Ptr<::jxx::lang::String>&) override;
 ::jxx::Ptr<StringArray> getClientAliases(const ::jxx::Ptr<::jxx::lang::String>&,const ::jxx::Ptr<PrincipalArray>&) override;
 ::jxx::Ptr<::jxx::security::PrivateKey> getPrivateKey(const ::jxx::Ptr<::jxx::lang::String>&) override;
 ::jxx::Ptr<StringArray> getServerAliases(const ::jxx::Ptr<::jxx::lang::String>&,const ::jxx::Ptr<PrincipalArray>&) override;
 ::jxx::Ptr<::jxx::lang::String> chooseEngineClientAlias(const ::jxx::Ptr<StringArray>&,const ::jxx::Ptr<PrincipalArray>&,const ::jxx::Ptr<::jxx::ext::net::ssl::SSLEngine>&) override;
 ::jxx::Ptr<::jxx::lang::String> chooseEngineServerAlias(const ::jxx::Ptr<::jxx::lang::String>&,const ::jxx::Ptr<PrincipalArray>&,const ::jxx::Ptr<::jxx::ext::net::ssl::SSLEngine>&) override;
private: std::vector<Manager> managers_;
};}
