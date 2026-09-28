#include <gtest/gtest.h>
#include <type_traits>
#include "security/cert/jxx.security.cert.X509Certificate.h"
#include "security/jxx.security.PublicKey.h"
namespace {
TEST(X509MetadataApiParityTest, PublicKeyIsAKeyInterface) {
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::security::Key,
        ::jxx::security::PublicKey>));
}
TEST(X509MetadataApiParityTest, MetadataMethodsAreDeclared) {
    using Certificate = ::jxx::security::cert::X509Certificate;
    EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&Certificate::getVersion)>));
    EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&Certificate::getSerialNumber)>));
    EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&Certificate::getPublicKey)>));
    EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&Certificate::getSigAlgOID)>));
}
}
