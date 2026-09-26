#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpsConfigurator.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpsParameters.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "lang/jxx.lang.Exceptions.h"
namespace jxx::com::sun::net::httpserver {
HttpsConfigurator::HttpsConfigurator(const ::jxx::Ptr<::jxx::ext::net::ssl::SSLContext>&c):Super(),context_(c){if(!c)throw ::jxx::lang::NullPointerException();}
::jxx::Ptr<::jxx::ext::net::ssl::SSLContext> HttpsConfigurator::getSSLContext()const{return context_;}
void HttpsConfigurator::configure(const ::jxx::Ptr<HttpsParameters>&p){if(!p)throw ::jxx::lang::NullPointerException();p->setSSLParameters(context_->getDefaultSSLParameters());}
}
