#include <gtest/gtest.h>

#include <cctype>
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
        if (std::isspace(character)) {
            continue;
        }
        if (character == '=') {
            break;
        }
        const auto position = alphabet.find(static_cast<char>(character));
        if (position == std::string::npos) {
            continue;
        }
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
    const auto store = loadTestKeyStore();
    const auto trustFactory =
        ::jxx::ext::net::ssl::TrustManagerFactory::getInstance(
            ::jxx::ext::net::ssl::TrustManagerFactory::getDefaultAlgorithm());
    trustFactory->init(store);

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

class HttpsServerTlsBehaviorTest : public ::testing::Test {
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
            ::jxx::NEW<::jxx::lang::String>("localhost"), port_);
        auto sslSocket = ::jxx::CAST<::jxx::ext::net::ssl::SSLSocket>(socket);
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

TEST_F(HttpsServerTlsBehaviorTest, CompletesTlsHandshake)
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

TEST_F(HttpsServerTlsBehaviorTest, ExposesServerCertificateToClient)
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

TEST_F(HttpsServerTlsBehaviorTest, RejectsClientThatDoesNotTrustCertificate)
{
    startServer();

    const auto context = ::jxx::ext::net::ssl::SSLContext::getInstance(
        ::jxx::NEW<::jxx::lang::String>("TLS"));
    context->init(nullptr, nullptr, nullptr);
    const auto socket = context->getSocketFactory()->createSocket(
        ::jxx::NEW<::jxx::lang::String>("localhost"), port_);
    client_ = ::jxx::CAST<::jxx::ext::net::ssl::SSLSocket>(socket);

    ASSERT_NE(nullptr, client_);
    client_->setUseClientMode(true);
    EXPECT_ANY_THROW(client_->startHandshake());
}

} // namespace
} // namespace jxx::com::sun::net::httpserver
