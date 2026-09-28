#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.DerCertificate.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace {
TEST(DerCertificateParityTest, SerializationSurfaceRemainsAvailable) {
    using Certificate = ::jxx::ext::net::ssl::internal::DerCertificate;
    EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&Certificate::writeObject)>));
    EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&Certificate::readObject)>));
    EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&Certificate::readObjectNoData)>));
}
TEST(DerCertificateParityTest, NullEncodingIsRejected) {
    EXPECT_THROW((::jxx::NEW<::jxx::ext::net::ssl::internal::DerCertificate>(nullptr)),
        ::jxx::lang::NullPointerException);
}
}
