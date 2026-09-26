#include "ext/net/ssl/jxx.ext.net.ssl.HandshakeCompletedEvent.h"

#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocket.h"
#include "ext/security/cert/internal/jxx.ext.security.cert.internal.DerX509Certificate.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLPeerUnverifiedException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.IllegalStateException.h"

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
    const auto certificates = session_->getPeerCertificates();
    if (certificates == nullptr)
        throw ::jxx::ext::net::ssl::SSLPeerUnverifiedException(
            "peer not authenticated");

    const auto result = ::jxx::NEW<LegacyCertificateArray>(
        certificates->length);
    for (::jxx::lang::jint index = 0;
         index < certificates->length;
         ++index) {
        if ((*certificates)[index] == nullptr)
            throw ::jxx::ext::net::ssl::SSLPeerUnverifiedException(
                "peer certificate chain contains a null certificate");
        (*result)[index] = ::jxx::CAST<
            ::jxx::ext::security::cert::X509Certificate>(
                ::jxx::NEW<
                    ::jxx::ext::security::cert::internal::DerX509Certificate>(
                        (*certificates)[index]->getEncoded()));
    }
    return result;
}

::jxx::Ptr<::jxx::security::Principal>
HandshakeCompletedEvent::getLocalPrincipal() const {
    return session_->getLocalPrincipal();
}

::jxx::Ptr<::jxx::security::Principal>
HandshakeCompletedEvent::getPeerPrincipal() const {
    return session_->getPeerPrincipal();
}


void HandshakeCompletedEvent::writeObject(
    const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output) {
    if (output == nullptr) throw ::jxx::lang::NullPointerException();
    output->writeObject(::jxx::CAST<::jxx::lang::Object>(getSocket()));
    output->writeObject(::jxx::CAST<::jxx::lang::Object>(session_));
}

void HandshakeCompletedEvent::readObject(
    const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) {
    if (input == nullptr) throw ::jxx::lang::NullPointerException();
    const auto socket = ::jxx::CAST<SSLSocket>(input->readObject());
    const auto session = ::jxx::CAST<SSLSession>(input->readObject());
    if (socket == nullptr || session == nullptr)
        throw ::jxx::lang::IllegalStateException(
            "invalid serialized handshake-completed event");
    session_ = session;
}

void HandshakeCompletedEvent::readObjectNoData() {
    throw ::jxx::lang::IllegalStateException(
        "handshake-completed event requires serialized data");
}

} // namespace jxx::ext::net::ssl
