#pragma once

#include <chrono>
#include <mutex>
#include <string>
#include <unordered_map>

#include "ext/net/ssl/jxx.ext.net.ssl.SSLSession.h"

namespace jxx::ext::net::ssl::internal {

class OpenSslSession final
    : public ::jxx::lang::ClassBase<
          OpenSslSession,
          ::jxx::lang::Object,
          ::jxx::ext::net::ssl::SSLSession> {
public:
    using CertificateArray = SSLSession::CertificateArray;
    using StringArray = SSLSession::StringArray;

    OpenSslSession(
        const ::jxx::Ptr<::jxx::lang::String>& cipher,
        const ::jxx::Ptr<::jxx::lang::String>& protocol,
        const ::jxx::Ptr<::jxx::lang::String>& host,
        ::jxx::lang::jint port,
        const ::jxx::Ptr<CertificateArray>& peerCertificates = nullptr,
        const ::jxx::Ptr<CertificateArray>& localCertificates = nullptr,
        const ::jxx::lang::ByteArray& id = nullptr,
        const ::jxx::Ptr<::jxx::ext::net::ssl::SSLSessionContext>& context = nullptr);

    ::jxx::lang::jint getApplicationBufferSize() const override;
    ::jxx::Ptr<::jxx::lang::String> getCipherSuite() const override;
    ::jxx::lang::jlong getCreationTime() const override;
    ::jxx::lang::ByteArray getId() const override;
    ::jxx::lang::jlong getLastAccessedTime() const override;
    ::jxx::Ptr<CertificateArray> getLocalCertificates() const override;
    ::jxx::Ptr<::jxx::security::Principal> getLocalPrincipal() const override;
    ::jxx::lang::jint getPacketBufferSize() const override;
    ::jxx::Ptr<CertificateArray> getPeerCertificates() const override;
    ::jxx::Ptr<::jxx::lang::String> getPeerHost() const override;
    ::jxx::lang::jint getPeerPort() const override;
    ::jxx::Ptr<::jxx::security::Principal> getPeerPrincipal() const override;
    ::jxx::Ptr<::jxx::lang::String> getProtocol() const override;
    ::jxx::Ptr<::jxx::ext::net::ssl::SSLSessionContext>
    getSessionContext() const override;
    ::jxx::Ptr<::jxx::lang::Object> getValue(
        const ::jxx::Ptr<::jxx::lang::String>& name) const override;
    ::jxx::Ptr<StringArray> getValueNames() const override;
    void invalidate() override;
    ::jxx::lang::jbool isValid() const override;
    void putValue(
        const ::jxx::Ptr<::jxx::lang::String>& name,
        const ::jxx::Ptr<::jxx::lang::Object>& value) override;
    void removeValue(
        const ::jxx::Ptr<::jxx::lang::String>& name) override;

private:
    static ::jxx::lang::jlong nowMillis();
    void touch() const;

    ::jxx::Ptr<::jxx::lang::String> cipher_;
    ::jxx::Ptr<::jxx::lang::String> protocol_;
    ::jxx::Ptr<::jxx::lang::String> host_;
    ::jxx::lang::jint port_;
    ::jxx::Ptr<CertificateArray> peerCertificates_;
    ::jxx::Ptr<CertificateArray> localCertificates_;
    ::jxx::lang::ByteArray id_;
    ::jxx::Ptr<::jxx::ext::net::ssl::SSLSessionContext> context_;
    ::jxx::lang::jlong creationTime_;
    mutable ::jxx::lang::jlong lastAccessedTime_;
    ::jxx::lang::jbool valid_ = true;
    mutable std::mutex mutex_;
    std::unordered_map<std::string, ::jxx::Ptr<::jxx::lang::Object>> values_;
};

} // namespace jxx::ext::net::ssl::internal
