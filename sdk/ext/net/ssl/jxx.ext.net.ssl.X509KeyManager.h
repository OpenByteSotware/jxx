#pragma once
#include "ext/net/ssl/jxx.ext.net.ssl.KeyManager.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::net { class Socket; }
namespace jxx::security { class Principal; class PrivateKey; }
namespace jxx::security::cert { class X509Certificate; }
namespace jxx::ext::net::ssl {
class X509KeyManager : public ::jxx::lang::InterfaceBase<X509KeyManager,KeyManager> {
public:
    using StringArray = ::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>,1U>;
    using PrincipalArray = ::jxx::lang::JxxArray<::jxx::Ptr<::jxx::security::Principal>,1U>;
    using CertificateArray = ::jxx::lang::JxxArray<::jxx::Ptr<::jxx::security::cert::X509Certificate>,1U>;
    ~X509KeyManager() override = default;
    virtual ::jxx::Ptr<::jxx::lang::String> chooseClientAlias(const ::jxx::Ptr<StringArray>& keyTypes,const ::jxx::Ptr<PrincipalArray>& issuers,const ::jxx::Ptr<::jxx::net::Socket>& socket)=0;
    virtual ::jxx::Ptr<::jxx::lang::String> chooseServerAlias(const ::jxx::Ptr<::jxx::lang::String>& keyType,const ::jxx::Ptr<PrincipalArray>& issuers,const ::jxx::Ptr<::jxx::net::Socket>& socket)=0;
    virtual ::jxx::Ptr<CertificateArray> getCertificateChain(const ::jxx::Ptr<::jxx::lang::String>& alias)=0;
    virtual ::jxx::Ptr<StringArray> getClientAliases(const ::jxx::Ptr<::jxx::lang::String>& keyType,const ::jxx::Ptr<PrincipalArray>& issuers)=0;
    virtual ::jxx::Ptr<::jxx::security::PrivateKey> getPrivateKey(const ::jxx::Ptr<::jxx::lang::String>& alias)=0;
    virtual ::jxx::Ptr<StringArray> getServerAliases(const ::jxx::Ptr<::jxx::lang::String>& keyType,const ::jxx::Ptr<PrincipalArray>& issuers)=0;
};
}
