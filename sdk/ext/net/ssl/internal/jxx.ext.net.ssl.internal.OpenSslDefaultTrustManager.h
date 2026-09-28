#pragma once

#include "ext/net/ssl/jxx.ext.net.ssl.X509TrustManager.h"

namespace jxx::ext::net::ssl::internal {
class OpenSslDefaultTrustManager final
    : public ::jxx::lang::ClassBase<OpenSslDefaultTrustManager,
          ::jxx::lang::Object, ::jxx::ext::net::ssl::X509TrustManager> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<OpenSslDefaultTrustManager,
        JxxSuper, ::jxx::ext::net::ssl::X509TrustManager>;
    void checkClientTrusted(const ::jxx::Ptr<CertificateArray>& chain,
        const ::jxx::Ptr<::jxx::lang::String>& authType) override;
    void checkServerTrusted(const ::jxx::Ptr<CertificateArray>& chain,
        const ::jxx::Ptr<::jxx::lang::String>& authType) override;
    ::jxx::Ptr<CertificateArray> getAcceptedIssuers() override;
private:
    static void verify(const ::jxx::Ptr<CertificateArray>& chain,
        const ::jxx::Ptr<::jxx::lang::String>& authType);
};
} // namespace jxx::ext::net::ssl::internal
