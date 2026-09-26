#include <gtest/gtest.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.X509Principal.h"
#include "io/jxx.io.SerializableI.h"
#include "ext/net/ssl/jxx.ext.net.ssl.HandshakeCompletedEvent.h"

TEST(SSLParityStageTest, X509PrincipalUsesDistinguishedName) {
    const auto name = ::jxx::NEW<::jxx::lang::String>("/CN=localhost");
    const auto principal = ::jxx::NEW<
        ::jxx::ext::net::ssl::internal::X509Principal>(name);
    EXPECT_EQ(principal->getName()->utf8(), "/CN=localhost");
}

TEST(SSLParityStageTest, HandshakeEventImplementsSerializableHooks) {
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::io::SerializableI,
        ::jxx::ext::net::ssl::HandshakeCompletedEvent>));
}
