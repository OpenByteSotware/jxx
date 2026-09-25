#pragma once

#include "lang/jxx.lang.buildin_array.h"
#include "net/jxx.net.Socket.h"

namespace jxx::ext::net::ssl {

class HandshakeCompletedListener;
class SSLParameters;
class SSLSession;

class SSLSocket
    : public ::jxx::lang::ClassBase<
          SSLSocket,
          ::jxx::net::Socket> {
public:
    using JxxSuper = ::jxx::net::Socket;
    using Super = ::jxx::lang::ClassBase<SSLSocket, JxxSuper>;
    using StringArray = ::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::String>, 1U>;

    SSLSocket();
    ~SSLSocket() override = default;

    virtual ::jxx::Ptr<StringArray> getSupportedCipherSuites() const = 0;
    virtual ::jxx::Ptr<StringArray> getEnabledCipherSuites() const = 0;
    virtual void setEnabledCipherSuites(const ::jxx::Ptr<StringArray>& values) = 0;
    virtual ::jxx::Ptr<StringArray> getSupportedProtocols() const = 0;
    virtual ::jxx::Ptr<StringArray> getEnabledProtocols() const = 0;
    virtual void setEnabledProtocols(const ::jxx::Ptr<StringArray>& values) = 0;
    virtual ::jxx::Ptr<SSLSession> getSession() = 0;
    virtual void addHandshakeCompletedListener(const ::jxx::Ptr<HandshakeCompletedListener>& listener) = 0;
    virtual void removeHandshakeCompletedListener(const ::jxx::Ptr<HandshakeCompletedListener>& listener) = 0;
    virtual void startHandshake() = 0;
    virtual void setUseClientMode(::jxx::lang::jbool value) = 0;
    virtual ::jxx::lang::jbool getUseClientMode() const = 0;
    virtual void setNeedClientAuth(::jxx::lang::jbool value) = 0;
    virtual ::jxx::lang::jbool getNeedClientAuth() const = 0;
    virtual void setWantClientAuth(::jxx::lang::jbool value) = 0;
    virtual ::jxx::lang::jbool getWantClientAuth() const = 0;
    virtual void setEnableSessionCreation(::jxx::lang::jbool value) = 0;
    virtual ::jxx::lang::jbool getEnableSessionCreation() const = 0;
    virtual ::jxx::Ptr<SSLParameters> getSSLParameters() const;
    virtual void setSSLParameters(const ::jxx::Ptr<SSLParameters>& parameters);
};

} // namespace jxx::ext::net::ssl
