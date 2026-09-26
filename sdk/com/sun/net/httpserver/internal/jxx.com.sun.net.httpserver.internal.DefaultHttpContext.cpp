#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpContext.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.Authenticator.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.Filter.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpHandler.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.String.h"
namespace jxx::com::sun::net::httpserver::internal
{
	DefaultHttpContext::DefaultHttpContext(
		const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpServer>& s, 
		const ::jxx::Ptr<::jxx::lang::String>& p, 
		const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpHandler>& h) :
		Super(), server_(s), path_(p), handler_(h), 
		attributes_(::jxx::NEW<::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::Object>>()), 
		filters_(::jxx::NEW<::jxx::util::ArrayList<::jxx::com::sun::net::httpserver::Filter>>())
	{
		if (!s || !p || p->utf8().empty() || p->utf8()[0] != '/')throw ::jxx::lang::IllegalArgumentException();
	}
	::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpHandler> DefaultHttpContext::getHandler()
	{
		return handler_;
	} void DefaultHttpContext::setHandler(const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpHandler>& h)
	{
		if (!h)throw ::jxx::lang::NullPointerException(); handler_ = h;
	}
	::jxx::Ptr<::jxx::lang::String>DefaultHttpContext::getPath()
	{
		return path_;
	}::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpServer>DefaultHttpContext::getServer()
	{
		return server_;
	}
	::jxx::Ptr<::jxx::util::Map<::jxx::lang::String, ::jxx::lang::Object>>DefaultHttpContext::getAttributes()
	{
		return attributes_;
	}::jxx::Ptr<::jxx::util::List<::jxx::com::sun::net::httpserver::Filter>>DefaultHttpContext::getFilters()
	{
		return filters_;
	}
	::jxx::Ptr<::jxx::com::sun::net::httpserver::Authenticator>DefaultHttpContext::setAuthenticator(const ::jxx::Ptr<::jxx::com::sun::net::httpserver::Authenticator>& a)
	{
		auto old = authenticator_; authenticator_ = a; return old;
	}::jxx::Ptr<::jxx::com::sun::net::httpserver::Authenticator>DefaultHttpContext::getAuthenticator()
	{
		return authenticator_;
	}
}
