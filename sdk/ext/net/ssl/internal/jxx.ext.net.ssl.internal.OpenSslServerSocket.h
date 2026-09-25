#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ext/net/ssl/jxx.ext.net.ssl.SSLServerSocket.h"

namespace jxx::ext::net::ssl::internal {

class OpenSslContextConfig;

class OpenSslServerSocket final
    : public ::jxx::lang::ClassBase<
          OpenSslServerSocket,
          ::jxx::ext::net::ssl::SSLServerSocket> {
public:
    using JxxSuper = ::jxx::ext::net::ssl::SSLServerSocket;
    using Super = ::jxx::lang::ClassBase<OpenSslServerSocket, JxxSuper>;

    OpenSslServerSocket(
        ::jxx::lang::jint port,
        ::jxx::lang::jint backlog,
        const ::jxx::Ptr<::jxx::net::InetAddress>& address,
        const std::shared_ptr<OpenSslContextConfig>& config);

    ::jxx::Ptr<::jxx::net::Socket> accept() override;
    ::jxx::Ptr<StringArray> getEnabledCipherSuites() const override;
    void setEnabledCipherSuites(const ::jxx::Ptr<StringArray>& suites) override;
    ::jxx::Ptr<StringArray> getSupportedCipherSuites() const override;
    ::jxx::Ptr<StringArray> getEnabledProtocols() const override;
    void setEnabledProtocols(const ::jxx::Ptr<StringArray>& protocols) override;
    ::jxx::Ptr<StringArray> getSupportedProtocols() const override;
    void setNeedClientAuth(::jxx::lang::jbool need) override;
    ::jxx::lang::jbool getNeedClientAuth() const override;
    void setWantClientAuth(::jxx::lang::jbool want) override;
    ::jxx::lang::jbool getWantClientAuth() const override;
    void setUseClientMode(::jxx::lang::jbool mode) override;
    ::jxx::lang::jbool getUseClientMode() const override;
    void setEnableSessionCreation(::jxx::lang::jbool enabled) override;
    ::jxx::lang::jbool getEnableSessionCreation() const override;

private:
    std::shared_ptr<OpenSslContextConfig> config_;
    std::vector<std::string> enabledCipherSuites_;
    std::vector<std::string> enabledProtocols_;
    ::jxx::lang::jbool needClientAuth_ = false;
    ::jxx::lang::jbool wantClientAuth_ = false;
    ::jxx::lang::jbool useClientMode_ = false;
    ::jxx::lang::jbool enableSessionCreation_ = true;
};

} // namespace jxx::ext::net::ssl::internal
