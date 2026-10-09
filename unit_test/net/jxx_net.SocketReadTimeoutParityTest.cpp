#include <gtest/gtest.h>
#include <type_traits>

#include "io/jxx.io.IOException.h"
#include "io/jxx.io.InterruptedIOException.h"
#include "net/jxx.net.SocketTimeoutException.h"

namespace
{

    TEST(
        SocketReadTimeoutParityTest,
        TimeoutExceptionHasCorrectHierarchyAndExactType)
    {
        using ::jxx::io::IOException;
        using ::jxx::io::InterruptedIOException;
        using ::jxx::net::SocketTimeoutException;

        EXPECT_TRUE((
            std::is_base_of_v<
            InterruptedIOException,
            SocketTimeoutException>));

        EXPECT_TRUE((
            std::is_base_of_v<
            IOException,
            SocketTimeoutException>));

        EXPECT_FALSE((
            std::is_base_of_v<
            std::runtime_error,
            SocketTimeoutException>));

        EXPECT_THROW(
            throw SocketTimeoutException(
                "socket read timed out"),
            SocketTimeoutException);
    }

} // namespace