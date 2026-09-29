#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/jxx.ext.net.ssl.KeyManagerFactory.h"
#include "ext/net/ssl/jxx.ext.net.ssl.TrustManagerFactory.h"
namespace { TEST(KeyStoreManagerFactoryParityTest, FactoryInitSurfacesRemainAvailable) { EXPECT_TRUE((std::is_class_v<::jxx::ext::net::ssl::KeyManagerFactory>)); EXPECT_TRUE((std::is_class_v<::jxx::ext::net::ssl::TrustManagerFactory>)); } }
