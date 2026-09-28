#include <gtest/gtest.h>

#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.Http11Parser.h"

namespace jxx::com::sun::net::httpserver::internal {

TEST(RequestHeadParserTests, CompletesBeforeFixedLengthBodyArrives)
{
    const std::string request =
        "POST /upload HTTP/1.1\r\nHost: localhost\r\nContent-Length: 4\r\n\r\n";
    Http11Parser parser;
    ParsedRequest parsed;
    std::size_t consumed = 0;
    std::string error;
    EXPECT_EQ(parser.parseHead(reinterpret_cast<const unsigned char*>(request.data()), request.size(), parsed, consumed, error), Http11Parser::Result::Complete);
    EXPECT_TRUE(parsed.contentLengthPresent);
    EXPECT_EQ(parsed.contentLength, 4U);
    EXPECT_FALSE(parsed.chunked);
    EXPECT_EQ(consumed, request.size());
    EXPECT_TRUE(parsed.body.empty());
}

TEST(RequestHeadParserTests, ReportsChunkedFramingWithoutWaitingForChunks)
{
    const std::string request =
        "POST /upload HTTP/1.1\r\nHost: localhost\r\nTransfer-Encoding: chunked\r\n\r\n";
    Http11Parser parser;
    ParsedRequest parsed;
    std::size_t consumed = 0;
    std::string error;
    EXPECT_EQ(parser.parseHead(reinterpret_cast<const unsigned char*>(request.data()), request.size(), parsed, consumed, error), Http11Parser::Result::Complete);
    EXPECT_TRUE(parsed.chunked);
    EXPECT_FALSE(parsed.contentLengthPresent);
    EXPECT_TRUE(parsed.body.empty());
}

TEST(RequestHeadParserTests, RetainsBodyPrefixOutsideConsumedHead)
{
    const std::string head =
        "POST /upload HTTP/1.1\r\nHost: localhost\r\nContent-Length: 4\r\n\r\n";
    const std::string request = head + "AB";
    Http11Parser parser;
    ParsedRequest parsed;
    std::size_t consumed = 0;
    std::string error;
    EXPECT_EQ(parser.parseHead(reinterpret_cast<const unsigned char*>(request.data()), request.size(), parsed, consumed, error), Http11Parser::Result::Complete);
    EXPECT_EQ(consumed, head.size());
    EXPECT_EQ(request.size() - consumed, 2U);
}

} // namespace jxx::com::sun::net::httpserver::internal
