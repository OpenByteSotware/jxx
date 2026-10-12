#include <gtest/gtest.h>

#include <cctype>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpsConfigurator.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpsServer.h"
#include "ext/net/ssl/jxx.ext.net.ssl.KeyManagerFactory.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSession.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocket.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocketFactory.h"
#include "ext/net/ssl/jxx.ext.net.ssl.TrustManagerFactory.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.ServerSocket.h"
#include "security/jxx.security.KeyStore.h"
#include "security/cert/jxx.security.cert.Certificate.h"

namespace jxx::com::sun::net::httpserver {
namespace {

const char* PKCS12_BASE64 = R"PKCS12(
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
)PKCS12";

::jxx::lang::ByteArray decodeBase64(const char* text)
{
    static const std::string alphabet =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::vector<unsigned char> bytes;
    ::jxx::lang::jint value = 0;
    ::jxx::lang::jint bits = -8;
    for (const unsigned char character : std::string(text)) {
        if (std::isspace(character)) continue;
        if (character == '=') break;
        const auto position = alphabet.find(static_cast<char>(character));
        if (position == std::string::npos) continue;
        value = (value << 6) + static_cast<::jxx::lang::jint>(position);
        bits += 6;
        if (bits >= 0) {
            bytes.push_back(static_cast<unsigned char>((value >> bits) & 0xFF));
            bits -= 8;
        }
    }
    auto result = ::jxx::NEW<::jxx::lang::ByteArrayType>(
        static_cast<std::uint32_t>(bytes.size()));
    for (::jxx::lang::jint index = 0;
         index < static_cast<::jxx::lang::jint>(bytes.size()); ++index) {
        (*result)[index] = static_cast<::jxx::lang::jbyte>(bytes[index]);
    }
    return result;
}

::jxx::Ptr<::jxx::security::KeyStore> loadTestKeyStore()
{
    const auto type = ::jxx::NEW<::jxx::lang::String>("PKCS12");
    const auto passwordText = ::jxx::NEW<::jxx::lang::String>("changeit");
    const auto password = passwordText->toCharArray();
    const auto bytes = decodeBase64(PKCS12_BASE64);
    const auto input = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes);
    const auto store = ::jxx::security::KeyStore::getInstance(type);
    store->load(input, password);
    return store;
}

::jxx::Ptr<::jxx::ext::net::ssl::SSLContext> createServerContext()
{
    const auto store = loadTestKeyStore();
    const auto password =
        ::jxx::NEW<::jxx::lang::String>("changeit")->toCharArray();
    const auto keyFactory =
        ::jxx::ext::net::ssl::KeyManagerFactory::getInstance(
            ::jxx::ext::net::ssl::KeyManagerFactory::getDefaultAlgorithm());
    keyFactory->init(store, password);
    const auto context = ::jxx::ext::net::ssl::SSLContext::getInstance(
        ::jxx::NEW<::jxx::lang::String>("TLS"));
    context->init(keyFactory->getKeyManagers(), nullptr, nullptr);
    return context;
}

::jxx::Ptr<::jxx::ext::net::ssl::SSLContext> createClientContext()
{
    const auto identityStore = loadTestKeyStore();
    const auto aliases = identityStore->aliases();
    if (aliases == nullptr || aliases->length == 0) {
        throw std::runtime_error("test PKCS12 contains no aliases");
    }

    ::jxx::Ptr<::jxx::security::cert::Certificate> serverCertificate;
    for (::jxx::lang::jint index = 0;
         index < static_cast<::jxx::lang::jint>(aliases->length); ++index) {
        const ::jxx::Ptr<::jxx::lang::String> certificateAlias =
            (*aliases)[index];
        if (certificateAlias == nullptr) continue;
        const auto certificate = identityStore->getCertificate(certificateAlias);
        if (certificate != nullptr) {
            serverCertificate = certificate;
            break;
        }
    }
    if (serverCertificate == nullptr) {
        throw std::runtime_error("test PKCS12 contains no certificate");
    }

    const auto trustStore = ::jxx::security::KeyStore::getInstance(
        ::jxx::NEW<::jxx::lang::String>("PKCS12"));
    trustStore->load(nullptr, nullptr);
    trustStore->setCertificateEntry(
        ::jxx::NEW<::jxx::lang::String>("test-server"),
        serverCertificate);

    const auto trustFactory =
        ::jxx::ext::net::ssl::TrustManagerFactory::getInstance(
            ::jxx::ext::net::ssl::TrustManagerFactory::getDefaultAlgorithm());
    trustFactory->init(trustStore);
    const auto context = ::jxx::ext::net::ssl::SSLContext::getInstance(
        ::jxx::NEW<::jxx::lang::String>("TLS"));
    context->init(nullptr, trustFactory->getTrustManagers(), nullptr);
    return context;
}

::jxx::lang::jint reserveLoopbackPort()
{
    const auto socket = ::jxx::NEW<::jxx::net::ServerSocket>(0);
    const auto port = socket->getLocalPort();
    socket->close();
    return port;
}

class HttpsServerTlsBehaviorTest1A : public ::testing::Test {
protected:
    void TearDown() override
    {
        if (client_ != nullptr) {
            try { client_->close(); } catch (...) {}
        }
        if (server_ != nullptr) {
            try { server_->stop(0); } catch (...) {}
        }
    }

    void startServer()
    {
        port_ = reserveLoopbackPort();
        const auto address =
            ::jxx::NEW<::jxx::net::InetSocketAddress>(port_);
        server_ = HttpsServer::create(address, 0);
        server_->setHttpsConfigurator(
            ::jxx::NEW<HttpsConfigurator>(createServerContext()));
        server_->start();
    }

    ::jxx::Ptr<::jxx::ext::net::ssl::SSLSocket> connectClient()
    {
        const auto factory = createClientContext()->getSocketFactory();
        const auto socket = factory->createSocket(
            ::jxx::NEW<::jxx::lang::String>("127.0.0.1"), port_);
        auto sslSocket =
            ::jxx::CAST<::jxx::ext::net::ssl::SSLSocket>(socket);
        if (sslSocket != nullptr) {
            sslSocket->setUseClientMode(true);
            sslSocket->startHandshake();
        }
        client_ = sslSocket;
        return sslSocket;
    }

    ::jxx::Ptr<HttpsServer> server_;
    ::jxx::Ptr<::jxx::ext::net::ssl::SSLSocket> client_;
    ::jxx::lang::jint port_ = 0;
};

TEST_F(HttpsServerTlsBehaviorTest1A, CompletesTlsHandshake)
{
    startServer();
    const auto socket = connectClient();
    ASSERT_NE(nullptr, socket);
    const auto session = socket->getSession();
    ASSERT_NE(nullptr, session);
    EXPECT_TRUE(session->isValid());
    ASSERT_NE(nullptr, session->getProtocol());
    EXPECT_FALSE(session->getProtocol()->isEmpty());
    ASSERT_NE(nullptr, session->getCipherSuite());
    EXPECT_FALSE(session->getCipherSuite()->isEmpty());
}

TEST_F(HttpsServerTlsBehaviorTest1A, ExposesServerCertificateToClient)
{
    startServer();
    const auto socket = connectClient();
    ASSERT_NE(nullptr, socket);
    const auto session = socket->getSession();
    ASSERT_NE(nullptr, session);
    const auto certificates = session->getPeerCertificates();
    ASSERT_NE(nullptr, certificates);
    ASSERT_GT(certificates->length, 0U);
    ASSERT_NE(nullptr, (*certificates)[0]);
    ASSERT_NE(nullptr, (*certificates)[0]->getType());
    EXPECT_TRUE((*certificates)[0]->getType()->equals(
        ::jxx::NEW<::jxx::lang::String>("X.509")));
}

TEST_F(HttpsServerTlsBehaviorTest1A, RejectsClientThatDoesNotTrustCertificate)
{
    startServer();
    const auto context = ::jxx::ext::net::ssl::SSLContext::getInstance(
        ::jxx::NEW<::jxx::lang::String>("TLS"));
    context->init(nullptr, nullptr, nullptr);
    const auto socket = context->getSocketFactory()->createSocket(
        ::jxx::NEW<::jxx::lang::String>("127.0.0.1"), port_);
    client_ = ::jxx::CAST<::jxx::ext::net::ssl::SSLSocket>(socket);
    ASSERT_NE(nullptr, client_);
    client_->setUseClientMode(true);
    EXPECT_ANY_THROW(client_->startHandshake());
}

} // namespace
} // namespace jxx::com::sun::net::httpserver
