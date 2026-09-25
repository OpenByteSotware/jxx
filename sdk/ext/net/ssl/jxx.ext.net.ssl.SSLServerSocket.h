#pragma once

#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "net/jxx.net.ServerSocket.h"

namespace jxx::ext::net::ssl {

class SSLServerSocket
    : public ::jxx::lang::ClassBase<
          SSLServerSocket,
          ::jxx::net::ServerSocket> {
public:
    using JxxSuper = ::jxx::net::ServerSocket;
    using Super = ::jxx::lang::ClassBase<SSLServerSocket, JxxSuper>;

    using StringArray = ::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::String>, 1U>;

    explicit SSLServerSocket(::jxx::lang::jint port);
    SSLServerSocket(::jxx::lang::jint port, ::jxx::lang::jint backlog);
    SSLServerSocket(
        ::jxx::lang::jint port,
        ::jxx::lang::jint backlog,
        const ::jxx::Ptr<::jxx::net::InetAddress>& address);
    ~SSLServerSocket() override = default;

    virtual ::jxx::Ptr<StringArray> getEnabledCipherSuites() const = 0;
    virtual void setEnabledCipherSuites(const ::jxx::Ptr<StringArray>& suites) = 0;
    virtual ::jxx::Ptr<StringArray> getSupportedCipherSuites() const = 0;
    virtual ::jxx::Ptr<StringArray> getEnabledProtocols() const = 0;
    virtual void setEnabledProtocols(const ::jxx::Ptr<StringArray>& protocols) = 0;
    virtual ::jxx::Ptr<StringArray> getSupportedProtocols() const = 0;
    virtual void setNeedClientAuth(::jxx::lang::jbool need) = 0;
    virtual ::jxx::lang::jbool getNeedClientAuth() const = 0;
    virtual void setWantClientAuth(::jxx::lang::jbool want) = 0;
    virtual ::jxx::lang::jbool getWantClientAuth() const = 0;
    virtual void setUseClientMode(::jxx::lang::jbool mode) = 0;
    virtual ::jxx::lang::jbool getUseClientMode() const = 0;
    virtual void setEnableSessionCreation(::jxx::lang::jbool enabled) = 0;
    virtual ::jxx::lang::jbool getEnableSessionCreation() const = 0;
};

} // namespace jxx::ext::net::ssl
