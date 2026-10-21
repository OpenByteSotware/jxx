#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
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
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.Filter.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpContext.h"

#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.ClassInitialization.h"
#include "com/sun/net/httpserver/spi/jxx.com.sun.net.httpserver.spi.ClassInitialization.h"
namespace jxx::com::sun::net::httpserver {


template <typename T>
void registerType()
{
    ::jxx::lang::class_info_detail::registerClassInfo<T>();
}

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        registerType<Authenticator>();
        registerType<Authenticator::Success>();
        registerType<Authenticator::Failure>();
        registerType<Authenticator::Retry>();
        registerType<BasicAuthenticator>();
        registerType<Headers>();
        registerType<HttpExchange>();
        registerType<HttpHandler>();
        registerType<HttpPrincipal>();
        registerType<HttpServer>();
        registerType<HttpsConfigurator>();
        registerType<HttpsExchange>();
        registerType<HttpsParameters>();
        registerType<HttpsServer>();
        registerType<Filter>();
        registerType<Filter::Chain>();
        registerType<HttpContext>();
    });
}


void initialize()
{
    initializeClasses();
    ::jxx::com::sun::net::httpserver::internal::initialize();
    ::jxx::com::sun::net::httpserver::spi::initialize();
}

} // namespace jxx::com::sun::net::httpserver
