#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.ClassInitialization.h"

#include <mutex>

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
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.ReadOnlyList.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.ReadOnlyViews.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.RequestBodyInputStream.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.RequestBodySource.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.ResponseBodyOutputStream.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.ServerConnectionTask.h"

namespace jxx::com::sun::net::httpserver::internal {

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)BufferedRequestBodySource::JxxClassInfoMarker::Class();
        (void)ChunkedRequestBodySource::JxxClassInfoMarker::Class();
        (void)DefaultHttpContext::JxxClassInfoMarker::Class();
        (void)DefaultHttpExchange::JxxClassInfoMarker::Class();
        (void)DefaultHttpServer::JxxClassInfoMarker::Class();
        (void)DefaultHttpServerProvider::JxxClassInfoMarker::Class();
        (void)DefaultHttpsExchange::JxxClassInfoMarker::Class();
        (void)DefaultHttpsParameters::JxxClassInfoMarker::Class();
        (void)DefaultHttpsServer::JxxClassInfoMarker::Class();
        (void)FixedLengthRequestBodySource::JxxClassInfoMarker::Class();
        (void)ReadOnlyListIterator<::jxx::lang::Object>::Class();
        (void)ReadOnlyList<::jxx::lang::Object>::Class();
        (void)ReadOnlyIterator<::jxx::lang::Object>::Class();
        (void)ReadOnlyMapEntry<::jxx::lang::Object, ::jxx::lang::Object>::Class();
        (void)ReadOnlyEntryIterator<::jxx::lang::Object, ::jxx::lang::Object>::Class();
        (void)ReadOnlyCollection<::jxx::lang::Object>::Class();
        (void)ReadOnlySet<::jxx::lang::Object>::Class();
        (void)ReadOnlyEntrySet<::jxx::lang::Object, ::jxx::lang::Object>::Class();
        (void)RequestBodyInputStream::JxxClassInfoMarker::Class();
        (void)RequestBodySource::JxxClassInfoMarker::Class();
        (void)ResponseBodyOutputStream::JxxClassInfoMarker::Class();
        (void)ServerConnectionTask::JxxClassInfoMarker::Class();
    });
}


void initialize()
{
    initializeClasses();
}

} // namespace jxx::com::sun::net::httpserver::internal
