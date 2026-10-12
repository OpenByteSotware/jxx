#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "ext/net/ssl/jxx.ext.net.ssl.HandshakeCompletedEvent.h"
#include "ext/net/ssl/jxx.ext.net.ssl.HostnameVerifier.h"
#include "ext/net/ssl/jxx.ext.net.ssl.KeyManagerFactory.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLEngine.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLException.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSession.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocket.h"
#include "ext/net/ssl/jxx.ext.net.ssl.TrustManagerFactory.h"

TEST(JxxSslClassInitialization, PublicTypesHaveClassMetadata)
{
    ::jxx::initialize();

    EXPECT_NE(::jxx::ext::net::ssl::HandshakeCompletedEvent::Class(), nullptr);
    EXPECT_NE(::jxx::ext::net::ssl::HostnameVerifier::Class(), nullptr);
    EXPECT_NE(::jxx::ext::net::ssl::KeyManagerFactory::Class(), nullptr);
    EXPECT_NE(::jxx::ext::net::ssl::SSLContext::Class(), nullptr);
    EXPECT_NE(::jxx::ext::net::ssl::SSLEngine::Class(), nullptr);
    EXPECT_NE(::jxx::ext::net::ssl::SSLException::Class(), nullptr);
    EXPECT_NE(::jxx::ext::net::ssl::SSLParameters::Class(), nullptr);
    EXPECT_NE(::jxx::ext::net::ssl::SSLSession::Class(), nullptr);
    EXPECT_NE(::jxx::ext::net::ssl::SSLSocket::Class(), nullptr);
    EXPECT_NE(::jxx::ext::net::ssl::TrustManagerFactory::Class(), nullptr);
}
