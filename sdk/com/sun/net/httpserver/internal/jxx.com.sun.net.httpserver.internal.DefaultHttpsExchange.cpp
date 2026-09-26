#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpsExchange.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpExchange.h"
#include "lang/jxx.lang.Exceptions.h"

namespace jxx::com::sun::net::httpserver::internal {

DefaultHttpsExchange::DefaultHttpsExchange(
    const ::jxx::Ptr<DefaultHttpExchange>& delegate,
    const ::jxx::Ptr<::jxx::ext::net::ssl::SSLSession>& session)
    : Super(), delegate_(delegate), session_(session)
{
    if (delegate_ == nullptr || session_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

::jxx::Ptr<::jxx::ext::net::ssl::SSLSession>
DefaultHttpsExchange::getSSLSession() { return session_; }

::jxx::Ptr<::jxx::com::sun::net::httpserver::Headers>
DefaultHttpsExchange::getRequestHeaders(){return delegate_->getRequestHeaders();}
::jxx::Ptr<::jxx::com::sun::net::httpserver::Headers>
DefaultHttpsExchange::getResponseHeaders(){return delegate_->getResponseHeaders();}
::jxx::Ptr<::jxx::net::URI> DefaultHttpsExchange::getRequestURI(){return delegate_->getRequestURI();}
::jxx::Ptr<::jxx::lang::String> DefaultHttpsExchange::getRequestMethod(){return delegate_->getRequestMethod();}
::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext> DefaultHttpsExchange::getHttpContext(){return delegate_->getHttpContext();}
void DefaultHttpsExchange::close(){delegate_->close();}
::jxx::Ptr<::jxx::io::InputStream> DefaultHttpsExchange::getRequestBody(){return delegate_->getRequestBody();}
::jxx::Ptr<::jxx::io::OutputStream> DefaultHttpsExchange::getResponseBody(){return delegate_->getResponseBody();}
void DefaultHttpsExchange::sendResponseHeaders(::jxx::lang::jint c,::jxx::lang::jlong l){delegate_->sendResponseHeaders(c,l);}
::jxx::Ptr<::jxx::net::InetSocketAddress> DefaultHttpsExchange::getRemoteAddress(){return delegate_->getRemoteAddress();}
::jxx::lang::jint DefaultHttpsExchange::getResponseCode(){return delegate_->getResponseCode();}
::jxx::Ptr<::jxx::net::InetSocketAddress> DefaultHttpsExchange::getLocalAddress(){return delegate_->getLocalAddress();}
::jxx::Ptr<::jxx::lang::String> DefaultHttpsExchange::getProtocol(){return delegate_->getProtocol();}
::jxx::Ptr<::jxx::lang::Object> DefaultHttpsExchange::getAttribute(const ::jxx::Ptr<::jxx::lang::String>&n){return delegate_->getAttribute(n);}
void DefaultHttpsExchange::setAttribute(const ::jxx::Ptr<::jxx::lang::String>&n,const ::jxx::Ptr<::jxx::lang::Object>&v){delegate_->setAttribute(n,v);}
void DefaultHttpsExchange::setStreams(const ::jxx::Ptr<::jxx::io::InputStream>&i,const ::jxx::Ptr<::jxx::io::OutputStream>&o){delegate_->setStreams(i,o);}
::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpPrincipal> DefaultHttpsExchange::getPrincipal(){return delegate_->getPrincipal();}

} // namespace jxx::com::sun::net::httpserver::internal
