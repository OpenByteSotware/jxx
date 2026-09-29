#include <mutex>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.DefaultHostnameVerifier.h"
#include "ext/net/ssl/jxx.ext.net.ssl.HttpsURLConnection.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "security/cert/jxx.security.cert.X509Certificate.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLPeerUnverifiedException.h"
namespace jxx::ext::net::ssl { namespace {
std::mutex& defaultsMutex(){static std::mutex m;return m;}
::jxx::Ptr<HostnameVerifier>& defaultVerifier(){static ::jxx::Ptr<HostnameVerifier> v=::jxx::NEW<::jxx::ext::net::ssl::internal::DefaultHostnameVerifier>();return v;}
::jxx::Ptr<SSLSocketFactory>& defaultFactory(){
    static ::jxx::Ptr<SSLSocketFactory> factory =
        ::jxx::CAST<SSLSocketFactory>(SSLSocketFactory::getDefault());
    return factory;
}
}
HttpsURLConnection::HttpsURLConnection(const ::jxx::Ptr<::jxx::net::URL>& url):Super(url){std::lock_guard<std::mutex> lock(defaultsMutex());hostnameVerifier_=defaultVerifier();sslSocketFactory_=defaultFactory();}
void HttpsURLConnection::setDefaultHostnameVerifier(const ::jxx::Ptr<HostnameVerifier>& verifier){if(verifier==nullptr)throw ::jxx::lang::IllegalArgumentException();std::lock_guard<std::mutex> lock(defaultsMutex());defaultVerifier()=verifier;}
::jxx::Ptr<HostnameVerifier> HttpsURLConnection::getDefaultHostnameVerifier(){std::lock_guard<std::mutex> lock(defaultsMutex());return defaultVerifier();}
void HttpsURLConnection::setHostnameVerifier(const ::jxx::Ptr<HostnameVerifier>& verifier){if(verifier==nullptr)throw ::jxx::lang::IllegalArgumentException();hostnameVerifier_=verifier;}
::jxx::Ptr<HostnameVerifier> HttpsURLConnection::getHostnameVerifier()const{return hostnameVerifier_;}
void HttpsURLConnection::setDefaultSSLSocketFactory(const ::jxx::Ptr<SSLSocketFactory>& factory){if(factory==nullptr)throw ::jxx::lang::IllegalArgumentException();std::lock_guard<std::mutex> lock(defaultsMutex());defaultFactory()=factory;}
::jxx::Ptr<SSLSocketFactory> HttpsURLConnection::getDefaultSSLSocketFactory(){std::lock_guard<std::mutex> lock(defaultsMutex());return defaultFactory();}
void HttpsURLConnection::setSSLSocketFactory(const ::jxx::Ptr<SSLSocketFactory>& factory){if(factory==nullptr)throw ::jxx::lang::IllegalArgumentException();sslSocketFactory_=factory;}
::jxx::Ptr<SSLSocketFactory> HttpsURLConnection::getSSLSocketFactory()const{return sslSocketFactory_;}
::jxx::Ptr<::jxx::security::Principal>
HttpsURLConnection::getPeerPrincipal() const {
    const auto certificates = getServerCertificates();
    if (certificates == nullptr || certificates->length == 0 ||
        (*certificates)[0] == nullptr)
        throw ::jxx::ext::net::ssl::SSLPeerUnverifiedException(
            "peer not authenticated");
    const auto certificate = ::jxx::CAST<
        ::jxx::security::cert::X509Certificate>((*certificates)[0]);
    if (certificate == nullptr)
        throw ::jxx::ext::net::ssl::SSLPeerUnverifiedException(
            "peer certificate is not X.509");
    return certificate->getSubjectDN();
}

::jxx::Ptr<::jxx::security::Principal>
HttpsURLConnection::getLocalPrincipal() const {
    const auto certificates = getLocalCertificates();
    if (certificates == nullptr || certificates->length == 0 ||
        (*certificates)[0] == nullptr)
        return nullptr;
    const auto certificate = ::jxx::CAST<
        ::jxx::security::cert::X509Certificate>((*certificates)[0]);
    return certificate == nullptr ? nullptr : certificate->getSubjectDN();
}
}
