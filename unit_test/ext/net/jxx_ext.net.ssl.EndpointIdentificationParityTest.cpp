#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocket.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
namespace {
TEST(EndpointIdentificationParityTest, SocketRetainsSSLParametersSurface) {
    using Socket = ::jxx::ext::net::ssl::internal::OpenSslSocket;
    EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&Socket::getSSLParameters)>));
    EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&Socket::setSSLParameters)>));
}
TEST(EndpointIdentificationParityTest, EmptyAlgorithmMeansNoEndpointCheck) {
    const auto parameters = ::jxx::NEW<::jxx::ext::net::ssl::SSLParameters>();
    EXPECT_EQ(parameters->getEndpointIdentificationAlgorithm(), nullptr);
}
}
