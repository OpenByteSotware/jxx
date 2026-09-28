#include <gtest/gtest.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslAlgorithmConstraints.h"
namespace {
TEST(AlgorithmConstraintPolicyTest, NullPolicyPreservesDefaults) {
    const std::vector<std::string> algorithms{"TLSv1.2", "TLSv1.3"};
    const auto result = ::jxx::ext::net::ssl::internal::filterAlgorithms(
        nullptr,
        ::jxx::security::CryptoPrimitive::KEY_AGREEMENT(),
        algorithms);
    EXPECT_EQ(result, algorithms);
}
}
