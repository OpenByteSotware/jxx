#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslHttpsURLConnection.h"
namespace { TEST(HttpsLocalCertificateParityTest, LocalCertificateApiIsImplemented) { using C=::jxx::ext::net::ssl::internal::OpenSslHttpsURLConnection; EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&C::getLocalCertificates)>)); } }
