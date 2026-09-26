#include <gtest/gtest.h>

#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLHandshakeException.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLEngine.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SNIHostName.h"
#include "nio/jxx.nio.ByteBuffer.h"
#include "util/jxx.util.ArrayList.h"

namespace {
using namespace jxx::ext::net::ssl;

TEST(OpenSslEngineTest, ContextCreatesClientAndServerEngines) {
    const auto context = SSLContext::getInstance(
        ::jxx::NEW<::jxx::lang::String>("TLS"));
    const auto client = context->createSSLEngine(
        ::jxx::NEW<::jxx::lang::String>("localhost"), 443);
    const auto server = context->createSSLEngine();
    client->setUseClientMode(true);
    server->setUseClientMode(false);
    EXPECT_TRUE(client->getUseClientMode());
    EXPECT_FALSE(server->getUseClientMode());
    EXPECT_EQ(client->getPeerPort(), 443);
    EXPECT_EQ(server->getPeerPort(), -1);
}

TEST(OpenSslEngineTest, AppliesSniAndHttpsEndpointIdentification) {
    const auto context = SSLContext::getInstance(
        ::jxx::NEW<::jxx::lang::String>("TLS"));
    const auto engine = context->createSSLEngine(
        ::jxx::NEW<::jxx::lang::String>("127.0.0.1"), 443);
    engine->setUseClientMode(true);
    auto parameters = engine->getSSLParameters();
    parameters->setEndpointIdentificationAlgorithm(
        ::jxx::NEW<::jxx::lang::String>("HTTPS"));
    const auto names = ::jxx::NEW<
        ::jxx::util::ArrayList<SNIServerName>>();
    names->add(::jxx::NEW<SNIHostName>(
        ::jxx::NEW<::jxx::lang::String>("localhost")));
    parameters->setServerNames(names);
    engine->setSSLParameters(parameters);
    const auto copy = engine->getSSLParameters();
    ASSERT_NE(copy->getEndpointIdentificationAlgorithm(), nullptr);
    EXPECT_EQ(copy->getEndpointIdentificationAlgorithm()->utf8(), "HTTPS");
    ASSERT_NE(copy->getServerNames(), nullptr);
    EXPECT_EQ(copy->getServerNames()->size(), 1);
}

TEST(OpenSslEngineTest, InitialClientHandshakeNeedsWrap) {
    const auto context = SSLContext::getInstance(
        ::jxx::NEW<::jxx::lang::String>("TLS"));
    const auto client = context->createSSLEngine(
        ::jxx::NEW<::jxx::lang::String>("localhost"), 443);
    client->setUseClientMode(true);
    client->beginHandshake();
    EXPECT_EQ(client->getHandshakeStatus(),
        SSLEngineResult::HandshakeStatus::NEED_WRAP);
    const auto empty = ::jxx::nio::ByteBuffer::allocate(0);
    const auto network = ::jxx::nio::ByteBuffer::allocate(32768);
    const auto result = client->wrap(empty, network);
    EXPECT_EQ(result->getStatus(), SSLEngineResult::Status::OK);
    EXPECT_GT(result->bytesProduced(), 0);
}

TEST(OpenSslEngineTest, EmptyServerInputReportsUnderflow) {
    const auto context = SSLContext::getInstance(
        ::jxx::NEW<::jxx::lang::String>("TLS"));
    const auto server = context->createSSLEngine();
    server->setUseClientMode(false);
    const auto emptyNetwork = ::jxx::nio::ByteBuffer::allocate(0);
    const auto application = ::jxx::nio::ByteBuffer::allocate(16384);
    EXPECT_THROW(
        server->unwrap(emptyNetwork, application),
        ::jxx::ext::net::ssl::SSLHandshakeException);
}

} // namespace
