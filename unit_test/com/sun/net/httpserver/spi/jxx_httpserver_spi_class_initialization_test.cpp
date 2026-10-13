#include <gtest/gtest.h>

#include "com/sun/net/httpserver/spi/jxx.com.sun.net.httpserver.spi.ClassInitialization.h"
#include "com/sun/net/httpserver/spi/jxx.com.sun.net.httpserver.spi.HttpServerProvider.h"
#include "jxx.initialize.h"

namespace {

auto httpServerProviderClass()
{
    return ::jxx::lang::ClassBase<
        ::jxx::com::sun::net::httpserver::spi::HttpServerProvider,
        ::jxx::lang::Object>::Class();
}

} // namespace

TEST(JxxHttpServerSpiClassInitialization, HttpServerProviderHasClassMetadata)
{
    ::jxx::initialize();

    EXPECT_NE(httpServerProviderClass(), nullptr);
}

TEST(JxxHttpServerSpiClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto before = httpServerProviderClass();

    EXPECT_NO_THROW(
        ::jxx::com::sun::net::httpserver::spi::initializeClasses());
    EXPECT_NO_THROW(
        ::jxx::com::sun::net::httpserver::spi::initializeClasses());

    EXPECT_EQ(before, httpServerProviderClass());
}
