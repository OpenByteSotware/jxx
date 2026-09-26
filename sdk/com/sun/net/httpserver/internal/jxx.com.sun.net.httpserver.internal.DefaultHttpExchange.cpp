#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpExchange.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.OutputStream.h"
#include "lang/jxx.lang.Exceptions.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.Socket.h"
#include "net/jxx.net.URI.h"
namespace jxx::com::sun::net::httpserver::internal
{
	DefaultHttpExchange::DefaultHttpExchange(const ::jxx::Ptr<::jxx::net::Socket>& s, const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>& c, const ::jxx::Ptr<::jxx::lang::String>& m, const ::jxx::Ptr<::jxx::net::URI>& u, const ::jxx::Ptr<::jxx::lang::String>& p, const ::jxx::Ptr<::jxx::com::sun::net::httpserver::Headers>& h, const ::jxx::lang::ByteArray& b) :Super(), socket_(s), context_(c), method_(m), protocol_(p), uri_(u), requestHeaders_(h), responseHeaders_(::jxx::NEW<::jxx::com::sun::net::httpserver::Headers>()), input_(::jxx::NEW<::jxx::io::ByteArrayInputStream>(b)), attributes_(::jxx::NEW<::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::Object>>())
	{
		if (!s || !c || !m || !u || !p || !h)throw ::jxx::lang::NullPointerException();
	}
	::jxx::Ptr<::jxx::com::sun::net::httpserver::Headers>DefaultHttpExchange::getRequestHeaders()
	{
		return requestHeaders_;
	}::jxx::Ptr<::jxx::com::sun::net::httpserver::Headers>DefaultHttpExchange::getResponseHeaders()
	{
		return responseHeaders_;
	}::jxx::Ptr<::jxx::net::URI>DefaultHttpExchange::getRequestURI()
	{
		return uri_;
	}::jxx::Ptr<::jxx::lang::String>DefaultHttpExchange::getRequestMethod()
	{
		return method_;
	}::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>DefaultHttpExchange::getHttpContext()
	{
		return context_;
	}
	void DefaultHttpExchange::close()
	{
		if (closed_)return; closed_ = true; if (input_)input_->close(); if (output_)output_->close(); if (socket_)socket_->close();
	}::jxx::Ptr<::jxx::io::InputStream>DefaultHttpExchange::getRequestBody()
	{
		return input_;
	}::jxx::Ptr<::jxx::io::OutputStream>DefaultHttpExchange::getResponseBody()
	{
		if (!output_)output_ = socket_->getOutputStream(); return output_;
	}
	void DefaultHttpExchange::sendResponseHeaders(::jxx::lang::jint code, ::jxx::lang::jlong length)
	{
		if (responseCode_ != -1)throw ::jxx::lang::IllegalStateException(); 
		responseCode_ = code;
		std::string msg = "HTTP/1.1 " + std::to_string(code) + "\r\nContent-Length: " + 
			std::to_string(length < 0 ? 0 : length) + "\r\n\r\n"; 
		auto out = getResponseBody(); 
		auto bytes = ::jxx::NEW<::jxx::lang::ByteArrayType>((::jxx::lang::jint)msg.size());
		for (::jxx::lang::jint i = 0; i < bytes->length; ++i)(*bytes)[i] = (::jxx::lang::jbyte)msg[(std::size_t)i]; out->write(bytes, 0, bytes->length);
	}
	::jxx::Ptr<::jxx::net::InetSocketAddress>DefaultHttpExchange::getRemoteAddress()
	{
		return ::jxx::CAST<::jxx::net::InetSocketAddress>(socket_->getRemoteSocketAddress());
	}::jxx::lang::jint DefaultHttpExchange::getResponseCode()
	{
		return responseCode_;
	}::jxx::Ptr<::jxx::net::InetSocketAddress>DefaultHttpExchange::getLocalAddress()
	{
		return ::jxx::CAST<::jxx::net::InetSocketAddress>(socket_->getLocalSocketAddress());
	}::jxx::Ptr<::jxx::lang::String>DefaultHttpExchange::getProtocol()
	{
		return protocol_;
	}
	::jxx::Ptr<::jxx::lang::Object>DefaultHttpExchange::getAttribute(const ::jxx::Ptr<::jxx::lang::String>& n)
	{
		return attributes_->get(n);
	}void DefaultHttpExchange::setAttribute(const ::jxx::Ptr<::jxx::lang::String>& n, const ::jxx::Ptr<::jxx::lang::Object>& v)
	{
		if (!n)throw ::jxx::lang::NullPointerException(); attributes_->put(n, v);
	}void DefaultHttpExchange::setStreams(const ::jxx::Ptr<::jxx::io::InputStream>& i, const ::jxx::Ptr<::jxx::io::OutputStream>& o)
	{
		if (i)input_ = i; if (o)output_ = o;
	}::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpPrincipal>DefaultHttpExchange::getPrincipal()
	{
		return nullptr;
	}
}
