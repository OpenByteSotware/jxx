#pragma once

#include "ext/net/ssl/jxx.ext.net.ssl.SSLSession.h"
#include "util/jxx.util.EventObject.h"
#include "io/jxx.io.SerializableI.h"

namespace jxx::ext::security::cert { class X509Certificate; }
namespace jxx::ext::net::ssl {

class SSLSocket;

class HandshakeCompletedEvent final
    : public ::jxx::lang::ClassBase<
          HandshakeCompletedEvent,
          ::jxx::util::EventObject,
          ::jxx::io::SerializableI> {
public:
    using JxxSuper = ::jxx::util::EventObject;
    using Super = ::jxx::lang::ClassBase<
        HandshakeCompletedEvent,
        JxxSuper,
        ::jxx::io::SerializableI>;
    using CertificateArray = SSLSession::CertificateArray;
    using LegacyCertificateArray = ::jxx::lang::JxxArray<::jxx::Ptr<::jxx::ext::security::cert::X509Certificate>,1U>;

    HandshakeCompletedEvent(
        const ::jxx::Ptr<SSLSocket>& socket,
        const ::jxx::Ptr<SSLSession>& session);

    ::jxx::Ptr<SSLSession> getSession() const;
    ::jxx::Ptr<SSLSocket> getSocket() const;
    ::jxx::Ptr<::jxx::lang::String> getCipherSuite() const;
    ::jxx::Ptr<CertificateArray> getLocalCertificates() const;
    ::jxx::Ptr<CertificateArray> getPeerCertificates() const;
    ::jxx::Ptr<LegacyCertificateArray> getPeerCertificateChain() const;
    ::jxx::Ptr<::jxx::security::Principal> getLocalPrincipal() const;
    ::jxx::Ptr<::jxx::security::Principal> getPeerPrincipal() const;

    void writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& output) override;
    void readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>& input) override;
    void readObjectNoData() override;

private:
    ::jxx::Ptr<SSLSession> session_;
};

} // namespace jxx::ext::net::ssl
