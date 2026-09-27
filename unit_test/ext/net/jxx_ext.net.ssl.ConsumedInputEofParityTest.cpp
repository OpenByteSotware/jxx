#include <gtest/gtest.h>

#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocketFactory.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "lang/jxx.lang.buildin_array.h"

namespace {

TEST(ConsumedInputEofParityTest, ByteArrayInputStreamReachesEof) {
    const auto bytes = ::jxx::NEW<
        ::jxx::lang::JxxArray<
            ::jxx::lang::jbyte,
            1U>>(3);

    (*bytes)[0] = 1;
    (*bytes)[1] = 2;
    (*bytes)[2] = 3;

    const auto consumed =
        ::jxx::NEW<
            ::jxx::io::ByteArrayInputStream>(
                bytes);

    EXPECT_EQ(consumed->read(), 1);
    EXPECT_EQ(consumed->read(), 2);
    EXPECT_EQ(consumed->read(), 3);
    EXPECT_EQ(consumed->read(), -1);
}

} // namespace
