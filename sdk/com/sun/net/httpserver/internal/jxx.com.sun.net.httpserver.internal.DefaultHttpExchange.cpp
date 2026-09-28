#include <exception>
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
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
namespace {
std::string reasonPhrase_(::jxx::lang::jint code)
{
    switch (code) {
    case 100: return "Continue";
    case 101: return "Switching Protocols";
    case 200: return "OK";
    case 201: return "Created";
    case 202: return "Accepted";
    case 203: return "Non-Authoritative Information";
    case 204: return "No Content";
    case 205: return "Reset Content";
    case 206: return "Partial Content";
    case 300: return "Multiple Choices";
    case 301: return "Moved Permanently";
    case 302: return "Found";
    case 303: return "See Other";
    case 304: return "Not Modified";
    case 305: return "Use Proxy";
    case 307: return "Temporary Redirect";
    case 400: return "Bad Request";
    case 401: return "Unauthorized";
    case 402: return "Payment Required";
    case 403: return "Forbidden";
    case 404: return "Not Found";
    case 405: return "Method Not Allowed";
    case 406: return "Not Acceptable";
    case 407: return "Proxy Authentication Required";
    case 408: return "Request Timeout";
    case 409: return "Conflict";
    case 410: return "Gone";
    case 411: return "Length Required";
    case 412: return "Precondition Failed";
    case 413: return "Payload Too Large";
    case 414: return "URI Too Long";
    case 415: return "Unsupported Media Type";
    case 416: return "Requested Range Not Satisfiable";
    case 417: return "Expectation Failed";
    case 500: return "Internal Server Error";
    case 501: return "Not Implemented";
    case 502: return "Bad Gateway";
    case 503: return "Service Unavailable";
    case 504: return "Gateway Timeout";
    case 505: return "HTTP Version Not Supported";
    default: return "";
    }
}

std::string currentHttpDate_()
{
    const auto now = std::chrono::system_clock::now();
    const auto current = std::chrono::system_clock::to_time_t(now);
    std::tm utc{};
#if defined(_WIN32)
    if (gmtime_s(&utc, &current) != 0) return {};
#else
    if (gmtime_r(&current, &utc) == nullptr) return {};
#endif
    std::ostringstream output;
    output.imbue(std::locale::classic());
    output << std::put_time(&utc, "%a, %d %b %Y %H:%M:%S GMT");
    return output.str();
}

bool containsConnectionToken_(const ::jxx::Ptr<::jxx::lang::String>& value,const std::string& requested)
{
    if (value == nullptr) return false;
    auto text = value->utf8();
    std::size_t position = 0;
    while (position <= text.size()) {
        const auto comma = text.find(',', position);
        const auto end = comma == std::string::npos ? text.size() : comma;
        auto first = position;
        while (first < end && (text[first] == ' ' || text[first] == '\t')) ++first;
        auto last = end;
        while (last > first && (text[last - 1] == ' ' || text[last - 1] == '\t')) --last;
        std::string token = text.substr(first, last - first);
        for (auto& character : token) if (character >= 'A' && character <= 'Z') character = static_cast<char>(character - 'A' + 'a');
        if (token == requested) return true;
        if (comma == std::string::npos) break;
        position = comma + 1;
    }
    return false;
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
		if (closed_) return;
		closed_ = true;
		completeInternal();
	}
	void DefaultHttpExchange::completeInternal()
	{
		if (completed_) return;
		std::exception_ptr firstFailure;
		try {
			if (output_ != nullptr) output_->close();
			else if (responseCode_ == -1) reusable_ = false;
		}
		catch (...) {
			reusable_ = false;
			firstFailure = std::current_exception();
		}
		try {
			if (input_ != nullptr) input_->close();
		}
		catch (...) {
			reusable_ = false;
			if (!firstFailure) firstFailure = std::current_exception();
		}
		completed_ = true;
		if (firstFailure) std::rethrow_exception(firstFailure);
	}
	::jxx::lang::jbool DefaultHttpExchange::isCompletedInternal() const noexcept { return completed_; }
	::jxx::lang::jbool DefaultHttpExchange::isResponseCommittedInternal() const noexcept { return responseCode_ >= 0; }
	::jxx::lang::jbool DefaultHttpExchange::isConnectionReusableInternal() const noexcept { return completed_ && reusable_ && !responseClose_; }
	::jxx::lang::jbool DefaultHttpExchange::responseRequestsCloseInternal() const noexcept { return responseClose_; }::jxx::Ptr<::jxx::io::InputStream>DefaultHttpExchange::getRequestBody()
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
		if (code < 100 || code > 999) throw ::jxx::lang::IllegalArgumentException();
		responseCode_ = code;
		const bool statusForbidsBody = (code >= 100 && code < 200) || code == 204 || code == 304;
		const bool headRequest = method_ != nullptr && method_->utf8() == "HEAD";
		const bool http10 = protocol_ != nullptr && protocol_->utf8() == "HTTP/1.0";
		ResponseBodyMode mode;
		::jxx::lang::jlong fixedLength = 0;
		if (statusForbidsBody || headRequest || length < 0) mode = ResponseBodyMode::NoBody;
		else if (length == 0 && !http10) mode = ResponseBodyMode::Chunked;
		else if (length == 0) { mode = ResponseBodyMode::FixedLength; fixedLength = 0; }
		else { mode = ResponseBodyMode::FixedLength; fixedLength = length; }

		// Server-controlled framing always wins over application headers.
		responseHeaders_->remove(::jxx::NEW<::jxx::lang::String>("content-length"));
		responseHeaders_->remove(::jxx::NEW<::jxx::lang::String>("transfer-encoding"));
		auto connectionValue = responseHeaders_->getFirst(::jxx::NEW<::jxx::lang::String>("connection"));
		if (containsConnectionToken_(connectionValue, "close")) responseClose_ = true;
		if (http10 && mode != ResponseBodyMode::FixedLength) responseClose_ = true;
		bool requestKeepAlive = false;
		auto requestConnection = requestHeaders_->getFirst(::jxx::NEW<::jxx::lang::String>("connection"));
		if (requestConnection != nullptr) {
			requestKeepAlive = containsConnectionToken_(requestConnection, "keep-alive") && !containsConnectionToken_(requestConnection, "close");
		}
		responseHeaders_->remove(::jxx::NEW<::jxx::lang::String>("connection"));
		auto dateKey = ::jxx::NEW<::jxx::lang::String>("date");
		if (responseHeaders_->getFirst(dateKey) == nullptr) {
			const auto dateValue = currentHttpDate_();
			if (!dateValue.empty()) responseHeaders_->set(dateKey, ::jxx::NEW<::jxx::lang::String>(dateValue.c_str()));
		}

		const std::string responseProtocol = http10 ? "HTTP/1.0" : "HTTP/1.1";
		std::string message = responseProtocol + " " + std::to_string(code) + " " + reasonPhrase_(code) + "\r\n";
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
		if (responseClose_) message += "Connection: close\r\n";
		else if (http10 && requestKeepAlive && mode == ResponseBodyMode::FixedLength) message += "Connection: keep-alive\r\n";
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
