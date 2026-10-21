#include "ext/net/ssl/jxx.ext.net.ssl.ClassInitialization.h"
#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.SerializationClassRegistry.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "ext/net/ssl/jxx.ext.net.ssl.CertPathTrustManagerParameters.h"
#include "ext/net/ssl/jxx.ext.net.ssl.ExtendedSSLSession.h"
#include "ext/net/ssl/jxx.ext.net.ssl.HandshakeCompletedEvent.h"
#include "ext/net/ssl/jxx.ext.net.ssl.HandshakeCompletedListener.h"
#include "ext/net/ssl/jxx.ext.net.ssl.HostnameVerifier.h"
#include "ext/net/ssl/jxx.ext.net.ssl.HttpsURLConnection.h"
#include "ext/net/ssl/jxx.ext.net.ssl.KeyManagerFactory.h"
#include "ext/net/ssl/jxx.ext.net.ssl.KeyManagerFactorySpi.h"
#include "ext/net/ssl/jxx.ext.net.ssl.KeyStoreBuilderParameters.h"
#include "ext/net/ssl/jxx.ext.net.ssl.ManagerFactoryParameters.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SNIMatcher.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContextSpi.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLEngine.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLEngineResult.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLException.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLHandshakeException.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLKeyException.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLPeerUnverifiedException.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLPermission.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLProtocolException.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLServerSocket.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLServerSocketFactory.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSession.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSessionContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocket.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocketFactory.h"
#include "ext/net/ssl/jxx.ext.net.ssl.StandardConstants.h"
#include "ext/net/ssl/jxx.ext.net.ssl.TrustManagerFactory.h"
#include "ext/net/ssl/jxx.ext.net.ssl.TrustManagerFactorySpi.h"
#include "ext/net/ssl/jxx.ext.net.ssl.X509ExtendedKeyManager.h"
#include "ext/net/ssl/jxx.ext.net.ssl.X509ExtendedTrustManager.h"
#include "ext/net/ssl/jxx.ext.net.ssl.X509KeyManager.h"
#include "ext/net/ssl/jxx.ext.net.ssl.X509TrustManager.h"
#include "ext/net/ssl/jxx.ext.net.ssl.KeyManager.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SNIHostName.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SNIServerName.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSessionBindingEvent.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSessionBindingListener.h"
#include "ext/net/ssl/jxx.ext.net.ssl.TrustManager.h"

namespace jxx::ext::net::ssl {
namespace {

template <typename T>
void registerType()
{
    ::jxx::lang::class_info_detail::registerClassInfo<T>();
}

} // namespace

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        registerType<CertPathTrustManagerParameters>();
        registerType<ExtendedSSLSession>();
        registerType<HandshakeCompletedEvent>();
        ::jxx::io::serialization::registerSerializableClass<HandshakeCompletedEvent>(
            1,
            [] { return HandshakeCompletedEvent::createForDeserialization(); });
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
            HandshakeCompletedEvent, 1U, 2U>();
        registerType<HandshakeCompletedListener>();
        registerType<HostnameVerifier>();
        registerType<HttpsURLConnection>();
        registerType<KeyManagerFactory>();
        registerType<KeyManagerFactorySpi>();
        registerType<KeyStoreBuilderParameters>();
        registerType<ManagerFactoryParameters>();
        registerType<SNIMatcher>();
        registerType<SSLContext>();
        registerType<SSLContextSpi>();
        registerType<SSLEngine>();
        registerType<SSLEngineResult>();
        registerType<SSLException>();
        registerType<SSLHandshakeException>();
        registerType<SSLKeyException>();
        registerType<SSLParameters>();
        registerType<SSLPeerUnverifiedException>();
        registerType<SSLPermission>();
        registerType<SSLProtocolException>();
        registerType<SSLServerSocket>();
        registerType<SSLServerSocketFactory>();
        registerType<SSLSession>();
        registerType<SSLSessionContext>();
        registerType<SSLSocket>();
        registerType<SSLSocketFactory>();
        registerType<StandardConstants>();
        registerType<TrustManagerFactory>();
        registerType<TrustManagerFactorySpi>();
        registerType<X509ExtendedKeyManager>();
        registerType<X509ExtendedTrustManager>();
        registerType<X509KeyManager>();
        registerType<X509TrustManager>();
        registerType<KeyManager>();
        registerType<SNIHostName>();
        registerType<SNIServerName>();
        registerType<SSLSessionBindingEvent>();
        ::jxx::io::serialization::registerSerializableClass<SSLSessionBindingEvent>(
            static_cast<::jxx::lang::jlong>(3989172637106345LL),
            [] { return SSLSessionBindingEvent::createForDeserialization(); });
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
            SSLSessionBindingEvent, 1U, 2U>();
        registerType<SSLSessionBindingListener>();
        registerType<TrustManager>();
    });
}


void initialize()
{
    initializeClasses();
}

} // namespace jxx::ext::net::ssl
