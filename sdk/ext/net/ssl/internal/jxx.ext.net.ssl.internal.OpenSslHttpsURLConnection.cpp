#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>


#include "ext/net/ssl/jxx.ext.net.ssl.HostnameVerifier.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLPeerUnverifiedException.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSession.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocket.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocketFactory.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.IOException.h"
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.OutputStream.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.ProtocolException.h"
#include "net/jxx.net.URL.h"
#include "security/cert/jxx.security.cert.Certificate.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslHttpsURLConnection.h"

namespace jxx::ext::net::ssl::internal {
namespace {

std::string lower(std::string value) {
    std::transform(
        value.begin(),
        value.end(),
        value.begin(),
        [](unsigned char current) {
            return static_cast<char>(std::tolower(current));
        });
    return value;
}

} // namespace

OpenSslHttpsURLConnection::OpenSslHttpsURLConnection(
    const ::jxx::Ptr<::jxx::net::URL>& url)
    : Super(url) {
}

OpenSslHttpsURLConnection::~OpenSslHttpsURLConnection() {
    releaseSocket();
}

void OpenSslHttpsURLConnection::releaseSocket() noexcept {
    if (socket_ != nullptr) {
        try { socket_->close(); } catch (...) { }
    }
    input_ = nullptr;
    output_ = nullptr;
    session_ = nullptr;
    socket_ = nullptr;
}

void OpenSslHttpsURLConnection::connect() {
    if (connected_) return;
    const auto url = getURL();
    if (url == nullptr || url->getHost() == nullptr)
        throw ::jxx::lang::IllegalStateException();
    host_ = url->getHost()->utf8();
    const auto host = ::jxx::NEW<::jxx::lang::String>(host_);
    const ::jxx::lang::jint port = url->getPort() < 0 ? 443 : url->getPort();
    try {
        const auto factory = getSSLSocketFactory();
        if (factory == nullptr)
            throw ::jxx::lang::IllegalStateException("HTTPS SSLSocketFactory is not configured");
        socket_ = ::jxx::CAST<::jxx::ext::net::ssl::SSLSocket>(factory->createSocket());
        if (socket_ == nullptr)
            throw ::jxx::io::IOException("configured SSLSocketFactory did not create an SSLSocket");
        const auto parameters = socket_->getSSLParameters();
        parameters->setEndpointIdentificationAlgorithm(
            ::jxx::NEW<::jxx::lang::String>("HTTPS"));
        socket_->setSSLParameters(parameters);
        socket_->connect(
            ::jxx::NEW<::jxx::net::InetSocketAddress>(host, port),
            getConnectTimeout());
        socket_->setSoTimeout(getReadTimeout());
        socket_->startHandshake();
        session_ = socket_->getSession();
        if (session_ == nullptr)
            throw ::jxx::io::IOException("HTTPS handshake did not produce an SSLSession");
        const auto verifier = getHostnameVerifier();
        if (verifier == nullptr)
            throw ::jxx::lang::IllegalStateException(
                "HTTPS HostnameVerifier is not configured");
        if (!verifier->verify(host, session_))
            throw ::jxx::ext::net::ssl::SSLPeerUnverifiedException(
                "HTTPS hostname verifier rejected peer");
        cipherSuite_ = session_->getCipherSuite();
        captureSessionCertificates();
        input_ = socket_->getInputStream();
        output_ = socket_->getOutputStream();
        connected_ = true;
    } catch (...) {
        releaseSocket();
        throw;
    }
}

void OpenSslHttpsURLConnection::captureSessionCertificates() {
    serverCertificates_ = session_->getPeerCertificates();
    localCertificates_ = session_->getLocalCertificates();
}

void OpenSslHttpsURLConnection::executeRequest() {
    connect();

    const auto url = getURL();
    std::string target =
        url->getFile() == nullptr
            ? "/"
            : url->getFile()->utf8();
    if (target.empty()) {
        target = "/";
    }

    const std::string method =
        getRequestMethod() == nullptr
            ? "GET"
            : getRequestMethod()->utf8();
    if (method != "GET" && method != "HEAD") {
        throw ::jxx::net::ProtocolException(
            ::jxx::NEW<::jxx::lang::String>(
                "OpenSSL HTTPS backend currently supports GET and HEAD"));
    }

    std::string request =
        method + " " + target + " HTTP/1.1\r\n" +
        "Host: " + host_ + "\r\n" +
        "Connection: close\r\n" +
        "Accept-Encoding: identity\r\n";
    for (const auto& item : requestProps_) {
        request += item.first + ": " + item.second + "\r\n";
    }
    request += "\r\n";

    const auto requestBytes = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(
            static_cast<::jxx::lang::jint>(request.size()));
    for (std::size_t index = 0; index < request.size(); ++index)
        (*requestBytes)[static_cast<::jxx::lang::jint>(index)] =
            static_cast<::jxx::lang::jbyte>(request[index]);
    output_->write(requestBytes);
    output_->flush();

    std::string response;
    const auto buffer = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(4096);
    for (;;) {
        const auto count = input_->read(buffer, 0, buffer->length);
        if (count < 0) break;
        if (count == 0) continue;
        for (::jxx::lang::jint index = 0; index < count; ++index)
            response.push_back(static_cast<char>((*buffer)[index]));
    }

    const auto split = response.find("\r\n\r\n");
    if (split == std::string::npos) {
        throw ::jxx::io::IOException("Invalid HTTP response");
    }

    std::istringstream headers(response.substr(0, split));
    std::string status;
    std::getline(headers, status);
    if (!status.empty() && status.back() == '\r') {
        status.pop_back();
    }

    std::istringstream statusLine(status);
    std::string version;
    statusLine >> version >> responseCode_;
    std::string reason;
    std::getline(statusLine, reason);
    if (!reason.empty() && reason.front() == ' ') {
        reason.erase(reason.begin());
    }
    responseMessage_ =
        ::jxx::NEW<::jxx::lang::String>(reason);

    std::string line;
    while (std::getline(headers, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        const auto colon = line.find(':');
        if (colon == std::string::npos) {
            continue;
        }
        auto name = lower(line.substr(0, colon));
        auto value = line.substr(colon + 1);
        while (!value.empty() &&
               std::isspace(
                   static_cast<unsigned char>(value.front()))) {
            value.erase(value.begin());
        }
        headerFields_[name] = value;
    }

    const std::string body = response.substr(split + 4);
    responseBody_ =
        ::jxx::NEW<
            ::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(
                static_cast<::jxx::lang::jint>(body.size()));
    for (std::size_t index = 0; index < body.size(); ++index) {
        (*responseBody_)[static_cast<::jxx::lang::jint>(index)] =
            static_cast<::jxx::lang::jbyte>(body[index]);
    }
}

::jxx::Ptr<::jxx::net::URLConnection>
openHttpsConnection(
    const ::jxx::Ptr<::jxx::net::URL>& url) {
    if (url == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    return ::jxx::NEW<OpenSslHttpsURLConnection>(url);
}

} // namespace jxx::ext::net::ssl::internal
