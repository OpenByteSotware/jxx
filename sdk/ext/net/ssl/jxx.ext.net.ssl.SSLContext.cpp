#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"

#include <mutex>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslContextSpi.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContextSpi.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "security/jxx.security.NoSuchAlgorithmException.h"
#include "security/jxx.security.NoSuchProviderException.h"
#include "security/jxx.security.Provider.h"

namespace jxx::ext::net::ssl {
namespace {

std::mutex defaultContextMutex;
::jxx::Ptr<SSLContext> defaultContext;

void validateProtocol(
    const ::jxx::Ptr<::jxx::lang::String>& protocol)
{
    if (protocol == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    const auto value = protocol->utf8();
    if (value.empty()) {
        throw ::jxx::security::NoSuchAlgorithmException(
            "SSLContext protocol is empty");
    }

    if (value != "TLS" &&
        value != "TLSv1" &&
        value != "TLSv1.2" &&
        value != "TLSv1.3")
    {
        throw ::jxx::security::NoSuchAlgorithmException(
            "OpenSSL provider does not implement the requested SSLContext protocol");
    }
}

void validateProviderName(
    const ::jxx::Ptr<::jxx::lang::String>& provider)
{
    if (provider == nullptr || provider->utf8().empty()) {
        throw ::jxx::lang::IllegalArgumentException();
    }

    if (provider->utf8() != "OpenSSL") {
        throw ::jxx::security::NoSuchProviderException(
            "SSLContext provider is not installed");
    }
}

void validateProvider(
    const ::jxx::Ptr<::jxx::security::Provider>& provider,
    const ::jxx::Ptr<::jxx::lang::String>& protocol)
{
    if (provider == nullptr) {
        throw ::jxx::lang::IllegalArgumentException();
    }

    const auto name = provider->getName();
    if (name == nullptr || name->utf8() != "OpenSSL") {
        throw ::jxx::security::NoSuchAlgorithmException(
            "Provider does not implement the requested SSLContext protocol");
    }

    validateProtocol(protocol);
}

::jxx::Ptr<::jxx::security::Provider> openSslProvider() {
    return ::jxx::NEW<::jxx::security::Provider>(
        ::jxx::NEW<::jxx::lang::String>("OpenSSL"));
}

} // namespace

SSLContext::SSLContext(
    const ::jxx::Ptr<SSLContextSpi>& contextSpi,
    const ::jxx::Ptr<::jxx::security::Provider>& provider,
    const ::jxx::Ptr<::jxx::lang::String>& protocol)
    : contextSpi_(contextSpi)
    , provider_(provider)
    , protocol_(protocol) {
    if (contextSpi_ == nullptr || provider_ == nullptr ||
        protocol_ == nullptr)
    {
        throw ::jxx::lang::NullPointerException();
    }
}

::jxx::Ptr<SSLContext> SSLContext::getInstance(
    const ::jxx::Ptr<::jxx::lang::String>& protocol) {
    validateProtocol(protocol);
    const auto provider = openSslProvider();
    return ::jxx::NEW<SSLContext>(
        ::jxx::NEW<internal::OpenSslContextSpi>(protocol),
        provider,
        protocol);
}

::jxx::Ptr<SSLContext> SSLContext::getInstance(
    const ::jxx::Ptr<::jxx::lang::String>& protocol,
    const ::jxx::Ptr<::jxx::lang::String>& provider) {
    validateProtocol(protocol);
    validateProviderName(provider);
    return getInstance(protocol);
}

::jxx::Ptr<SSLContext> SSLContext::getInstance(
    const ::jxx::Ptr<::jxx::lang::String>& protocol,
    const ::jxx::Ptr<::jxx::security::Provider>& provider) {
    validateProvider(provider, protocol);
    return ::jxx::NEW<SSLContext>(
        ::jxx::NEW<internal::OpenSslContextSpi>(protocol),
        provider,
        protocol);
}

::jxx::Ptr<SSLContext> SSLContext::getDefault() {
    std::lock_guard<std::mutex> lock(defaultContextMutex);
    if (defaultContext == nullptr) {
        defaultContext = getInstance(
            ::jxx::NEW<::jxx::lang::String>("TLS"));
        defaultContext->init(nullptr, nullptr, nullptr);
    }
    return defaultContext;
}

void SSLContext::setDefault(
    const ::jxx::Ptr<SSLContext>& context) {
    if (context == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    std::lock_guard<std::mutex> lock(defaultContextMutex);
    defaultContext = context;
}

void SSLContext::init(
    const ::jxx::Ptr<KeyManagerArray>& keyManagers,
    const ::jxx::Ptr<TrustManagerArray>& trustManagers,
    const ::jxx::Ptr<::jxx::security::SecureRandom>& secureRandom) {
    contextSpi_->engineInit(
        keyManagers, trustManagers, secureRandom);
}

::jxx::Ptr<SSLSocketFactory>
SSLContext::getSocketFactory() {
    return contextSpi_->engineGetSocketFactory();
}
::jxx::Ptr<SSLServerSocketFactory>
SSLContext::getServerSocketFactory() {
    return contextSpi_->engineGetServerSocketFactory();
}
::jxx::Ptr<SSLSessionContext>
SSLContext::getClientSessionContext() {
    return contextSpi_->engineGetClientSessionContext();
}
::jxx::Ptr<SSLSessionContext>
SSLContext::getServerSessionContext() {
    return contextSpi_->engineGetServerSessionContext();
}
::jxx::Ptr<SSLEngine>
SSLContext::createSSLEngine() {
    return contextSpi_->engineCreateSSLEngine();
}
::jxx::Ptr<SSLEngine> SSLContext::createSSLEngine(
    const ::jxx::Ptr<::jxx::lang::String>& peerHost,
    ::jxx::lang::jint peerPort) {
    return contextSpi_->engineCreateSSLEngine(peerHost, peerPort);
}
::jxx::Ptr<SSLParameters>
SSLContext::getDefaultSSLParameters() {
    return contextSpi_->engineGetDefaultSSLParameters();
}
::jxx::Ptr<SSLParameters>
SSLContext::getSupportedSSLParameters() {
    return contextSpi_->engineGetSupportedSSLParameters();
}
::jxx::Ptr<::jxx::lang::String>
SSLContext::getProtocol() const {
    return protocol_;
}
::jxx::Ptr<::jxx::security::Provider>
SSLContext::getProvider() const {
    return provider_;
}

} // namespace jxx::ext::net::ssl
