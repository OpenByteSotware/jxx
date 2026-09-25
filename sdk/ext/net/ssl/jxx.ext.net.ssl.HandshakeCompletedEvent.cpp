#include "ext/net/ssl/jxx.ext.net.ssl.HandshakeCompletedEvent.h"

#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocket.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::ext::net::ssl {

HandshakeCompletedEvent::HandshakeCompletedEvent(
    const ::jxx::Ptr<SSLSocket>& socket,
    const ::jxx::Ptr<SSLSession>& session)
    : Super(::jxx::CAST<::jxx::lang::Object>(socket))
    , session_(session) {
    if (socket == nullptr || session_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

::jxx::Ptr<SSLSession> HandshakeCompletedEvent::getSession() const {
    return session_;
}

::jxx::Ptr<SSLSocket> HandshakeCompletedEvent::getSocket() const {
    return ::jxx::CAST<SSLSocket>(getSource());
}

::jxx::Ptr<::jxx::lang::String>
HandshakeCompletedEvent::getCipherSuite() const {
    return session_->getCipherSuite();
}

::jxx::Ptr<HandshakeCompletedEvent::CertificateArray>
HandshakeCompletedEvent::getLocalCertificates() const {
    return session_->getLocalCertificates();
}

::jxx::Ptr<HandshakeCompletedEvent::CertificateArray>
HandshakeCompletedEvent::getPeerCertificates() const {
    return session_->getPeerCertificates();
}


::jxx::Ptr<HandshakeCompletedEvent::LegacyCertificateArray>
HandshakeCompletedEvent::getPeerCertificateChain() const {
    return nullptr;
}

::jxx::Ptr<::jxx::security::Principal>
HandshakeCompletedEvent::getLocalPrincipal() const {
    return session_->getLocalPrincipal();
}

::jxx::Ptr<::jxx::security::Principal>
HandshakeCompletedEvent::getPeerPrincipal() const {
    return session_->getPeerPrincipal();
}

} // namespace jxx::ext::net::ssl
