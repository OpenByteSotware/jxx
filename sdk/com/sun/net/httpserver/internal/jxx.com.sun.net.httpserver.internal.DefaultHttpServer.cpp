#include <chrono>
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpServer.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpContext.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpExchange.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpsExchange.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpsParameters.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpsConfigurator.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocket.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.Http11Parser.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.Headers.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpHandler.h"
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.OutputStream.h"
#include "lang/jxx.lang.Exceptions.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.Socket.h"
#include "net/jxx.net.ServerSocket.h"
#include "net/jxx.net.URI.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_Array.h"
namespace jxx::com::sun::net::httpserver::internal
{
	namespace
	{
		void writeAscii(const ::jxx::Ptr<::jxx::io::OutputStream>& out, const std::string& s)
		{
			auto a = ::jxx::NEW<::jxx::lang::ByteArrayType>((::jxx::lang::jint)s.size()); 
			for (::jxx::lang::jint i = 0; i < a->length; ++i)(*a)[i] = (::jxx::lang::jbyte)s[(std::size_t)i]; out->write(a, 0, a->length); out->flush();
		}
		void simpleResponse(const ::jxx::Ptr<::jxx::net::Socket>& s, int code, const char* reason)
		{
			try {
				auto body = std::string(reason) + "\n"; 
				writeAscii(s->getOutputStream(), "HTTP/1.1 " + std::to_string(code) + " " +
					reason + "\r\nContent-Type: text/plain; charset=utf-8\r\nContent-Length: " +
					std::to_string(body.size()) + "\r\nConnection: close\r\n\r\n" + body);
			}
			catch (...) {
			}
		}
	}
	DefaultHttpServer::DefaultHttpServer() :Super(), serverSocket_(::jxx::NEW<::jxx::net::ServerSocket>())
	{
	}
	DefaultHttpServer::DefaultHttpServer(const ::jxx::Ptr<::jxx::net::InetSocketAddress>& a, ::jxx::lang::jint b) :DefaultHttpServer()
	{
		bind(a, b);
	} DefaultHttpServer::~DefaultHttpServer()
	{
		stop(0);
	}
	void DefaultHttpServer::bind(const ::jxx::Ptr<::jxx::net::InetSocketAddress>& a, ::jxx::lang::jint b)
	{
		if (!a)throw ::jxx::lang::NullPointerException(); if (address_)throw ::jxx::lang::IllegalStateException(); serverSocket_->bind(a, b); address_ = a;
	}
	void DefaultHttpServer::start()
	{
		if (!address_)throw ::jxx::lang::IllegalStateException(); bool expected = false; if (!running_.compare_exchange_strong(expected, true))throw ::jxx::lang::IllegalStateException(); acceptThread_ = std::thread(&DefaultHttpServer::acceptLoop, this);
	}
	void DefaultHttpServer::stop(::jxx::lang::jint delay)
	{
		if (delay < 0)throw ::jxx::lang::IllegalArgumentException(); if (!running_.exchange(false))return; serverSocket_->close(); if (acceptThread_.joinable())acceptThread_.join(); if (delay > 0)std::this_thread::sleep_for(std::chrono::seconds(delay)); for (auto& t : workers_)if (t.joinable())t.join(); workers_.clear();
	}
	void DefaultHttpServer::setExecutor(const ::jxx::Ptr<::jxx::util::concurrent::Executor>& e)
	{
		if (running_)throw ::jxx::lang::IllegalStateException(); executor_ = e;
	} ::jxx::Ptr<::jxx::util::concurrent::Executor>DefaultHttpServer::getExecutor()
	{
		return executor_;
	}
	::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>DefaultHttpServer::createContext(const ::jxx::Ptr<::jxx::lang::String>& p, const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpHandler>& h)
	{
		auto c = ::jxx::NEW<DefaultHttpContext>(::jxx::CAST<::jxx::com::sun::net::httpserver::HttpServer>(this->thisPtr()), p, h); std::lock_guard<std::mutex>lock(mutex_); for (auto& e : contexts_)if (e->getPath()->equals(p))throw ::jxx::lang::IllegalArgumentException(); contexts_.push_back(c); return c;
	}
	::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>DefaultHttpServer::createContext(const ::jxx::Ptr<::jxx::lang::String>& p)
	{
		return createContext(p, nullptr);
	} void DefaultHttpServer::removeContext(const ::jxx::Ptr<::jxx::lang::String>& p)
	{
		std::lock_guard<std::mutex>lock(mutex_); for (auto i = contexts_.begin(); i != contexts_.end(); ++i)if ((*i)->getPath()->equals(p)) {
			contexts_.erase(i); return;
		}throw ::jxx::lang::IllegalArgumentException();
	} void DefaultHttpServer::removeContext(const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>& c)
	{
		if (!c)throw ::jxx::lang::NullPointerException(); removeContext(c->getPath());
	} ::jxx::Ptr<::jxx::net::InetSocketAddress>DefaultHttpServer::getAddress()
	{
		return address_;
	}
	void DefaultHttpServer::acceptLoop()
	{
		while (running_) {
			try {
				auto s = serverSocket_->accept(); if (s)workers_.emplace_back(&DefaultHttpServer::serve, this, s);
			}
			catch (...) {
				if (!running_)break;
			}
		}
	}
	void DefaultHttpServer::serve(const ::jxx::Ptr<::jxx::net::Socket>& socket)
	{
		if (!socket)return; try {
			auto sslSocket = ::jxx::CAST<::jxx::ext::net::ssl::SSLSocket>(socket);
			if (sslSocket != nullptr && httpsConfigurator_ != nullptr) {
				auto remoteAddress = ::jxx::CAST<::jxx::net::InetSocketAddress>(socket->getRemoteSocketAddress());
				if (remoteAddress == nullptr) throw ::jxx::lang::IllegalStateException();
				auto httpsParameters = ::jxx::NEW<DefaultHttpsParameters>(remoteAddress, httpsConfigurator_);
				httpsConfigurator_->configure(httpsParameters);
				auto sslParameters = httpsParameters->getAppliedSSLParameters();
				if (sslParameters != nullptr) {
					sslSocket->setSSLParameters(sslParameters);
				}
				else {
					auto suites = httpsParameters->getCipherSuites();
					if (suites != nullptr) sslSocket->setEnabledCipherSuites(suites);
					auto protocols = httpsParameters->getProtocols();
					if (protocols != nullptr) sslSocket->setEnabledProtocols(protocols);
					sslSocket->setNeedClientAuth(httpsParameters->getNeedClientAuth());
					sslSocket->setWantClientAuth(httpsParameters->getWantClientAuth());
				}
				sslSocket->startHandshake();
			}
			auto in = socket->getInputStream(); std::vector<unsigned char>buffer; buffer.reserve(8192); Http11Parser parser; for (;;) {
				ParsedRequest request; std::size_t used = 0;
				std::string error; 
				auto result = parser.parse(buffer.data(), buffer.size(), request, used, error); 
				if (result == Http11Parser::Result::NeedMore) {
					auto chunk = ::jxx::NEW<::jxx::lang::ByteArrayType>(4096);
					auto n = in->read(chunk, 0, chunk->length); if (n < 0) {
						socket->close(); return;
					}for (::jxx::lang::jint i = 0; i < n; ++i)buffer.push_back((unsigned char)(*chunk)[i]); if (buffer.size() > 1048576) {
						simpleResponse(socket, 413, "Payload Too Large"); socket->close(); return;
					}continue;
				}if (result == Http11Parser::Result::Error) {
					simpleResponse(socket, 400, "Bad Request"); socket->close(); return;
				}auto uri = ::jxx::NEW<::jxx::net::URI>(::jxx::NEW<::jxx::lang::String>(request.target.c_str())); auto path = uri->getRawPath(); auto context = match(path ? path->utf8() : std::string("/")); if (!context || !context->getHandler()) {
					simpleResponse(socket, 404, "Not Found"); socket->close(); return;
				}auto headers = ::jxx::NEW<::jxx::com::sun::net::httpserver::Headers>();
				for (const auto& h : request.headers)
					headers->add(::jxx::NEW<::jxx::lang::String>(h.first.c_str()),
						::jxx::NEW<::jxx::lang::String>(h.second.c_str())); 
				auto body = ::jxx::NEW<::jxx::lang::ByteArrayType>((::jxx::lang::jint)
					request.body.size()); for (::jxx::lang::jint i = 0; i < body->length; ++i)(*body)[i] = (::jxx::lang::jbyte)request.body[(std::size_t)i]; auto httpExchange = ::jxx::NEW<DefaultHttpExchange>(socket, context, ::jxx::NEW<::jxx::lang::String>(request.method.c_str()), uri, ::jxx::NEW<::jxx::lang::String>(request.version.c_str()), headers, body);
				::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpExchange> exchange = httpExchange;
				if (sslSocket != nullptr) {
					auto session = sslSocket->getSession();
					exchange = ::jxx::NEW<DefaultHttpsExchange>(httpExchange, session);
				}
				context->getHandler()->handle(exchange);
				if (exchange->getResponseCode() < 0) exchange->sendResponseHeaders(200, -1);
				exchange->getResponseBody()->flush(); buffer.erase(buffer.begin(), buffer.begin() + (std::ptrdiff_t)used); if (!request.keepAlive) {
					exchange->close(); return;
				}if (buffer.empty())continue;
			}
		}
		catch (...) {
			simpleResponse(socket, 500, "Internal Server Error"); socket->close();
		}
	}
	::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>DefaultHttpServer::match(const std::string& p)
	{
		std::lock_guard<std::mutex>lock(mutex_); 
		::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>best; 
		std::size_t n = 0; for (auto& c : contexts_) {
			auto q = c->getPath()->utf8(); 
			if (p.compare(0, q.size(), q) == 0 && q.size() > n) {
				best = c; n = q.size();
			}
		}return best;
	}
}
