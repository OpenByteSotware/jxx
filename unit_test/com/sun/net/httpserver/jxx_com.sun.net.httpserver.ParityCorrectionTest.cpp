#include <gtest/gtest.h>

#include <type_traits>

#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.HttpServerLimits.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.Headers.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpHandler.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpServer.h"
#include "lang/jxx.lang.Exceptions.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "util/jxx.util.HashMap.h"
#include "util/jxx.util.Map.h"

namespace jxx::com::sun::net::httpserver {
namespace {

TEST(HttpServerParityCorrectionTest, HeadersImplementsMapWithoutExtendingHashMap)
{
    using ValueList = ::jxx::util::List<::jxx::lang::String>;
    using MapType = ::jxx::util::Map<::jxx::lang::String, ValueList>;
    using HashMapType = ::jxx::util::HashMap<::jxx::lang::String, ValueList>;

    EXPECT_TRUE((std::is_base_of_v<MapType, Headers>));
    EXPECT_FALSE((std::is_base_of_v<HashMapType, Headers>));
}

TEST(HttpServerParityCorrectionTest, NullContextPathThrowsNullPointerException)
{
    const auto server = HttpServer::create();
    const ::jxx::Ptr<::jxx::lang::String> nullPath;

    EXPECT_THROW(server->createContext(nullPath),
                 ::jxx::lang::NullPointerException);
    EXPECT_THROW(server->removeContext(nullPath),
                 ::jxx::lang::NullPointerException);
    server->stop(0);
}

TEST(HttpServerParityCorrectionTest, PortZeroReportsActualListeningPort)
{
    const auto server = HttpServer::create();
    server->bind(::jxx::NEW<::jxx::net::InetSocketAddress>(0), 0);

    const auto actual = server->getAddress();
    ASSERT_NE(nullptr, actual);
    EXPECT_GT(actual->getPort(), 0);
    server->stop(0);
}

TEST(HttpServerParityCorrectionTest, DefaultLimitsRemainAvailable)
{
    const internal::HttpServerLimits limits;

    EXPECT_GT(limits.maxRequestLineBytes, 0U);
    EXPECT_GT(limits.maxHeaderBytes, 0U);
    EXPECT_GT(limits.maxDecodedBodyBytes, 0U);
}

} // namespace
} // namespace jxx::com::sun::net::httpserver
