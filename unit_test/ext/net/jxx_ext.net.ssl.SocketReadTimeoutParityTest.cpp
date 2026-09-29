#include <gtest/gtest.h>
#include <type_traits>
#include "net/jxx.net.SocketTimeoutException.h"
namespace { TEST(SocketReadTimeoutParityTest, IsIOException) { EXPECT_TRUE((std::is_base_of_v<::jxx::io::IOException,::jxx::net::SocketTimeoutException>)); } }
