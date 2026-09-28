#include <gtest/gtest.h>

#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.BufferedRequestBodySource.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.RequestBodyInputStream.h"

namespace jxx::com::sun::net::httpserver::internal {

TEST(RequestBodySourceTests, ReadsOnlyConfiguredSlice)
{
    auto bytes = ::jxx::NEW<::jxx::lang::ByteArrayType>(4);
    (*bytes)[0] = 9;
    (*bytes)[1] = 1;
    (*bytes)[2] = 2;
    (*bytes)[3] = 8;
    auto source = ::jxx::NEW<BufferedRequestBodySource>(bytes, 1, 2);
    auto stream = ::jxx::NEW<RequestBodyInputStream>(::jxx::CAST<RequestBodySource>(source));
    EXPECT_EQ(stream->read(), 1);
    EXPECT_EQ(stream->read(), 2);
    EXPECT_EQ(stream->read(), -1);
    EXPECT_TRUE(stream->isFullyConsumedInternal());
}

TEST(RequestBodySourceTests, ClosePreservesUnreadState)
{
    auto bytes = ::jxx::NEW<::jxx::lang::ByteArrayType>(2);
    auto source = ::jxx::NEW<BufferedRequestBodySource>(bytes, 0, 2);
    auto stream = ::jxx::NEW<RequestBodyInputStream>(::jxx::CAST<RequestBodySource>(source));
    stream->close();
    EXPECT_TRUE(stream->wasClosedInternal());
    EXPECT_FALSE(stream->isFullyConsumedInternal());
}

} // namespace jxx::com::sun::net::httpserver::internal
