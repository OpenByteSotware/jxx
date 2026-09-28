#include <gtest/gtest.h>

#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.Http11Parser.h"

namespace jxx::com::sun::net::httpserver::internal {

TEST(ServerProtocolConformanceTests, ParsesHttp10ProtocolForProtocolAwareErrors)
{
    const std::string request = "GET /missing HTTP/1.0\r\n\r\n";
    Http11Parser parser;
    ParsedRequest parsed;
    std::size_t consumed = 0;
    std::string error;
    EXPECT_EQ(parser.parse(reinterpret_cast<const unsigned char*>(request.data()), request.size(), parsed, consumed, error), Http11Parser::Result::Complete);
    EXPECT_EQ(parsed.version, "HTTP/1.0");
}

TEST(ServerProtocolConformanceTests, ParsesHttp11ProtocolForProtocolAwareErrors)
{
    const std::string request = "GET /missing HTTP/1.1\r\nHost: localhost\r\n\r\n";
    Http11Parser parser;
    ParsedRequest parsed;
    std::size_t consumed = 0;
    std::string error;
    EXPECT_EQ(parser.parse(reinterpret_cast<const unsigned char*>(request.data()), request.size(), parsed, consumed, error), Http11Parser::Result::Complete);
    EXPECT_EQ(parsed.version, "HTTP/1.1");
}

} // namespace jxx::com::sun::net::httpserver::internal
