#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocket.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::ext::net::ssl {

SSLSocket::SSLSocket() = default;

::jxx::Ptr<SSLParameters> SSLSocket::getSSLParameters() const {
    const auto parameters = ::jxx::NEW<SSLParameters>(
        getEnabledCipherSuites(), getEnabledProtocols());
    parameters->setNeedClientAuth(getNeedClientAuth());
    parameters->setWantClientAuth(getWantClientAuth());
    return parameters;
}

void SSLSocket::setSSLParameters(
    const ::jxx::Ptr<SSLParameters>& parameters) {
    if (parameters == nullptr) throw ::jxx::lang::NullPointerException();
    const auto ciphers = parameters->getCipherSuites();
    if (ciphers != nullptr) setEnabledCipherSuites(ciphers);
    const auto protocols = parameters->getProtocols();
    if (protocols != nullptr) setEnabledProtocols(protocols);
    if (parameters->getNeedClientAuth()) setNeedClientAuth(true);
    else if (parameters->getWantClientAuth()) setWantClientAuth(true);
    else { setNeedClientAuth(false); setWantClientAuth(false); }
}

} // namespace jxx::ext::net::ssl
