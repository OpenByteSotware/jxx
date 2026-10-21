#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.ClassInitialization.h"

#include <mutex>

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

namespace jxx::ext::net::ssl::internal {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)DefaultHostnameVerifier::JxxClassInfoMarker::Class();
        (void)DerCertificate::JxxClassInfoMarker::Class();
        (void)HandshakeNotificationTask::JxxClassInfoMarker::Class();
        (void)OpenSslCompositeKeyManager::JxxClassInfoMarker::Class();
        (void)OpenSslContextSpi::JxxClassInfoMarker::Class();
        (void)OpenSslDefaultKeyManager::JxxClassInfoMarker::Class();
        (void)OpenSslDefaultTrustManager::JxxClassInfoMarker::Class();
        (void)OpenSslEngine::JxxClassInfoMarker::Class();
        (void)OpenSslHttpsURLConnection::JxxClassInfoMarker::Class();
        (void)OpenSslKeyManagerFactorySpi::JxxClassInfoMarker::Class();
        (void)OpenSslKeyStoreKeyManager::JxxClassInfoMarker::Class();
        (void)OpenSslPropertyTrustManager::JxxClassInfoMarker::Class();
        (void)OpenSslPublicKey::JxxClassInfoMarker::Class();
        (void)OpenSslServerSocket::JxxClassInfoMarker::Class();
        (void)OpenSslServerSocketFactory::JxxClassInfoMarker::Class();
        (void)OpenSslSession::JxxClassInfoMarker::Class();
        (void)OpenSslSessionContext::JxxClassInfoMarker::Class();
        (void)OpenSslSocket::JxxClassInfoMarker::Class();
        (void)OpenSslSocketFactory::JxxClassInfoMarker::Class();
        (void)OpenSslTrustManagerFactorySpi::JxxClassInfoMarker::Class();
        (void)OpenSslX509Certificate::JxxClassInfoMarker::Class();
        (void)X509Principal::JxxClassInfoMarker::Class();
    });
}


void initialize()
{
    initializeClasses();
}

} // namespace jxx::ext::net::ssl::internal
