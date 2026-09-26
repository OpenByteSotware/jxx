#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpsServer.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpsConfigurator.h"
#include "lang/jxx.lang.Exceptions.h"
namespace jxx::com::sun::net::httpserver::internal {
DefaultHttpsServer::DefaultHttpsServer():Super(),delegate_(::jxx::NEW<DefaultHttpServer>()){}
DefaultHttpsServer::DefaultHttpsServer(const ::jxx::Ptr<::jxx::net::InetSocketAddress>&a,::jxx::lang::jint b):Super(),delegate_(::jxx::NEW<DefaultHttpServer>(a,b)){}
void DefaultHttpsServer::setHttpsConfigurator(const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsConfigurator>&c){if(!c)throw ::jxx::lang::NullPointerException();configurator_=c;}
::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsConfigurator>DefaultHttpsServer::getHttpsConfigurator(){return configurator_;}
void DefaultHttpsServer::bind(const ::jxx::Ptr<::jxx::net::InetSocketAddress>&a,::jxx::lang::jint b){delegate_->bind(a,b);}void DefaultHttpsServer::start(){if(!configurator_)throw ::jxx::lang::IllegalStateException();delegate_->start();}void DefaultHttpsServer::stop(::jxx::lang::jint d){delegate_->stop(d);}
void DefaultHttpsServer::setExecutor(const ::jxx::Ptr<::jxx::util::concurrent::Executor>&e){delegate_->setExecutor(e);}::jxx::Ptr<::jxx::util::concurrent::Executor>DefaultHttpsServer::getExecutor(){return delegate_->getExecutor();}
::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>DefaultHttpsServer::createContext(const ::jxx::Ptr<::jxx::lang::String>&p,const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpHandler>&h){return delegate_->createContext(p,h);}::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>DefaultHttpsServer::createContext(const ::jxx::Ptr<::jxx::lang::String>&p){return delegate_->createContext(p);}void DefaultHttpsServer::removeContext(const ::jxx::Ptr<::jxx::lang::String>&p){delegate_->removeContext(p);}void DefaultHttpsServer::removeContext(const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>&c){delegate_->removeContext(c);}::jxx::Ptr<::jxx::net::InetSocketAddress>DefaultHttpsServer::getAddress(){return delegate_->getAddress();}
}
