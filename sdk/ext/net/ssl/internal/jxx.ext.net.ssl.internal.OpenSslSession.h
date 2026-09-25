#pragma once

#include "ext/net/ssl/jxx.ext.net.ssl.SSLSession.h"

namespace jxx::ext::net::ssl::internal {

class OpenSslSession final
    : public ::jxx::lang::ClassBase<
          OpenSslSession,
          ::jxx::lang::Object,
          ::jxx::ext::net::ssl::SSLSession> {
public:
    using CertificateArray =
        ::jxx::ext::net::ssl::SSLSession::CertificateArray;

    OpenSslSession(
        const ::jxx::Ptr<::jxx::lang::String>& cipher,
        const ::jxx::Ptr<::jxx::lang::String>& protocol,
        const ::jxx::Ptr<::jxx::lang::String>& host,
        ::jxx::lang::jint port,
        const ::jxx::Ptr<CertificateArray>& peerCertificates = nullptr,
        const ::jxx::Ptr<CertificateArray>& localCertificates = nullptr);

    ::jxx::lang::ByteArray getId() const override;
    ::jxx::Ptr<::jxx::lang::String> getCipherSuite() const override;
    ::jxx::Ptr<::jxx::lang::String> getProtocol() const override;
    ::jxx::Ptr<::jxx::lang::String> getPeerHost() const override;
    ::jxx::lang::jint getPeerPort() const override;
    ::jxx::Ptr<CertificateArray> getLocalCertificates() const override;
    ::jxx::Ptr<CertificateArray> getPeerCertificates() const override;
    ::jxx::Ptr<::jxx::security::Principal> getPeerPrincipal() const override;
    ::jxx::Ptr<::jxx::security::Principal> getLocalPrincipal() const override;
    ::jxx::lang::jbool isValid() const override;
    void invalidate() override;

private:
    ::jxx::Ptr<::jxx::lang::String> cipher_;
    ::jxx::Ptr<::jxx::lang::String> protocol_;
    ::jxx::Ptr<::jxx::lang::String> host_;
    ::jxx::lang::jint port_;
    ::jxx::Ptr<CertificateArray> peerCertificates_;
    ::jxx::Ptr<CertificateArray> localCertificates_;
    ::jxx::lang::jbool valid_ = true;
};

} // namespace jxx::ext::net::ssl::internal
