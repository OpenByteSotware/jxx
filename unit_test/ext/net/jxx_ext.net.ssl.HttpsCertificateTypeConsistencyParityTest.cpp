#include <gtest/gtest.h>
#include <type_traits>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslHttpsURLConnection.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslX509Certificate.h"
#include "security/cert/jxx.security.cert.X509Certificate.h"

namespace {

TEST(HttpsCertificateTypeConsistencyParityTest,
     ServerCertificateApiRetainsCertificateArraySurface) {
    using Connection =
        ::jxx::ext::net::ssl::internal::OpenSslHttpsURLConnection;
    EXPECT_TRUE((std::is_member_function_pointer_v<
        decltype(&Connection::getServerCertificates)>));
}

TEST(HttpsCertificateTypeConsistencyParityTest,
     ProviderCertificateIsAnX509CertificateType) {
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::security::cert::X509Certificate,
        ::jxx::ext::net::ssl::internal::OpenSslX509Certificate>));
}

} // namespace
