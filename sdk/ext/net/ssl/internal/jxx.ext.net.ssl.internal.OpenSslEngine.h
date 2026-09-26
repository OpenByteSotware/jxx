#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ext/net/ssl/jxx.ext.net.ssl.SSLEngine.h"

struct ssl_ctx_st;
struct ssl_st;
struct bio_st;

namespace jxx::ext::net::ssl::internal {

class OpenSslContextConfig;
class OpenSslManagerBridge;

class OpenSslEngine final
    : public ::jxx::lang::ClassBase<
          OpenSslEngine,
          ::jxx::ext::net::ssl::SSLEngine> {
public:
    using JxxSuper = ::jxx::ext::net::ssl::SSLEngine;
    using Super = ::jxx::lang::ClassBase<OpenSslEngine, JxxSuper>;
    using StringArray = JxxSuper::StringArray;
    using ByteBufferArray = JxxSuper::ByteBufferArray;

    OpenSslEngine(
        const std::shared_ptr<OpenSslContextConfig>& config,
        const ::jxx::Ptr<::jxx::lang::String>& peerHost,
        ::jxx::lang::jint peerPort);
    ~OpenSslEngine() override;

    void beginHandshake() override;
    void closeInbound() override;
    void closeOutbound() override;
    ::jxx::Ptr<::jxx::lang::Runnable> getDelegatedTask() override;
    ::jxx::Ptr<StringArray> getEnabledCipherSuites() const override;
    ::jxx::Ptr<StringArray> getEnabledProtocols() const override;
    ::jxx::lang::jbool getEnableSessionCreation() const override;
    SSLEngineResult::HandshakeStatus getHandshakeStatus() const override;
    ::jxx::lang::jbool getNeedClientAuth() const override;
    ::jxx::Ptr<SSLSession> getSession() const override;
    ::jxx::Ptr<SSLParameters> getSSLParameters() const override;
    void setSSLParameters(
        const ::jxx::Ptr<SSLParameters>& parameters) override;
    ::jxx::Ptr<StringArray> getSupportedCipherSuites() const override;
    ::jxx::Ptr<StringArray> getSupportedProtocols() const override;
    ::jxx::lang::jbool getUseClientMode() const override;
    ::jxx::lang::jbool getWantClientAuth() const override;
    ::jxx::lang::jbool isInboundDone() const override;
    ::jxx::lang::jbool isOutboundDone() const override;

    void setEnabledCipherSuites(const ::jxx::Ptr<StringArray>& suites) override;
    void setEnabledProtocols(const ::jxx::Ptr<StringArray>& protocols) override;
    void setEnableSessionCreation(::jxx::lang::jbool enabled) override;
    void setNeedClientAuth(::jxx::lang::jbool need) override;
    void setUseClientMode(::jxx::lang::jbool mode) override;
    void setWantClientAuth(::jxx::lang::jbool want) override;

    ::jxx::Ptr<SSLEngineResult> unwrap(
        const ::jxx::Ptr<::jxx::nio::ByteBuffer>& source,
        const ::jxx::Ptr<::jxx::nio::ByteBuffer>& destination) override;
    ::jxx::Ptr<SSLEngineResult> unwrap(
        const ::jxx::Ptr<::jxx::nio::ByteBuffer>& source,
        const ::jxx::Ptr<ByteBufferArray>& destinations,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) override;
    ::jxx::Ptr<SSLEngineResult> wrap(
        const ::jxx::Ptr<::jxx::nio::ByteBuffer>& source,
        const ::jxx::Ptr<::jxx::nio::ByteBuffer>& destination) override;
    ::jxx::Ptr<SSLEngineResult> wrap(
        const ::jxx::Ptr<ByteBufferArray>& sources,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length,
        const ::jxx::Ptr<::jxx::nio::ByteBuffer>& destination) override;

private:
    void ensureInitialized();
    void driveHandshake();
    void updateHandshakeStatus(int sslError);
    void completeSession();
    ::jxx::lang::jint drainNetwork(
        const ::jxx::Ptr<::jxx::nio::ByteBuffer>& destination);
    ::jxx::lang::jint feedNetwork(
        const ::jxx::Ptr<::jxx::nio::ByteBuffer>& source);
    static ::jxx::Ptr<StringArray> toArray(
        const std::vector<std::string>& values);
    static std::vector<std::string> toVector(
        const ::jxx::Ptr<StringArray>& values);

    std::shared_ptr<OpenSslContextConfig> config_;
    ssl_ctx_st* context_ = nullptr;
    ssl_st* ssl_ = nullptr;
    bio_st* inboundBio_ = nullptr;
    bio_st* outboundBio_ = nullptr;
    std::unique_ptr<OpenSslManagerBridge> managerBridge_;
    ::jxx::Ptr<SSLSession> session_;
    ::jxx::Ptr<::jxx::lang::String> endpointIdentificationAlgorithm_;
    ::jxx::Ptr<::jxx::lang::String> explicitSniHost_;
    std::vector<std::string> enabledCipherSuites_;
    std::vector<std::string> enabledProtocols_;
    SSLEngineResult::HandshakeStatus handshakeStatus_ =
        SSLEngineResult::HandshakeStatus::NOT_HANDSHAKING;
    ::jxx::lang::jbool clientMode_ = false;
    ::jxx::lang::jbool needClientAuth_ = false;
    ::jxx::lang::jbool wantClientAuth_ = false;
    ::jxx::lang::jbool enableSessionCreation_ = true;
    ::jxx::lang::jbool initialized_ = false;
    ::jxx::lang::jbool handshakeStarted_ = false;
    ::jxx::lang::jbool handshakeFinishedReported_ = false;
    ::jxx::lang::jbool inboundDone_ = false;
    ::jxx::lang::jbool outboundDone_ = false;
};

} // namespace jxx::ext::net::ssl::internal
