#pragma once

#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::ext::net::ssl {

class SSLContextSpi
    : public ::jxx::lang::ClassBase<
          SSLContextSpi,
          ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<SSLContextSpi, JxxSuper>;
    using KeyManagerArray = SSLContext::KeyManagerArray;
    using TrustManagerArray = SSLContext::TrustManagerArray;

    ~SSLContextSpi() override = default;

    virtual void engineInit(
        const ::jxx::Ptr<KeyManagerArray>& keyManagers,
        const ::jxx::Ptr<TrustManagerArray>& trustManagers,
        const ::jxx::Ptr<::jxx::security::SecureRandom>& secureRandom) = 0;

    virtual ::jxx::Ptr<SSLSocketFactory>
    engineGetSocketFactory() = 0;
    virtual ::jxx::Ptr<SSLServerSocketFactory>
    engineGetServerSocketFactory() = 0;
    virtual ::jxx::Ptr<SSLSessionContext>
    engineGetClientSessionContext() = 0;
    virtual ::jxx::Ptr<SSLSessionContext>
    engineGetServerSessionContext() = 0;
    virtual ::jxx::Ptr<SSLEngine>
    engineCreateSSLEngine() = 0;
    virtual ::jxx::Ptr<SSLEngine>
    engineCreateSSLEngine(
        const ::jxx::Ptr<::jxx::lang::String>& peerHost,
        ::jxx::lang::jint peerPort) = 0;

    virtual ::jxx::Ptr<SSLParameters>
    engineGetDefaultSSLParameters();
    virtual ::jxx::Ptr<SSLParameters>
    engineGetSupportedSSLParameters();

protected:
    SSLContextSpi() = default;
};

} // namespace jxx::ext::net::ssl
