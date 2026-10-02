#pragma once

#include <string>
#include <vector>

#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.X509TrustManager.h"

namespace jxx::ext::net::ssl::internal {

class OpenSslPropertyTrustManager final
    : public ::jxx::lang::ClassBase<
          OpenSslPropertyTrustManager,
          ::jxx::lang::Object,
          ::jxx::ext::net::ssl::X509TrustManager> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        OpenSslPropertyTrustManager,
        JxxSuper,
        ::jxx::ext::net::ssl::X509TrustManager>;

    explicit OpenSslPropertyTrustManager(
        const std::vector<
            ::jxx::lang::ByteArray>&
                trustedCertificates);

    void checkClientTrusted(
        const ::jxx::Ptr<CertificateArray>& chain,
        const ::jxx::Ptr<::jxx::lang::String>& authType) override;

    void checkServerTrusted(
        const ::jxx::Ptr<CertificateArray>& chain,
        const ::jxx::Ptr<::jxx::lang::String>& authType) override;

    ::jxx::Ptr<CertificateArray>
    getAcceptedIssuers() override;

private:
    void verify(
        const ::jxx::Ptr<CertificateArray>& chain,
        const ::jxx::Ptr<::jxx::lang::String>& authType) const;

    std::vector<::jxx::lang::ByteArray>
        trustedCertificates_;
};

std::string discoverDefaultTrustStorePath(
    const std::string& runtimeHome);

::jxx::Ptr<::jxx::ext::net::ssl::SSLContext::TrustManagerArray>
loadDefaultPropertyTrustManagers();

} // namespace jxx::ext::net::ssl::internal
