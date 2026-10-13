#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.ClassInitialization.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.BufferedRequestBodySource.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.ChunkedRequestBodySource.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpContext.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpExchange.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpServer.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpServerProvider.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpsExchange.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpsParameters.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpsServer.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.FixedLengthRequestBodySource.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.RequestBodyInputStream.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.RequestBodySource.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.ResponseBodyOutputStream.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.ServerConnectionTask.h"

namespace T = ::jxx::com::sun::net::httpserver::internal;

TEST(JxxStage54ClassInitialization, MetadataAndIdempotence)
{
    ::jxx::initialize();
    using namespace ::jxx::com::sun::net::httpserver::internal;
    EXPECT_NE(BufferedRequestBodySource::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(ChunkedRequestBodySource::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(DefaultHttpContext::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(DefaultHttpExchange::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(DefaultHttpServer::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(DefaultHttpServerProvider::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(DefaultHttpsExchange::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(DefaultHttpsParameters::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(DefaultHttpsServer::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(FixedLengthRequestBodySource::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(RequestBodyInputStream::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(RequestBodySource::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(ResponseBodyOutputStream::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(ServerConnectionTask::JxxClassInfoMarker::Class(), nullptr);
    const auto before = BufferedRequestBodySource::JxxClassInfoMarker::Class();
    EXPECT_NO_THROW(T::initializeClasses());
    EXPECT_NO_THROW(T::initializeClasses());
    EXPECT_EQ(before, BufferedRequestBodySource::JxxClassInfoMarker::Class());
}
