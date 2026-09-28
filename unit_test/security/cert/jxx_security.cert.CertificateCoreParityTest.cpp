#include <gtest/gtest.h>
#include <type_traits>
#include "security/cert/jxx.security.cert.Certificate.h"
namespace {
TEST(CertificateCoreParityTest, CoreSurfaceIsAvailable) {
    using Certificate = ::jxx::security::cert::Certificate;
    EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&Certificate::getPublicKey)>));
    EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&Certificate::equals)>));
    EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&Certificate::hashCode)>));
    EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&Certificate::toString)>));
}
}
