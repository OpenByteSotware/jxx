#include <gtest/gtest.h>
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
namespace {
TEST(SSLParametersJava8ParityTest, CipherSuitePreferenceRoundTrips) {
    const auto parameters =
        ::jxx::NEW<::jxx::ext::net::ssl::SSLParameters>();
    EXPECT_FALSE(parameters->getUseCipherSuitesOrder());
    parameters->setUseCipherSuitesOrder(true);
    EXPECT_TRUE(parameters->getUseCipherSuitesOrder());
}
TEST(SSLParametersJava8ParityTest, NullSniMatchersRoundTrip) {
    const auto parameters =
        ::jxx::NEW<::jxx::ext::net::ssl::SSLParameters>();
    parameters->setSNIMatchers(nullptr);
    EXPECT_EQ(parameters->getSNIMatchers(), nullptr);
}
}
