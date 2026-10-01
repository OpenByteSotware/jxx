#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocket.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::ext::net::ssl {

SSLSocket::SSLSocket() = default;

SSLSocket::SSLSocket(
    const ::jxx::Ptr<::jxx::lang::String>& host,
    ::jxx::lang::jint port)
    : Super(host, port) {
}

SSLSocket::SSLSocket(
    const ::jxx::Ptr<::jxx::net::InetAddress>& address,
    ::jxx::lang::jint port)
    : Super(address, port) {
}

SSLSocket::SSLSocket(
    const ::jxx::Ptr<::jxx::lang::String>& host,
    ::jxx::lang::jint port,
    const ::jxx::Ptr<::jxx::net::InetAddress>& clientAddress,
    ::jxx::lang::jint clientPort)
    : Super(host, port, clientAddress, clientPort) {
}

SSLSocket::SSLSocket(
    const ::jxx::Ptr<::jxx::net::InetAddress>& address,
    ::jxx::lang::jint port,
    const ::jxx::Ptr<::jxx::net::InetAddress>& clientAddress,
    ::jxx::lang::jint clientPort)
    : Super(address, port, clientAddress, clientPort) {
}

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

::jxx::Ptr<SSLSession> SSLSocket::getHandshakeSession() const { return nullptr; }

} // namespace jxx::ext::net::ssl
