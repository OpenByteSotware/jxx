#pragma once

#include "ext/net/ssl/jxx.ext.net.ssl.SSLSession.h"
#include "util/jxx.util.EventObject.h"

namespace jxx::ext::net::ssl {

class SSLSocket;

class HandshakeCompletedEvent final
    : public ::jxx::lang::ClassBase<
          HandshakeCompletedEvent,
          ::jxx::util::EventObject> {
public:
    using JxxSuper = ::jxx::util::EventObject;
    using Super =
        ::jxx::lang::ClassBase<HandshakeCompletedEvent, JxxSuper>;
    using CertificateArray = SSLSession::CertificateArray;

    HandshakeCompletedEvent(
        const ::jxx::Ptr<SSLSocket>& socket,
        const ::jxx::Ptr<SSLSession>& session);

    ::jxx::Ptr<SSLSession> getSession() const;
    ::jxx::Ptr<SSLSocket> getSocket() const;
    ::jxx::Ptr<::jxx::lang::String> getCipherSuite() const;
    ::jxx::Ptr<CertificateArray> getLocalCertificates() const;
    ::jxx::Ptr<CertificateArray> getPeerCertificates() const;
    ::jxx::Ptr<::jxx::security::Principal> getLocalPrincipal() const;
    ::jxx::Ptr<::jxx::security::Principal> getPeerPrincipal() const;

private:
    ::jxx::Ptr<SSLSession> session_;
};

} // namespace jxx::ext::net::ssl
