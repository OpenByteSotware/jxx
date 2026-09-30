#include <gtest/gtest.h>

#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpsConfigurator.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpsServer.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InetSocketAddress.h"

namespace jxx::com::sun::net::httpserver {
namespace {

class HttpsServerBehaviorTest : public ::testing::Test {
protected:
    void TearDown() override
    {
        if (server_ != nullptr) {
            try {
                server_->stop(0);
            }
            catch (...) {
            }
        }
    }

    static ::jxx::Ptr<HttpsConfigurator> newConfigurator()
    {
        const auto protocol = ::jxx::NEW<::jxx::lang::String>("TLS");
        const auto context =
            ::jxx::ext::net::ssl::SSLContext::getInstance(protocol);
        return ::jxx::NEW<HttpsConfigurator>(context);
    }

    ::jxx::Ptr<HttpsServer> server_;
};

TEST_F(HttpsServerBehaviorTest, CreateReturnsUnboundServerWithoutConfigurator)
{
    server_ = HttpsServer::create();

    ASSERT_NE(nullptr, server_);
    EXPECT_EQ(nullptr, server_->getAddress());
    EXPECT_EQ(nullptr, server_->getHttpsConfigurator());
}

TEST_F(HttpsServerBehaviorTest, ConfiguratorRoundTripsByIdentity)
{
    server_ = HttpsServer::create();
    const auto configurator = newConfigurator();

    server_->setHttpsConfigurator(configurator);

    EXPECT_EQ(configurator.get(), server_->getHttpsConfigurator().get());
}

TEST_F(HttpsServerBehaviorTest, NullConfiguratorIsRejected)
{
    server_ = HttpsServer::create();
    const ::jxx::Ptr<HttpsConfigurator> nullConfigurator;

    EXPECT_THROW(server_->setHttpsConfigurator(nullConfigurator),
                 ::jxx::lang::NullPointerException);
}

TEST_F(HttpsServerBehaviorTest, StartRequiresBoundAddress)
{
    server_ = HttpsServer::create();
    server_->setHttpsConfigurator(newConfigurator());

    EXPECT_THROW(server_->start(), ::jxx::lang::IllegalStateException);
}

TEST_F(HttpsServerBehaviorTest, StartRequiresHttpsConfigurator)
{
    server_ = HttpsServer::create();
    server_->bind(::jxx::NEW<::jxx::net::InetSocketAddress>(0), 0);

    EXPECT_THROW(server_->start(), ::jxx::lang::IllegalStateException);
}

TEST_F(HttpsServerBehaviorTest, BoundFactoryPublishesAddress)
{
    const auto address = ::jxx::NEW<::jxx::net::InetSocketAddress>(0);
    server_ = HttpsServer::create(address, 0);

    ASSERT_NE(nullptr, server_);
    EXPECT_EQ(address.get(), server_->getAddress().get());
}

TEST_F(HttpsServerBehaviorTest, ConfiguratorCannotChangeAfterStop)
{
    server_ = HttpsServer::create();
    server_->stop(0);

    EXPECT_THROW(server_->setHttpsConfigurator(newConfigurator()),
                 ::jxx::lang::IllegalStateException);
    server_.reset();
}

TEST_F(HttpsServerBehaviorTest, StopRejectsNegativeDelay)
{
    server_ = HttpsServer::create();

    EXPECT_THROW(server_->stop(-1), ::jxx::lang::IllegalArgumentException);
    server_.reset();
}

} // namespace
} // namespace jxx::com::sun::net::httpserver
