#include "ext/net/ssl/jxx.ext.net.ssl.SSLEngine.h"

#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::ext::net::ssl {

SSLEngine::SSLEngine() = default;

SSLEngine::SSLEngine(
    const ::jxx::Ptr<::jxx::lang::String>& peerHost,
    ::jxx::lang::jint peerPort)
    : peerHost_(peerHost)
    , peerPort_(peerPort) {
}

::jxx::Ptr<SSLParameters> SSLEngine::getSSLParameters() const {
    const auto parameters = ::jxx::NEW<SSLParameters>(
        getEnabledCipherSuites(), getEnabledProtocols());
    parameters->setNeedClientAuth(getNeedClientAuth());
    parameters->setWantClientAuth(getWantClientAuth());
    return parameters;
}

void SSLEngine::setSSLParameters(
    const ::jxx::Ptr<SSLParameters>& parameters) {
    if (parameters == nullptr)
        throw ::jxx::lang::NullPointerException();
    const auto ciphers = parameters->getCipherSuites();
    if (ciphers != nullptr) setEnabledCipherSuites(ciphers);
    const auto protocols = parameters->getProtocols();
    if (protocols != nullptr) setEnabledProtocols(protocols);
    if (parameters->getNeedClientAuth()) setNeedClientAuth(true);
    else if (parameters->getWantClientAuth()) setWantClientAuth(true);
    else {
        setNeedClientAuth(false);
        setWantClientAuth(false);
    }
}

::jxx::Ptr<::jxx::lang::String> SSLEngine::getPeerHost() const {
    return peerHost_;
}

::jxx::lang::jint SSLEngine::getPeerPort() const noexcept {
    return peerPort_;
}

} // namespace jxx::ext::net::ssl
