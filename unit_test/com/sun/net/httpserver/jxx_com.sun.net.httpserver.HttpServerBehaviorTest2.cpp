#include <gtest/gtest.h>

#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpContext.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpExchange.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpHandler.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpServer.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InetSocketAddress.h"

namespace jxx::com::sun::net::httpserver {
namespace {

class NoOpHandler final : public HttpHandler {
public:
    void handle(const ::jxx::Ptr<HttpExchange>& exchange) override
    {
        if (exchange != nullptr) {
            exchange->sendResponseHeaders(204, -1);
            exchange->close();
        }
    }
};

// This file intentionally uses a unique suite name. Another translation unit
// already defines HttpServerBehaviorTest with a different fixture type.
class HttpServerBehaviorExtendedTest : public ::testing::Test {
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

    ::jxx::Ptr<HttpServer> server_;
};

TEST_F(HttpServerBehaviorExtendedTest, CreateReturnsUnboundServer)
{
    server_ = HttpServer::create();

    ASSERT_NE(nullptr, server_);
    EXPECT_EQ(nullptr, server_->getAddress());
    EXPECT_EQ(nullptr, server_->getExecutor());
}

TEST_F(HttpServerBehaviorExtendedTest, BindPublishesConfiguredAddress)
{
    server_ = HttpServer::create();
    const auto requested = ::jxx::NEW<::jxx::net::InetSocketAddress>(0);

    server_->bind(requested, 0);

    const auto bound = server_->getAddress();
    ASSERT_NE(nullptr, bound);
    EXPECT_EQ(requested->getAddress(), bound->getAddress());
    EXPECT_EQ(0, requested->getPort());
    EXPECT_GT(bound->getPort(), 0);
}

TEST_F(HttpServerBehaviorExtendedTest, BindRejectsNullAndSecondBind)
{
    server_ = HttpServer::create();
    const ::jxx::Ptr<::jxx::net::InetSocketAddress> nullAddress;

    EXPECT_THROW(server_->bind(nullAddress, 0),
                 ::jxx::lang::NullPointerException);

    const auto address = ::jxx::NEW<::jxx::net::InetSocketAddress>(0);
    server_->bind(address, 0);

    EXPECT_THROW(server_->bind(address, 0),
                 ::jxx::lang::IllegalStateException);
}

TEST_F(HttpServerBehaviorExtendedTest, StartRequiresBoundServer)
{
    server_ = HttpServer::create();

    EXPECT_THROW(server_->start(), ::jxx::lang::IllegalStateException);
}

TEST_F(HttpServerBehaviorExtendedTest, CreateContextExposesHandlerPathAndServer)
{
    server_ = HttpServer::create();
    const auto path = ::jxx::NEW<::jxx::lang::String>("/service");
    const auto handler = ::jxx::NEW<NoOpHandler>();

    const auto context = server_->createContext(path, handler);

    ASSERT_NE(nullptr, context);
    EXPECT_EQ(handler.get(), context->getHandler().get());
    EXPECT_TRUE(path->equals(context->getPath()));
    EXPECT_EQ(server_.get(), context->getServer().get());
    ASSERT_NE(nullptr, context->getAttributes());
    ASSERT_NE(nullptr, context->getFilters());
}

TEST_F(HttpServerBehaviorExtendedTest, DuplicateContextPathIsRejected)
{
    server_ = HttpServer::create();
    const auto path = ::jxx::NEW<::jxx::lang::String>("/duplicate");
    const auto handler = ::jxx::NEW<NoOpHandler>();
    server_->createContext(path, handler);

    EXPECT_THROW(server_->createContext(path, handler),
                 ::jxx::lang::IllegalArgumentException);
}

TEST_F(HttpServerBehaviorExtendedTest, ContextCanBeRemovedByObjectAndPath)
{
    server_ = HttpServer::create();
    const auto handler = ::jxx::NEW<NoOpHandler>();
    const auto firstPath = ::jxx::NEW<::jxx::lang::String>("/first");
    const auto secondPath = ::jxx::NEW<::jxx::lang::String>("/second");
    const auto first = server_->createContext(firstPath, handler);
    server_->createContext(secondPath, handler);

    EXPECT_NO_THROW(server_->removeContext(first));
    EXPECT_NO_THROW(server_->removeContext(secondPath));
    EXPECT_THROW(server_->removeContext(firstPath),
                 ::jxx::lang::IllegalArgumentException);
}

TEST_F(HttpServerBehaviorExtendedTest, ContextRejectsNullPathAndHandler)
{
    server_ = HttpServer::create();
    const auto path = ::jxx::NEW<::jxx::lang::String>("/service");
    const auto handler = ::jxx::NEW<NoOpHandler>();
    const ::jxx::Ptr<::jxx::lang::String> nullPath;
    const ::jxx::Ptr<HttpHandler> nullHandler;

    EXPECT_THROW(server_->createContext(nullPath, handler),
                 ::jxx::lang::NullPointerException);
    EXPECT_THROW(server_->createContext(path, nullHandler),
                 ::jxx::lang::NullPointerException);
}

TEST_F(HttpServerBehaviorExtendedTest, StopRejectsNegativeDelay)
{
    server_ = HttpServer::create();

    EXPECT_THROW(server_->stop(-1), ::jxx::lang::IllegalArgumentException);
    server_.reset();
}

} // namespace
} // namespace jxx::com::sun::net::httpserver
