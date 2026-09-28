#include <gtest/gtest.h>

#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.FixedLengthRequestBodySource.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.IOException.h"

namespace jxx::com::sun::net::httpserver::internal {

TEST(FixedLengthRequestBodySourceTests, ReadsPrefixThenSocketInput)
{
    auto prefix = ::jxx::NEW<::jxx::lang::ByteArrayType>(2);
    (*prefix)[0] = 1; (*prefix)[1] = 2;
    auto tail = ::jxx::NEW<::jxx::lang::ByteArrayType>(2);
    (*tail)[0] = 3; (*tail)[1] = 4;
    auto input = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(tail);
    auto source = ::jxx::NEW<FixedLengthRequestBodySource>(input, prefix, 0, 2, 4);
    auto output = ::jxx::NEW<::jxx::lang::ByteArrayType>(4);
    EXPECT_EQ(source->read(output, 0, 4), 4);
    EXPECT_EQ((*output)[0], 1); EXPECT_EQ((*output)[1], 2);
    EXPECT_EQ((*output)[2], 3); EXPECT_EQ((*output)[3], 4);
    EXPECT_TRUE(source->isFullyConsumedInternal());
}

TEST(FixedLengthRequestBodySourceTests, DetectsPrematureEnd)
{
    auto prefix = ::jxx::NEW<::jxx::lang::ByteArrayType>(0);
    auto tail = ::jxx::NEW<::jxx::lang::ByteArrayType>(1);
    auto input = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(tail);
    auto source = ::jxx::NEW<FixedLengthRequestBodySource>(input, prefix, 0, 0, 2);
    auto output = ::jxx::NEW<::jxx::lang::ByteArrayType>(2);
    EXPECT_EQ(source->read(output, 0, 2), 1);
    EXPECT_THROW(source->read(output, 1, 1), ::jxx::io::IOException);
}

} // namespace jxx::com::sun::net::httpserver::internal
