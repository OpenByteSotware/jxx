#pragma once
#include "ext/net/ssl/jxx.ext.net.ssl.TrustManager.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::security::cert { class X509Certificate; }
namespace jxx::ext::net::ssl {
class X509TrustManager : public ::jxx::lang::InterfaceBase<X509TrustManager,TrustManager> {
public:
    using CertificateArray = ::jxx::lang::JxxArray<::jxx::Ptr<::jxx::security::cert::X509Certificate>,1U>;
    ~X509TrustManager() override = default;
    virtual void checkClientTrusted(const ::jxx::Ptr<CertificateArray>& chain,const ::jxx::Ptr<::jxx::lang::String>& authType)=0;
    virtual void checkServerTrusted(const ::jxx::Ptr<CertificateArray>& chain,const ::jxx::Ptr<::jxx::lang::String>& authType)=0;
    virtual ::jxx::Ptr<CertificateArray> getAcceptedIssuers()=0;
};
}
