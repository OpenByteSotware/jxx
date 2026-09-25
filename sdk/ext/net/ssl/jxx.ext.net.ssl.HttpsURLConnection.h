#pragma once
#include "ext/net/ssl/jxx.ext.net.ssl.HostnameVerifier.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocketFactory.h"
#include "lang/jxx.lang.buildin_array.h"
#include "net/jxx.net.HttpURLConnection.h"
#include "security/cert/jxx.security.cert.Certificate.h"
#include "security/jxx.security.Principal.h"
namespace jxx::ext::net::ssl {
class HttpsURLConnection : public ::jxx::lang::ClassBase<HttpsURLConnection,::jxx::net::HttpURLConnection> {
public:
    using JxxSuper=::jxx::net::HttpURLConnection;
    using Super=::jxx::lang::ClassBase<HttpsURLConnection,JxxSuper>;
    using CertificateArray=::jxx::lang::JxxArray<::jxx::Ptr<::jxx::security::cert::Certificate>,1U>;
    ~HttpsURLConnection() override=default;
    static void setDefaultHostnameVerifier(const ::jxx::Ptr<HostnameVerifier>& verifier);
    static ::jxx::Ptr<HostnameVerifier> getDefaultHostnameVerifier();
    void setHostnameVerifier(const ::jxx::Ptr<HostnameVerifier>& verifier);
    ::jxx::Ptr<HostnameVerifier> getHostnameVerifier() const;
    static void setDefaultSSLSocketFactory(const ::jxx::Ptr<SSLSocketFactory>& factory);
    static ::jxx::Ptr<SSLSocketFactory> getDefaultSSLSocketFactory();
    void setSSLSocketFactory(const ::jxx::Ptr<SSLSocketFactory>& factory);
    ::jxx::Ptr<SSLSocketFactory> getSSLSocketFactory() const;
    virtual ::jxx::Ptr<::jxx::lang::String> getCipherSuite() const=0;
    virtual ::jxx::Ptr<CertificateArray> getLocalCertificates() const=0;
    virtual ::jxx::Ptr<CertificateArray> getServerCertificates() const=0;
    virtual ::jxx::Ptr<::jxx::security::Principal> getPeerPrincipal() const;
    virtual ::jxx::Ptr<::jxx::security::Principal> getLocalPrincipal() const;
protected:
    explicit HttpsURLConnection(const ::jxx::Ptr<::jxx::net::URL>& url);
    ::jxx::Ptr<HostnameVerifier> hostnameVerifier_;
private:
    ::jxx::Ptr<SSLSocketFactory> sslSocketFactory_;
};
}
