#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpsServer.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpsConfigurator.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLServerSocketFactory.h"
#include "lang/jxx.lang.Exceptions.h"
#include "net/jxx.net.InetSocketAddress.h"

namespace jxx::com::sun::net::httpserver::internal {

DefaultHttpsServer::DefaultHttpsServer()
    : Super(), delegate_(::jxx::NEW<DefaultHttpServer>()) {}

DefaultHttpsServer::DefaultHttpsServer(const ::jxx::Ptr<::jxx::net::InetSocketAddress>& address, ::jxx::lang::jint backlog)
    : DefaultHttpsServer()
{
    if (!address) throw ::jxx::lang::NullPointerException();
    address_ = address;
    backlog_ = backlog <= 0 ? 50 : backlog;
}

void DefaultHttpsServer::ensurePublicOwner_()
{
    if (ownerInstalled_) return;
    delegate_->setPublicOwnerInternal(::jxx::CAST<::jxx::com::sun::net::httpserver::HttpServer>(this->thisPtr()));
    ownerInstalled_ = true;
}

void DefaultHttpsServer::setHttpsConfigurator(const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsConfigurator>& config)
{
    if (!config) throw ::jxx::lang::NullPointerException();
    if (started_ || stopped_) throw ::jxx::lang::IllegalStateException();
    configurator_ = config;
}
::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsConfigurator> DefaultHttpsServer::getHttpsConfigurator(){ return configurator_; }

void DefaultHttpsServer::bind(const ::jxx::Ptr<::jxx::net::InetSocketAddress>& address, ::jxx::lang::jint backlog)
{
    if (!address) throw ::jxx::lang::NullPointerException();
    if (started_ || stopped_ || address_) throw ::jxx::lang::IllegalStateException();
    address_ = address;
    backlog_ = backlog <= 0 ? 50 : backlog;
}

void DefaultHttpsServer::start()
{
    if (started_ || stopped_ || configurator_ == nullptr || address_ == nullptr) throw ::jxx::lang::IllegalStateException();
    ensurePublicOwner_();
    auto context = configurator_->getSSLContext();
    if (context == nullptr) throw ::jxx::lang::IllegalStateException();
    auto factory = context->getServerSocketFactory();
    if (factory == nullptr) throw ::jxx::lang::IllegalStateException();
    auto listener = factory->createServerSocket(address_->getPort(), backlog_, address_->getAddress());
    if (listener == nullptr) throw ::jxx::lang::IllegalStateException();
    address_ = ::jxx::CAST<::jxx::net::InetSocketAddress>(listener->getLocalSocketAddress());
    delegate_->installListenerInternal(listener, address_, configurator_);
    try {
        delegate_->start();
        started_ = true;
    }
    catch (...) {
        try { listener->close(); } catch (...) {}
        throw;
    }
}
void DefaultHttpsServer::stop(::jxx::lang::jint delay)
{
    if (delay < 0) throw ::jxx::lang::IllegalArgumentException();
    stopped_ = true;
    delegate_->stop(delay);
}
void DefaultHttpsServer::setExecutor(const ::jxx::Ptr<::jxx::util::concurrent::Executor>& executor){ if(started_ || stopped_)throw ::jxx::lang::IllegalStateException(); delegate_->setExecutor(executor); }
::jxx::Ptr<::jxx::util::concurrent::Executor> DefaultHttpsServer::getExecutor(){ return delegate_->getExecutor(); }

::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext> DefaultHttpsServer::createContext(const ::jxx::Ptr<::jxx::lang::String>& path,const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpHandler>& handler)
{
    if (!handler) throw ::jxx::lang::NullPointerException();
    ensurePublicOwner_();
    return delegate_->createContext(path, handler);
}
::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext> DefaultHttpsServer::createContext(const ::jxx::Ptr<::jxx::lang::String>& path)
{
    ensurePublicOwner_();
    return delegate_->createContext(path);
}
void DefaultHttpsServer::removeContext(const ::jxx::Ptr<::jxx::lang::String>& path){ delegate_->removeContext(path); }
void DefaultHttpsServer::removeContext(const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>& context){ delegate_->removeContext(context); }
::jxx::Ptr<::jxx::net::InetSocketAddress> DefaultHttpsServer::getAddress(){ return address_; }

} // namespace jxx::com::sun::net::httpserver::internal
