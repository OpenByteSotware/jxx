#include <gtest/gtest.h>
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLServerSocket.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace {
TEST(SSLServerSocketParametersParityTest, NullParametersAreRejectedByContract) {
    EXPECT_TRUE((std::is_member_function_pointer_v<
        decltype(&::jxx::ext::net::ssl::SSLServerSocket::setSSLParameters)>));
}
TEST(SSLServerSocketParametersParityTest, ParametersExposeConstraints) {
    const auto parameters = ::jxx::NEW<::jxx::ext::net::ssl::SSLParameters>();
    EXPECT_EQ(parameters->getAlgorithmConstraints(), nullptr);
}
}
