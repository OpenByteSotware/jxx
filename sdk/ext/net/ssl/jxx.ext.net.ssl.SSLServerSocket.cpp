#include "ext/net/ssl/jxx.ext.net.ssl.SSLServerSocket.h"

#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::ext::net::ssl {

SSLServerSocket::SSLServerSocket() = default;

SSLServerSocket::SSLServerSocket(::jxx::lang::jint port)
    : Super(port) {
}

SSLServerSocket::SSLServerSocket(
    ::jxx::lang::jint port,
    ::jxx::lang::jint backlog)
    : Super(port, backlog) {
}

SSLServerSocket::SSLServerSocket(
    ::jxx::lang::jint port,
    ::jxx::lang::jint backlog,
    const ::jxx::Ptr<::jxx::net::InetAddress>& address)
    : Super(port, backlog, address) {
}

::jxx::Ptr<SSLParameters>
SSLServerSocket::getSSLParameters() const {
    const auto parameters = ::jxx::NEW<SSLParameters>();
    parameters->setCipherSuites(getEnabledCipherSuites());
    parameters->setProtocols(getEnabledProtocols());
    if (getNeedClientAuth())
        parameters->setNeedClientAuth(true);
    else if (getWantClientAuth())
        parameters->setWantClientAuth(true);
    return parameters;
}

void SSLServerSocket::setSSLParameters(
    const ::jxx::Ptr<SSLParameters>& parameters) {
    if (parameters == nullptr)
        throw ::jxx::lang::IllegalArgumentException();
    const auto suites = parameters->getCipherSuites();
    if (suites != nullptr) setEnabledCipherSuites(suites);
    const auto protocols = parameters->getProtocols();
    if (protocols != nullptr) setEnabledProtocols(protocols);
    if (parameters->getNeedClientAuth())
        setNeedClientAuth(true);
    else if (parameters->getWantClientAuth())
        setWantClientAuth(true);
    else {
        setNeedClientAuth(false);
        setWantClientAuth(false);
    }
}

} // namespace jxx::ext::net::ssl
