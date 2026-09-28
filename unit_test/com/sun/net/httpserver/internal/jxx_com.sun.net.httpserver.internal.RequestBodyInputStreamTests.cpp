#include <gtest/gtest.h>

#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.RequestBodyInputStream.h"

namespace jxx::com::sun::net::httpserver::internal {

TEST(RequestBodyInputStreamTests, TracksFullConsumption)
{
    auto body = ::jxx::NEW<::jxx::lang::ByteArrayType>(2);
    (*body)[0] = 1;
    (*body)[1] = 2;
    auto stream = ::jxx::NEW<RequestBodyInputStream>(body);
    EXPECT_FALSE(stream->isFullyConsumedInternal());
    EXPECT_EQ(stream->read(), 1);
    EXPECT_FALSE(stream->isFullyConsumedInternal());
    EXPECT_EQ(stream->read(), 2);
    EXPECT_TRUE(stream->isFullyConsumedInternal());
    EXPECT_EQ(stream->read(), -1);
}

TEST(RequestBodyInputStreamTests, CloseDoesNotPretendUnreadBytesWereConsumed)
{
    auto body = ::jxx::NEW<::jxx::lang::ByteArrayType>(1);
    auto stream = ::jxx::NEW<RequestBodyInputStream>(body);
    stream->close();
    EXPECT_TRUE(stream->wasClosedInternal());
    EXPECT_FALSE(stream->isFullyConsumedInternal());
    EXPECT_EQ(stream->read(), -1);
}

} // namespace jxx::com::sun::net::httpserver::internal
