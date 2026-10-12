#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.Authenticator.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.BasicAuthenticator.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.Headers.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpExchange.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpHandler.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpPrincipal.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpServer.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpsConfigurator.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpsExchange.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpsParameters.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpsServer.h"

TEST(JxxHttpServerClassInitialization, PublicTypesHaveClassMetadata)
{
    ::jxx::initialize();
    EXPECT_NE(::jxx::com::sun::net::httpserver::Authenticator::Class(), nullptr);
    EXPECT_NE(::jxx::com::sun::net::httpserver::BasicAuthenticator::Class(), nullptr);
    EXPECT_NE(::jxx::com::sun::net::httpserver::Headers::Class(), nullptr);
    EXPECT_NE(::jxx::com::sun::net::httpserver::HttpExchange::Class(), nullptr);
    EXPECT_NE(::jxx::com::sun::net::httpserver::HttpHandler::Class(), nullptr);
    EXPECT_NE(::jxx::com::sun::net::httpserver::HttpPrincipal::Class(), nullptr);
    EXPECT_NE(::jxx::com::sun::net::httpserver::HttpServer::Class(), nullptr);
    EXPECT_NE(::jxx::com::sun::net::httpserver::HttpsConfigurator::Class(), nullptr);
    EXPECT_NE(::jxx::com::sun::net::httpserver::HttpsExchange::Class(), nullptr);
    EXPECT_NE(::jxx::com::sun::net::httpserver::HttpsParameters::Class(), nullptr);
    EXPECT_NE(::jxx::com::sun::net::httpserver::HttpsServer::Class(), nullptr);
}
