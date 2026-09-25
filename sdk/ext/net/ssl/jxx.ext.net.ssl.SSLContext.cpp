#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"

#include <mutex>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslContextConfig.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocketFactory.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::ext::net::ssl {
namespace {
std::mutex defaultContextMutex;
::jxx::Ptr<SSLContext> defaultContext;
}

SSLContext::SSLContext(const ::jxx::Ptr<::jxx::lang::String>& protocol)
    : protocol_(protocol)
    , config_(std::make_shared<internal::OpenSslContextConfig>(
          protocol,
          nullptr,
          nullptr,
          nullptr)) {
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
        protocol_, keyManagers, trustManagers, secureRandom);
}

::jxx::Ptr<SSLSocketFactory> SSLContext::getSocketFactory() {
    return ::jxx::NEW<internal::OpenSslSocketFactory>(config_);
}

::jxx::Ptr<::jxx::lang::String> SSLContext::getProtocol() const {
    return protocol_;
}

} // namespace jxx::ext::net::ssl
