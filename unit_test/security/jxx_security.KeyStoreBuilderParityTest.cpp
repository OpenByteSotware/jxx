#include <gtest/gtest.h>
#include <type_traits>
#include "security/jxx.security.KeyStore.h"
namespace {
TEST(KeyStoreBuilderParityTest, BuilderIsAKeyStoreNestedClass) {
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::lang::Object,
        ::jxx::security::KeyStore::Builder>));
}
}
