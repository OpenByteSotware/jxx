#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslStreams.h"
namespace {
TEST(SocketStreamBoundsParityTest, StreamTypesRemainConcrete) {
 EXPECT_TRUE((std::is_base_of_v<::jxx::io::InputStream,::jxx::ext::net::ssl::internal::OpenSslInputStream>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::io::OutputStream,::jxx::ext::net::ssl::internal::OpenSslOutputStream>));
}
}
