#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSession.h"

namespace jxx::ext::net::ssl::internal {

OpenSslSession::OpenSslSession(
    const ::jxx::Ptr<::jxx::lang::String>& cipher,
    const ::jxx::Ptr<::jxx::lang::String>& protocol,
    const ::jxx::Ptr<::jxx::lang::String>& host,
    ::jxx::lang::jint port,
    const ::jxx::Ptr<CertificateArray>& peerCertificates,
    const ::jxx::Ptr<CertificateArray>& localCertificates)
    : cipher_(cipher)
    , protocol_(protocol)
    , host_(host)
    , port_(port)
    , peerCertificates_(peerCertificates)
    , localCertificates_(localCertificates) {
}

::jxx::lang::ByteArray OpenSslSession::getId() const {
    return ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(0);
}

::jxx::Ptr<::jxx::lang::String> OpenSslSession::getCipherSuite() const {
    return cipher_;
}

::jxx::Ptr<::jxx::lang::String> OpenSslSession::getProtocol() const {
    return protocol_;
}

::jxx::Ptr<::jxx::lang::String> OpenSslSession::getPeerHost() const {
    return host_;
}

::jxx::lang::jint OpenSslSession::getPeerPort() const {
    return port_;
}

::jxx::Ptr<OpenSslSession::CertificateArray>
OpenSslSession::getLocalCertificates() const {
    return localCertificates_;
}

::jxx::Ptr<OpenSslSession::CertificateArray>
OpenSslSession::getPeerCertificates() const {
    return peerCertificates_;
}

::jxx::Ptr<::jxx::security::Principal>
OpenSslSession::getPeerPrincipal() const {
    return nullptr;
}

::jxx::Ptr<::jxx::security::Principal>
OpenSslSession::getLocalPrincipal() const {
    return nullptr;
}

::jxx::lang::jbool OpenSslSession::isValid() const {
    return valid_;
}

void OpenSslSession::invalidate() {
    valid_ = false;
}

} // namespace jxx::ext::net::ssl::internal
