#include <gtest/gtest.h>
#include <type_traits>
#include "net/jxx.net.SocketTimeoutException.h"
namespace {
TEST(SocketReadTimeoutParityTest, TimeoutExceptionIsCatchableAsItsExactType) {
    EXPECT_TRUE((std::is_base_of_v<
        std::runtime_error,
        ::jxx::net::SocketTimeoutException>));
    EXPECT_THROW(
        throw ::jxx::net::SocketTimeoutException("socket read timed out"),
        ::jxx::net::SocketTimeoutException);
}
}
