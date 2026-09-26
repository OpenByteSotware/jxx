#pragma once

#include "ext/net/ssl/jxx.ext.net.ssl.SSLEngineResult.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::lang { class Runnable; }
namespace jxx::nio { class ByteBuffer; }

namespace jxx::ext::net::ssl {

class SSLParameters;
class SSLSession;

class SSLEngine
    : public ::jxx::lang::ClassBase<
          SSLEngine,
          ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<SSLEngine, JxxSuper>;
    using StringArray = ::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::String>, 1U>;
    using ByteBufferArray = ::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::nio::ByteBuffer>, 1U>;

    ~SSLEngine() override = default;

    virtual void beginHandshake() = 0;
    virtual void closeInbound() = 0;
    virtual void closeOutbound() = 0;
    virtual ::jxx::Ptr<::jxx::lang::Runnable> getDelegatedTask() = 0;
    virtual ::jxx::Ptr<StringArray> getEnabledCipherSuites() const = 0;
    virtual ::jxx::Ptr<StringArray> getEnabledProtocols() const = 0;
    virtual ::jxx::lang::jbool getEnableSessionCreation() const = 0;
    virtual SSLEngineResult::HandshakeStatus getHandshakeStatus() const = 0;
    virtual ::jxx::lang::jbool getNeedClientAuth() const = 0;
    virtual ::jxx::Ptr<SSLSession> getSession() const = 0;
    virtual ::jxx::Ptr<SSLParameters> getSSLParameters() const;
    virtual ::jxx::Ptr<StringArray> getSupportedCipherSuites() const = 0;
    virtual ::jxx::Ptr<StringArray> getSupportedProtocols() const = 0;
    virtual ::jxx::lang::jbool getUseClientMode() const = 0;
    virtual ::jxx::lang::jbool getWantClientAuth() const = 0;
    virtual ::jxx::lang::jbool isInboundDone() const = 0;
    virtual ::jxx::lang::jbool isOutboundDone() const = 0;

    virtual void setEnabledCipherSuites(
        const ::jxx::Ptr<StringArray>& suites) = 0;
    virtual void setEnabledProtocols(
        const ::jxx::Ptr<StringArray>& protocols) = 0;
    virtual void setEnableSessionCreation(::jxx::lang::jbool enabled) = 0;
    virtual void setNeedClientAuth(::jxx::lang::jbool need) = 0;
    virtual void setSSLParameters(
        const ::jxx::Ptr<SSLParameters>& parameters);
    virtual void setUseClientMode(::jxx::lang::jbool mode) = 0;
    virtual void setWantClientAuth(::jxx::lang::jbool want) = 0;

    virtual ::jxx::Ptr<SSLEngineResult> unwrap(
        const ::jxx::Ptr<::jxx::nio::ByteBuffer>& source,
        const ::jxx::Ptr<::jxx::nio::ByteBuffer>& destination) = 0;
    virtual ::jxx::Ptr<SSLEngineResult> unwrap(
        const ::jxx::Ptr<::jxx::nio::ByteBuffer>& source,
        const ::jxx::Ptr<ByteBufferArray>& destinations,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length) = 0;
    virtual ::jxx::Ptr<SSLEngineResult> wrap(
        const ::jxx::Ptr<::jxx::nio::ByteBuffer>& source,
        const ::jxx::Ptr<::jxx::nio::ByteBuffer>& destination) = 0;
    virtual ::jxx::Ptr<SSLEngineResult> wrap(
        const ::jxx::Ptr<ByteBufferArray>& sources,
        ::jxx::lang::jint offset,
        ::jxx::lang::jint length,
        const ::jxx::Ptr<::jxx::nio::ByteBuffer>& destination) = 0;

    ::jxx::Ptr<::jxx::lang::String> getPeerHost() const;
    ::jxx::lang::jint getPeerPort() const noexcept;

protected:
    SSLEngine();
    SSLEngine(
        const ::jxx::Ptr<::jxx::lang::String>& peerHost,
        ::jxx::lang::jint peerPort);

private:
    ::jxx::Ptr<::jxx::lang::String> peerHost_;
    ::jxx::lang::jint peerPort_ = -1;
};

} // namespace jxx::ext::net::ssl
