#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.ClassInitialization.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.DefaultHostnameVerifier.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.DerCertificate.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.HandshakeNotificationTask.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslCompositeKeyManager.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslContextSpi.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslDefaultKeyManager.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslDefaultTrustManager.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslEngine.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslHttpsURLConnection.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslKeyManagerFactorySpi.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslKeyStoreKeyManager.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPropertyTrustManager.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPublicKey.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslServerSocket.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslServerSocketFactory.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSession.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSessionContext.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocket.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocketFactory.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslTrustManagerFactorySpi.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslX509Certificate.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.X509Principal.h"

namespace T = ::jxx::ext::net::ssl::internal;

TEST(JxxStage55ClassInitialization, MetadataAndIdempotence)
{
    ::jxx::initialize();
    using namespace ::jxx::ext::net::ssl::internal;
    EXPECT_NE(DefaultHostnameVerifier::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(DerCertificate::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(HandshakeNotificationTask::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(OpenSslCompositeKeyManager::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(OpenSslContextSpi::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(OpenSslDefaultKeyManager::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(OpenSslDefaultTrustManager::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(OpenSslEngine::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(OpenSslHttpsURLConnection::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(OpenSslKeyManagerFactorySpi::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(OpenSslKeyStoreKeyManager::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(OpenSslPropertyTrustManager::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(OpenSslPublicKey::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(OpenSslServerSocket::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(OpenSslServerSocketFactory::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(OpenSslSession::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(OpenSslSessionContext::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(OpenSslSocket::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(OpenSslSocketFactory::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(OpenSslTrustManagerFactorySpi::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(OpenSslX509Certificate::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(X509Principal::JxxClassInfoMarker::Class(), nullptr);
    const auto before = DefaultHostnameVerifier::JxxClassInfoMarker::Class();
    EXPECT_NO_THROW(T::initializeClasses());
    EXPECT_NO_THROW(T::initializeClasses());
    EXPECT_EQ(before, DefaultHostnameVerifier::JxxClassInfoMarker::Class());
}
