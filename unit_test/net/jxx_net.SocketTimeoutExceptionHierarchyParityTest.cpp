#include <gtest/gtest.h>
#include <type_traits>

#include "io/jxx.io.IOException.h"
#include "io/jxx.io.InterruptedIOException.h"
#include "net/jxx.net.SocketTimeoutException.h"

namespace {

TEST(
    SocketTimeoutExceptionHierarchyParityTest,
    ExtendsInterruptedIOException)
{
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::io::InterruptedIOException,
        ::jxx::net::SocketTimeoutException>));

    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::io::IOException,
        ::jxx::net::SocketTimeoutException>));
}

TEST(
    SocketTimeoutExceptionHierarchyParityTest,
    PreservesInterruptedBytesTransferredField)
{
    ::jxx::net::SocketTimeoutException
        exception(
            "read timed out");

    exception.bytesTransferred = 7;

    EXPECT_EQ(
        exception.bytesTransferred,
        7);
}

} // namespace
