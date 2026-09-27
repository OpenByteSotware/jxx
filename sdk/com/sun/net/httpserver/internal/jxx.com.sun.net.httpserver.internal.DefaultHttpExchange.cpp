#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpExchange.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.OutputStream.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.ResponseBodyOutputStream.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.MapEntry.h"
#include "lang/jxx.lang.Exceptions.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.Socket.h"
#include "net/jxx.net.URI.h"
namespace {
std::string reasonPhrase_(::jxx::lang::jint code)
{
    switch (code) {
    case 200: return "OK"; case 201: return "Created"; case 202: return "Accepted";
    case 204: return "No Content"; case 301: return "Moved Permanently"; case 302: return "Found";
    case 304: return "Not Modified"; case 400: return "Bad Request"; case 401: return "Unauthorized";
    case 403: return "Forbidden"; case 404: return "Not Found"; case 405: return "Method Not Allowed";
    case 413: return "Payload Too Large"; case 500: return "Internal Server Error";
    case 501: return "Not Implemented"; case 503: return "Service Unavailable"; default: return "";
    }
}
}
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
		if (responseCode_ == -1) throw ::jxx::lang::IllegalStateException();
		return output_;
	}
	void DefaultHttpExchange::sendResponseHeaders(::jxx::lang::jint code, ::jxx::lang::jlong length)
	{
		if (responseCode_ != -1) throw ::jxx::lang::IllegalStateException();
		responseCode_ = code;
		const bool statusForbidsBody = (code >= 100 && code < 200) || code == 204 || code == 304;
		const bool headRequest = method_ != nullptr && method_->utf8() == "HEAD";
		ResponseBodyMode mode;
		::jxx::lang::jlong fixedLength = 0;
		if (statusForbidsBody || headRequest || length < 0) mode = ResponseBodyMode::NoBody;
		else if (length == 0) mode = ResponseBodyMode::Chunked;
		else { mode = ResponseBodyMode::FixedLength; fixedLength = length; }

		// Server-controlled framing always wins over application headers.
		responseHeaders_->remove(::jxx::NEW<::jxx::lang::String>("content-length"));
		responseHeaders_->remove(::jxx::NEW<::jxx::lang::String>("transfer-encoding"));

		std::string message = "HTTP/1.1 " + std::to_string(code) + " " + reasonPhrase_(code) + "\r\n";
		auto entries = responseHeaders_->entrySet()->iterator();
		while (entries->hasNext()) {
			auto entry = entries->next();
			if (entry == nullptr || entry->getKey() == nullptr || entry->getValue() == nullptr) continue;
			const auto name = entry->getKey()->utf8();
			if (name.find('\r') != std::string::npos || name.find('\n') != std::string::npos) throw ::jxx::lang::IllegalArgumentException();
			auto values = entry->getValue();
			for (::jxx::lang::jint index = 0; index < values->size(); ++index) {
				auto value = values->get(index); if (value == nullptr) continue;
				const auto text = value->utf8();
				if (text.find('\r') != std::string::npos || text.find('\n') != std::string::npos) throw ::jxx::lang::IllegalArgumentException();
				message += name + ": " + text + "\r\n";
			}
		}
		if (mode == ResponseBodyMode::FixedLength) message += "Content-Length: " + std::to_string(fixedLength) + "\r\n";
		else if (mode == ResponseBodyMode::Chunked) message += "Transfer-Encoding: chunked\r\n";
		message += "\r\n";
		rawOutput_ = socket_->getOutputStream();
		auto bytes = ::jxx::NEW<::jxx::lang::ByteArrayType>(static_cast<::jxx::lang::jint>(message.size()));
		for (::jxx::lang::jint index = 0; index < bytes->length; ++index) (*bytes)[index] = static_cast<::jxx::lang::jbyte>(message[static_cast<std::size_t>(index)]);
		rawOutput_->write(bytes, 0, bytes->length);
		output_ = ::jxx::NEW<ResponseBodyOutputStream>(rawOutput_, mode, fixedLength);
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
		return principal_;
	}
	void DefaultHttpExchange::setPrincipalInternal(const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpPrincipal>& principal)
	{
		principal_ = principal;
	}
}
