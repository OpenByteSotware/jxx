#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/jxx.ext.net.ssl.KeyStoreBuilderParameters.h"
namespace {
TEST(KeyStoreBuilderParametersParityTest, ImplementsManagerFactoryParameters) {
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::ext::net::ssl::ManagerFactoryParameters,
        ::jxx::ext::net::ssl::KeyStoreBuilderParameters>));
}
}
