#include <gtest/gtest.h>

#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.Http11Parser.h"

namespace jxx::com::sun::net::httpserver::internal {
namespace {
Http11Parser::Result parseRequest(const std::string& request, std::string& error)
{
    Http11Parser parser;
    ParsedRequest parsed;
    std::size_t consumed = 0;
    return parser.parse(reinterpret_cast<const unsigned char*>(request.data()), request.size(), parsed, consumed, error);
}
} // namespace

TEST(RequestTargetConformanceTests, RejectsFragment)
{
    std::string error;
    EXPECT_EQ(parseRequest("GET /path#fragment HTTP/1.1\r\nHost: localhost\r\n\r\n", error), Http11Parser::Result::Error);
    EXPECT_EQ(error, "request target contains fragment");
}

TEST(RequestTargetConformanceTests, RejectsControlOrWhitespace)
{
    std::string error;
    EXPECT_EQ(parseRequest("GET /bad\tpath HTTP/1.1\r\nHost: localhost\r\n\r\n", error), Http11Parser::Result::Error);
}

TEST(RequestTargetConformanceTests, AllowsAsteriskForOptions)
{
    std::string error;
    EXPECT_EQ(parseRequest("OPTIONS * HTTP/1.1\r\nHost: localhost\r\n\r\n", error), Http11Parser::Result::Complete);
}

TEST(RequestTargetConformanceTests, RejectsAsteriskForOtherMethods)
{
    std::string error;
    EXPECT_EQ(parseRequest("GET * HTTP/1.1\r\nHost: localhost\r\n\r\n", error), Http11Parser::Result::Error);
    EXPECT_EQ(error, "asterisk target requires OPTIONS");
}

} // namespace jxx::com::sun::net::httpserver::internal
