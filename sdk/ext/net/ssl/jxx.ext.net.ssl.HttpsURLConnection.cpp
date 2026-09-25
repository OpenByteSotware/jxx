#include "ext/net/ssl/jxx.ext.net.ssl.HttpsURLConnection.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include <mutex>
namespace jxx::ext::net::ssl { namespace {
std::mutex& defaultsMutex(){static std::mutex m;return m;}
::jxx::Ptr<HostnameVerifier>& defaultVerifier(){static ::jxx::Ptr<HostnameVerifier> v;return v;}
::jxx::Ptr<SSLSocketFactory>& defaultFactory(){static ::jxx::Ptr<SSLSocketFactory> f;return f;}
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
::jxx::Ptr<::jxx::security::Principal> HttpsURLConnection::getPeerPrincipal()const{throw ::jxx::lang::UnsupportedOperationException();}
::jxx::Ptr<::jxx::security::Principal> HttpsURLConnection::getLocalPrincipal()const{return nullptr;}
}
