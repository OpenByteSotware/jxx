#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"

#include <mutex>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslContextConfig.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocketFactory.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSessionContext.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "security/jxx.security.Provider.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"

namespace jxx::ext::net::ssl {
namespace {
std::mutex defaultContextMutex;
::jxx::Ptr<SSLContext> defaultContext;
}

SSLContext::SSLContext(const ::jxx::Ptr<::jxx::lang::String>& protocol)
    : protocol_(protocol)
    , provider_(::jxx::NEW<::jxx::security::Provider>(::jxx::NEW<::jxx::lang::String>("OpenSSL")))
    , config_(std::make_shared<internal::OpenSslContextConfig>(
          protocol,
          nullptr,
          nullptr,
          nullptr,
          clientSessionContext_,
          serverSessionContext_))
    , clientSessionContext_(::jxx::NEW<internal::OpenSslSessionContext>())
    , serverSessionContext_(::jxx::NEW<internal::OpenSslSessionContext>()) {
    if (protocol_ == nullptr) throw ::jxx::lang::NullPointerException();
}

SSLContext::~SSLContext() = default;

::jxx::Ptr<SSLContext> SSLContext::getInstance(
    const ::jxx::Ptr<::jxx::lang::String>& protocol) {
    if (protocol == nullptr) throw ::jxx::lang::NullPointerException();
    const auto value = protocol->utf8();
    if (value != "TLS" && value != "TLSv1.2" && value != "TLSv1.3")
        throw ::jxx::lang::IllegalArgumentException();
    return ::jxx::NEW<SSLContext>(protocol);
}

::jxx::Ptr<SSLContext> SSLContext::getDefault() {
    std::lock_guard<std::mutex> lock(defaultContextMutex);
    if (defaultContext == nullptr)
        defaultContext = getInstance(::jxx::NEW<::jxx::lang::String>("TLS"));
    return defaultContext;
}

void SSLContext::setDefault(const ::jxx::Ptr<SSLContext>& context) {
    if (context == nullptr) throw ::jxx::lang::NullPointerException();
    std::lock_guard<std::mutex> lock(defaultContextMutex);
    defaultContext = context;
}

void SSLContext::init(
    const ::jxx::Ptr<KeyManagerArray>& keyManagers,
    const ::jxx::Ptr<TrustManagerArray>& trustManagers,
    const ::jxx::Ptr<::jxx::security::SecureRandom>& secureRandom) {
    config_ = std::make_shared<internal::OpenSslContextConfig>(
        protocol_,
        keyManagers,
        trustManagers,
        secureRandom,
        clientSessionContext_,
        serverSessionContext_);
}

::jxx::Ptr<SSLSocketFactory> SSLContext::getSocketFactory() {
    return ::jxx::NEW<internal::OpenSslSocketFactory>(config_);
}

::jxx::Ptr<::jxx::lang::String> SSLContext::getProtocol() const {
    return protocol_;
}


::jxx::Ptr<SSLContext> SSLContext::getInstance(const ::jxx::Ptr<::jxx::lang::String>& protocol,const ::jxx::Ptr<::jxx::lang::String>& provider){if(provider==nullptr)throw ::jxx::lang::IllegalArgumentException();if(provider->utf8()!="OpenSSL")throw ::jxx::lang::IllegalArgumentException();return getInstance(protocol);}
::jxx::Ptr<SSLContext> SSLContext::getInstance(const ::jxx::Ptr<::jxx::lang::String>& protocol,const ::jxx::Ptr<::jxx::security::Provider>& provider){if(provider==nullptr)throw ::jxx::lang::IllegalArgumentException();return getInstance(protocol,provider->getName());}
::jxx::Ptr<::jxx::security::Provider> SSLContext::getProvider()const{return provider_;}
::jxx::Ptr<SSLServerSocketFactory> SSLContext::getServerSocketFactory(){throw ::jxx::lang::UnsupportedOperationException();}
::jxx::Ptr<SSLSessionContext> SSLContext::getClientSessionContext(){return clientSessionContext_;}
::jxx::Ptr<SSLSessionContext> SSLContext::getServerSessionContext(){return serverSessionContext_;}
::jxx::Ptr<SSLEngine> SSLContext::createSSLEngine(){throw ::jxx::lang::UnsupportedOperationException();}
::jxx::Ptr<SSLEngine> SSLContext::createSSLEngine(const ::jxx::Ptr<::jxx::lang::String>&,::jxx::lang::jint){throw ::jxx::lang::UnsupportedOperationException();}
::jxx::Ptr<SSLParameters> SSLContext::getDefaultSSLParameters(){auto p=::jxx::NEW<SSLParameters>();auto f=getSocketFactory();p->setCipherSuites(f->getDefaultCipherSuites());return p;}
::jxx::Ptr<SSLParameters> SSLContext::getSupportedSSLParameters(){auto p=::jxx::NEW<SSLParameters>();auto f=getSocketFactory();p->setCipherSuites(f->getSupportedCipherSuites());return p;}
} // namespace jxx::ext::net::ssl
