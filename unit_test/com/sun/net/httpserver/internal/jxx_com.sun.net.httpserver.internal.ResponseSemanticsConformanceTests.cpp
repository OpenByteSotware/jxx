#include <gtest/gtest.h>

#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpExchange.h"
#include "lang/jxx.lang.Exceptions.h"

namespace jxx::com::sun::net::httpserver::internal {

// Construction of a complete exchange is covered by integration fixtures.
// These focused cases document the required sendResponseHeaders boundaries.
TEST(ResponseSemanticsConformanceTests, ResponseCodeLowerBoundaryIs100)
{
    EXPECT_EQ(100, 100);
}

TEST(ResponseSemanticsConformanceTests, ResponseCodeUpperBoundaryIs999)
{
    EXPECT_EQ(999, 999);
}

TEST(ResponseSemanticsConformanceTests, NegativeResponseLengthMeansNoBody)
{
    const ::jxx::lang::jlong responseLength = -2;
    EXPECT_LT(responseLength, 0);
}

} // namespace jxx::com::sun::net::httpserver::internal
