#pragma once
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpExchange.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.Headers.h"
#include "util/jxx.util.HashMap.h"
namespace jxx::net { class Socket; }
namespace jxx::com::sun::net::httpserver::internal {
class DefaultHttpExchange final : public ::jxx::lang::ClassBase<DefaultHttpExchange,::jxx::com::sun::net::httpserver::HttpExchange> {
public:
 using JxxSuper=::jxx::com::sun::net::httpserver::HttpExchange; using Super=::jxx::lang::ClassBase<DefaultHttpExchange,JxxSuper>;
 DefaultHttpExchange(const ::jxx::Ptr<::jxx::net::Socket>&socket,const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>&context,const ::jxx::Ptr<::jxx::lang::String>&method,const ::jxx::Ptr<::jxx::net::URI>&uri,const ::jxx::Ptr<::jxx::lang::String>&protocol,const ::jxx::Ptr<::jxx::com::sun::net::httpserver::Headers>&requestHeaders,const ::jxx::lang::ByteArray&body);
 ::jxx::Ptr<::jxx::com::sun::net::httpserver::Headers>getRequestHeaders()override;::jxx::Ptr<::jxx::com::sun::net::httpserver::Headers>getResponseHeaders()override;::jxx::Ptr<::jxx::net::URI>getRequestURI()override;::jxx::Ptr<::jxx::lang::String>getRequestMethod()override;::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>getHttpContext()override;
 void close()override;::jxx::Ptr<::jxx::io::InputStream>getRequestBody()override;::jxx::Ptr<::jxx::io::OutputStream>getResponseBody()override;void sendResponseHeaders(::jxx::lang::jint code,::jxx::lang::jlong length)override;
 ::jxx::Ptr<::jxx::net::InetSocketAddress>getRemoteAddress()override;::jxx::lang::jint getResponseCode()override;::jxx::Ptr<::jxx::net::InetSocketAddress>getLocalAddress()override;::jxx::Ptr<::jxx::lang::String>getProtocol()override;
 ::jxx::Ptr<::jxx::lang::Object>getAttribute(const ::jxx::Ptr<::jxx::lang::String>&name)override;void setAttribute(const ::jxx::Ptr<::jxx::lang::String>&name,const ::jxx::Ptr<::jxx::lang::Object>&value)override;void setStreams(const ::jxx::Ptr<::jxx::io::InputStream>&input,const ::jxx::Ptr<::jxx::io::OutputStream>&output)override;::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpPrincipal>getPrincipal()override;
private:
 ::jxx::Ptr<::jxx::net::Socket>socket_;::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>context_;::jxx::Ptr<::jxx::lang::String>method_,protocol_;::jxx::Ptr<::jxx::net::URI>uri_;::jxx::Ptr<::jxx::com::sun::net::httpserver::Headers>requestHeaders_,responseHeaders_;::jxx::Ptr<::jxx::io::InputStream>input_;::jxx::Ptr<::jxx::io::OutputStream>output_;::jxx::Ptr<::jxx::util::Map<::jxx::lang::String,::jxx::lang::Object>>attributes_;::jxx::lang::jint responseCode_=-1;::jxx::lang::jbool closed_=false;
}; }
