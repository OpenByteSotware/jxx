#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslX509Certificate.h"
namespace {
TEST(X509CertificatePublicParityTest, ExtensionAndSerializationSurfaceIsImplemented) {
    using Certificate = ::jxx::ext::net::ssl::internal::OpenSslX509Certificate;
    EXPECT_TRUE((std::is_member_function_pointer_v<
        decltype(&Certificate::hasUnsupportedCriticalExtension)>));
    EXPECT_TRUE((std::is_member_function_pointer_v<
        decltype(&Certificate::writeObject)>));
    EXPECT_TRUE((std::is_member_function_pointer_v<
        decltype(&Certificate::readObject)>));
}
}
