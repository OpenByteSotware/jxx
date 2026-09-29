#include <gtest/gtest.h>
#include <type_traits>

#include "io/jxx.io.IOException.h"
#include "net/jxx.net.BindException.h"
#include "net/jxx.net.ConnectException.h"
#include "net/jxx.net.SocketException.h"

namespace {

TEST(
    SocketExceptionHierarchyParityTest,
    SocketExceptionExtendsIOException)
{
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::io::IOException,
        ::jxx::net::SocketException>));
}

TEST(
    SocketExceptionHierarchyParityTest,
    DerivedNetworkExceptionsRemainSocketExceptions)
{
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::net::SocketException,
        ::jxx::net::ConnectException>));

    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::net::SocketException,
        ::jxx::net::BindException>));
}

} // namespace
