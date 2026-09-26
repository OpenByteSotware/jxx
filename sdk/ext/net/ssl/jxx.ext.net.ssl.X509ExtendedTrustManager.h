#pragma once

#include "ext/net/ssl/jxx.ext.net.ssl.X509TrustManager.h"

namespace jxx::net { class Socket; }
namespace jxx::ext::net::ssl {
class SSLEngine;
class X509ExtendedTrustManager
    : public ::jxx::lang::ClassBase<
          X509ExtendedTrustManager,
          ::jxx::lang::Object,
          X509TrustManager> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        X509ExtendedTrustManager, JxxSuper, X509TrustManager>;
    ~X509ExtendedTrustManager() override = default;
    virtual void checkClientTrusted(
        const ::jxx::Ptr<CertificateArray>& chain,
        const ::jxx::Ptr<::jxx::lang::String>& authType,
        const ::jxx::Ptr<::jxx::net::Socket>& socket) = 0;
    virtual void checkServerTrusted(
        const ::jxx::Ptr<CertificateArray>& chain,
        const ::jxx::Ptr<::jxx::lang::String>& authType,
        const ::jxx::Ptr<::jxx::net::Socket>& socket) = 0;
    virtual void checkClientTrusted(
        const ::jxx::Ptr<CertificateArray>& chain,
        const ::jxx::Ptr<::jxx::lang::String>& authType,
        const ::jxx::Ptr<SSLEngine>& engine) = 0;
    virtual void checkServerTrusted(
        const ::jxx::Ptr<CertificateArray>& chain,
        const ::jxx::Ptr<::jxx::lang::String>& authType,
        const ::jxx::Ptr<SSLEngine>& engine) = 0;
protected:
    X509ExtendedTrustManager() = default;
};
} // namespace jxx::ext::net::ssl
