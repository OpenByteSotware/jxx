#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocket.h"

namespace jxx::ext::net::ssl {
class HandshakeCompletedListener;
class SSLParameters;
class SSLSession;
}
namespace jxx::net {
class Socket;
class SocketAddress;
}
namespace jxx::ext::net::ssl::internal {

class OpenSslContextConfig;
class OpenSslSocketNative;

class OpenSslSocket final
    : public ::jxx::lang::ClassBase<
          OpenSslSocket,
          ::jxx::ext::net::ssl::SSLSocket> {
public:
    using JxxSuper = ::jxx::ext::net::ssl::SSLSocket;
    using Super = ::jxx::lang::ClassBase<OpenSslSocket, JxxSuper>;
    using StringArray = JxxSuper::StringArray;
    using SSLSession = ::jxx::ext::net::ssl::SSLSession;
    using HandshakeCompletedListener =
        ::jxx::ext::net::ssl::HandshakeCompletedListener;

    explicit OpenSslSocket(
        const std::shared_ptr<OpenSslContextConfig>& config);

    OpenSslSocket(
        const ::jxx::Ptr<::jxx::lang::String>& host,
        ::jxx::lang::jint port,
        const std::shared_ptr<OpenSslContextConfig>& config);

    OpenSslSocket(
        const ::jxx::Ptr<::jxx::net::Socket>& transport,
        const ::jxx::Ptr<::jxx::lang::String>& host,
        ::jxx::lang::jint port,
        ::jxx::lang::jbool autoClose,
        const std::vector<unsigned char>& consumed,
        const std::shared_ptr<OpenSslContextConfig>& config);

    ~OpenSslSocket() override;

    void connect(
        const ::jxx::Ptr<::jxx::net::SocketAddress>& endpoint) override;
    void connect(
        const ::jxx::Ptr<::jxx::net::SocketAddress>& endpoint,
        ::jxx::lang::jint timeout) override;
    void bind(
        const ::jxx::Ptr<::jxx::net::SocketAddress>& bindpoint) override;

    void startHandshake() override;
    ::jxx::Ptr<::jxx::io::InputStream> getInputStream() override;
    ::jxx::Ptr<::jxx::io::OutputStream> getOutputStream() override;
    void close() override;

    ::jxx::Ptr<StringArray> getSupportedCipherSuites() const override;
    ::jxx::Ptr<StringArray> getEnabledCipherSuites() const override;
    void setEnabledCipherSuites(
        const ::jxx::Ptr<StringArray>& values) override;
    ::jxx::Ptr<StringArray> getSupportedProtocols() const override;
    ::jxx::Ptr<StringArray> getEnabledProtocols() const override;
    void setEnabledProtocols(
        const ::jxx::Ptr<StringArray>& values) override;

    ::jxx::Ptr<SSLSession> getSession() override;
    void addHandshakeCompletedListener(
        const ::jxx::Ptr<HandshakeCompletedListener>& listener) override;
    void removeHandshakeCompletedListener(
        const ::jxx::Ptr<HandshakeCompletedListener>& listener) override;

    void setUseClientMode(::jxx::lang::jbool value) override;
    ::jxx::lang::jbool getUseClientMode() const override;
    void setNeedClientAuth(::jxx::lang::jbool value) override;
    ::jxx::lang::jbool getNeedClientAuth() const override;
    void setWantClientAuth(::jxx::lang::jbool value) override;
    ::jxx::lang::jbool getWantClientAuth() const override;
    void setEnableSessionCreation(::jxx::lang::jbool value) override;
    ::jxx::lang::jbool getEnableSessionCreation() const override;

    ::jxx::Ptr<::jxx::ext::net::ssl::SSLParameters>
    getSSLParameters() const override;
    void setSSLParameters(
        const ::jxx::Ptr<::jxx::ext::net::ssl::SSLParameters>& parameters)
        override;

    int tlsRead(unsigned char* data, int length);
    int tlsWrite(const unsigned char* data, int length);

private:
    std::shared_ptr<OpenSslContextConfig> config_;
    std::unique_ptr<OpenSslSocketNative> native_;
    ::jxx::Ptr<::jxx::lang::String> host_;
    ::jxx::Ptr<::jxx::lang::String> sniHost_;
    ::jxx::Ptr<::jxx::lang::String> endpointIdentificationAlgorithm_;
    ::jxx::Ptr<::jxx::net::Socket> pendingTransport_;
    ::jxx::lang::jint port_ = 0;
    ::jxx::Ptr<::jxx::net::Socket> transport_;
    ::jxx::lang::jbool autoClose_ = false;
    std::vector<unsigned char> consumed_;
    ::jxx::Ptr<SSLSession> session_;
    std::vector<::jxx::Ptr<HandshakeCompletedListener>> listeners_;
    std::vector<std::string> enabledCipherSuites_;
    std::vector<std::string> enabledProtocols_;
    ::jxx::lang::jbool client_ = true;
    ::jxx::lang::jbool need_ = false;
    ::jxx::lang::jbool want_ = false;
    ::jxx::lang::jbool create_ = true;
};

} // namespace jxx::ext::net::ssl::internal
