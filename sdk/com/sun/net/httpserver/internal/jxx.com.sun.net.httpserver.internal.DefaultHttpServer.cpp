#include <chrono>
#include <memory>
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpServer.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpContext.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpExchange.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpsExchange.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpsParameters.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpsConfigurator.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.Authenticator.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocket.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLException.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.Http11Parser.h"
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.ServerConnectionTask.h"
#include "util/jxx.util.concurrent.Executor.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.Headers.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpHandler.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.Filter.h"
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.OutputStream.h"
#include "lang/jxx.lang.Exceptions.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.Socket.h"
#include "net/jxx.net.ServerSocket.h"
#include "net/jxx.net.URI.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_Array.h"
#include "util/jxx.util.concurrent.RejectedExecutionException.h"
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
	}
	void DefaultHttpServer::setPublicOwnerInternal(const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpServer>& owner)
	{
		if (!owner) throw ::jxx::lang::NullPointerException();
		if (running_ || !contexts_.empty()) throw ::jxx::lang::IllegalStateException();
		publicOwner_ = owner;
	}
	void DefaultHttpServer::installListenerInternal(const ::jxx::Ptr<::jxx::net::ServerSocket>& listener,const ::jxx::Ptr<::jxx::net::InetSocketAddress>& address,const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsConfigurator>& configurator)
	{
		if (!listener || !address || !configurator) throw ::jxx::lang::NullPointerException();
		if (running_ || stopped_) throw ::jxx::lang::IllegalStateException();
		serverSocket_ = listener;
		address_ = address;
		httpsConfigurator_ = configurator;
	}
	void DefaultHttpServer::registerConnection_(const ::jxx::Ptr<::jxx::net::Socket>& socket)
	{
		std::lock_guard<std::mutex> lock(activeMutex_);
		activeConnections_[socket.get()] = socket;
	}
	void DefaultHttpServer::unregisterConnection_(const ::jxx::Ptr<::jxx::net::Socket>& socket)
	{
		{ std::lock_guard<std::mutex> lock(activeMutex_); activeConnections_.erase(socket.get()); }
		activeCondition_.notify_all();
	}
	void DefaultHttpServer::closeActiveConnections_()
	{
		std::vector<::jxx::Ptr<::jxx::net::Socket>> snapshot;
		{ std::lock_guard<std::mutex> lock(activeMutex_); for (const auto& entry : activeConnections_) snapshot.push_back(entry.second); }
		for (const auto& socket : snapshot) { try { if (socket) socket->close(); } catch (...) {} }
	}
 DefaultHttpServer::~DefaultHttpServer()
	{
		stop(0);
	}
	void DefaultHttpServer::bind(const ::jxx::Ptr<::jxx::net::InetSocketAddress>& a, ::jxx::lang::jint b)
	{
		if (!a)throw ::jxx::lang::NullPointerException(); if (address_)throw ::jxx::lang::IllegalStateException(); serverSocket_->bind(a, b); address_ = a;
	}
	void DefaultHttpServer::start()
	{
		std::lock_guard<std::mutex> shutdownLock(shutdownMutex_);
		if (!address_ || stopped_ || shutdownInProgress_ || shutdownComplete_) throw ::jxx::lang::IllegalStateException();
		bool expected = false;
		if (!running_.compare_exchange_strong(expected, true)) throw ::jxx::lang::IllegalStateException();
		taskLifetime_ = ::jxx::CAST<DefaultHttpServer>(this->thisPtr());
		try {
			acceptThread_ = std::thread(&DefaultHttpServer::acceptLoop, this);
		}
		catch (...) {
			running_ = false;
			taskLifetime_.reset();
			throw;
		}
	}
	void DefaultHttpServer::stop(::jxx::lang::jint delay)
	{
		if (delay < 0) throw ::jxx::lang::IllegalArgumentException();
		{
			std::unique_lock<std::mutex> shutdownLock(shutdownMutex_);
			if (shutdownComplete_) return;
			if (shutdownInProgress_) {
				shutdownCondition_.wait(shutdownLock, [this] { return shutdownComplete_; });
				return;
			}
			shutdownInProgress_ = true;
			stopped_ = true;
		}

		running_ = false;
		try { if (serverSocket_) serverSocket_->close(); } catch (...) {}
		if (acceptThread_.joinable()) {
			if (acceptThread_.get_id() == std::this_thread::get_id()) acceptThread_.detach();
			else acceptThread_.join();
		}

		const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(delay);
		{
			std::unique_lock<std::mutex> lock(activeMutex_);
			if (delay > 0) activeCondition_.wait_until(lock, deadline, [this] { return activeConnections_.empty(); });
		}
		closeActiveConnections_();

		{
			std::lock_guard<std::mutex> lock(workersMutex_);
			for (auto& worker : workers_) {
				if (!worker.joinable()) continue;
				if (worker.get_id() == std::this_thread::get_id()) worker.detach();
				else worker.join();
			}
			workers_.clear();
		}
		taskLifetime_.reset();

		{
			std::lock_guard<std::mutex> shutdownLock(shutdownMutex_);
			shutdownInProgress_ = false;
			shutdownComplete_ = true;
		}
		shutdownCondition_.notify_all();
	}
	void DefaultHttpServer::setExecutor(const ::jxx::Ptr<::jxx::util::concurrent::Executor>& e)
	{
		if (running_ || stopped_) throw ::jxx::lang::IllegalStateException(); executor_ = e;
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
				auto socket = serverSocket_->accept();
				if (!socket) continue;
				registerConnection_(socket);
				if (executor_) {
					try {
						auto owner = taskLifetime_;
						auto task = ::jxx::NEW<ServerConnectionTask>([owner, socket] { if (owner) owner->serve(socket); });
						executor_->execute(::jxx::CAST<::jxx::lang::Runnable>(task));
					}
					catch (const ::jxx::util::concurrent::RejectedExecutionException&) {
						unregisterConnection_(socket);
						socket->close();
					}
				}
				else {
					std::lock_guard<std::mutex> lock(workersMutex_);
					workers_.emplace_back(&DefaultHttpServer::serve, this, socket);
				}
			}
			catch (...) { if (!running_) break; }
		}
	}
	void DefaultHttpServer::serve(const ::jxx::Ptr<::jxx::net::Socket>& socket)
	{
		if (!socket) return;
		auto connectionRegistration = std::shared_ptr<void>(socket.get(), [this, socket](void*) { unregisterConnection_(socket); });
		bool secureTransport = false;
		bool handshakeComplete = false;
		bool httpRequestParsed = false;
		::jxx::Ptr<DefaultHttpExchange> currentExchange;
		try {
			auto sslSocket = ::jxx::CAST<::jxx::ext::net::ssl::SSLSocket>(socket);
			secureTransport = sslSocket != nullptr;
			if (sslSocket != nullptr) {
				try {
					if (!httpsConfigurator_) throw ::jxx::lang::IllegalStateException();
					auto clientAddress = ::jxx::CAST<::jxx::net::InetSocketAddress>(socket->getRemoteSocketAddress());
					auto httpsParameters = ::jxx::NEW<DefaultHttpsParameters>(clientAddress, httpsConfigurator_);
					httpsConfigurator_->configure(httpsParameters);
					auto applied = httpsParameters->getAppliedSSLParameters();
					if (applied != nullptr) {
						sslSocket->setSSLParameters(applied);
					}
					else {
						auto suites = httpsParameters->getCipherSuites();
						if (suites != nullptr) sslSocket->setEnabledCipherSuites(suites);
						auto protocols = httpsParameters->getProtocols();
						if (protocols != nullptr) sslSocket->setEnabledProtocols(protocols);
						if (httpsParameters->getNeedClientAuth()) sslSocket->setNeedClientAuth(true);
						else if (httpsParameters->getWantClientAuth()) sslSocket->setWantClientAuth(true);
					}
					sslSocket->startHandshake();
					handshakeComplete = true;
				}
				catch (const ::jxx::ext::net::ssl::SSLException&) {
					socket->close();
					return;
				}
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
				}
				if (result == Http11Parser::Result::Error) {
					simpleResponse(socket, 400, "Bad Request"); socket->close(); return;
				}
				httpRequestParsed = true;
				auto uri = ::jxx::NEW<::jxx::net::URI>(::jxx::NEW<::jxx::lang::String>(request.target.c_str())); auto path = uri->getRawPath(); auto context = match(path ? path->utf8() : std::string("/")); if (!context || !context->getHandler()) {
					simpleResponse(socket, 404, "Not Found"); socket->close(); return;
				}auto headers = ::jxx::NEW<::jxx::com::sun::net::httpserver::Headers>();
				for (const auto& h : request.headers)
					headers->add(::jxx::NEW<::jxx::lang::String>(h.first.c_str()),
						::jxx::NEW<::jxx::lang::String>(h.second.c_str()));
				for (const auto& trailer : request.trailers)
					headers->add(::jxx::NEW<::jxx::lang::String>(trailer.first.c_str()),
						::jxx::NEW<::jxx::lang::String>(trailer.second.c_str()));
				headers->freezeInternal(); 
				auto body = ::jxx::NEW<::jxx::lang::ByteArrayType>((::jxx::lang::jint)
					request.body.size()); for (::jxx::lang::jint i = 0; i < body->length; ++i)(*body)[i] = (::jxx::lang::jbyte)request.body[(std::size_t)i]; auto httpExchange = ::jxx::NEW<DefaultHttpExchange>(socket, context, ::jxx::NEW<::jxx::lang::String>(request.method.c_str()), uri, ::jxx::NEW<::jxx::lang::String>(request.version.c_str()), headers, body);
				currentExchange = httpExchange;
				::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpExchange> exchange = httpExchange;
				if (sslSocket != nullptr) {
					auto session = sslSocket->getSession();
					exchange = ::jxx::NEW<DefaultHttpsExchange>(httpExchange, session);
				}
				auto authenticator = context->getAuthenticator();
				if (authenticator != nullptr) {
					auto authentication = authenticator->authenticate(exchange);
					if (authentication == nullptr) throw ::jxx::lang::NullPointerException();
					auto success = ::jxx::CAST<::jxx::com::sun::net::httpserver::Authenticator::Success>(authentication);
					if (success != nullptr) {
						httpExchange->setPrincipalInternal(success->getPrincipal());
					}
					else {
						auto failure = ::jxx::CAST<::jxx::com::sun::net::httpserver::Authenticator::Failure>(authentication);
						auto retry = ::jxx::CAST<::jxx::com::sun::net::httpserver::Authenticator::Retry>(authentication);
						const ::jxx::lang::jint responseCode = failure != nullptr ? failure->getResponseCode() : (retry != nullptr ? retry->getResponseCode() : 500);
						exchange->sendResponseHeaders(responseCode, -1);
						httpExchange->completeInternal();
						socket->close();
						return;
					}
				}
				auto chain = ::jxx::NEW<::jxx::com::sun::net::httpserver::Filter::Chain>(
					context->getFilters(), context->getHandler());
				chain->doFilter(exchange);
				if (exchange->getResponseCode() < 0) exchange->sendResponseHeaders(200, -1);
				httpExchange->completeInternal();
				buffer.erase(buffer.begin(), buffer.begin() + (std::ptrdiff_t)used);
				if (!request.keepAlive || !httpExchange->isConnectionReusableInternal()) { socket->close(); return; }
				if (buffer.empty()) continue;
			}
		}
		catch (...) {
			const bool responseCommitted = currentExchange != nullptr && currentExchange->isResponseCommittedInternal();
			if (httpRequestParsed && !responseCommitted && (!secureTransport || handshakeComplete)) {
				try { simpleResponse(socket, 500, "Internal Server Error"); } catch (...) {}
			}
			socket->close();
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
