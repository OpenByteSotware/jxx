#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpsServer.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpsConfigurator.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLServerSocketFactory.h"
#include "lang/jxx.lang.Exceptions.h"
#include "net/jxx.net.InetSocketAddress.h"
namespace jxx::com::sun::net::httpserver::internal {
DefaultHttpsServer::DefaultHttpsServer():Super(),delegate_(nullptr){}
DefaultHttpsServer::DefaultHttpsServer(const ::jxx::Ptr<::jxx::net::InetSocketAddress>&a,::jxx::lang::jint b):Super(),delegate_(nullptr){address_=a;backlog_=b<=0?50:b;}
void DefaultHttpsServer::setHttpsConfigurator(const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsConfigurator>&c){if(!c)throw ::jxx::lang::NullPointerException();configurator_=c;}
::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsConfigurator>DefaultHttpsServer::getHttpsConfigurator(){return configurator_;}
void DefaultHttpsServer::bind(const ::jxx::Ptr<::jxx::net::InetSocketAddress>&a,::jxx::lang::jint b){if(!a)throw ::jxx::lang::NullPointerException();address_=a;backlog_=b<=0?50:b;}
void DefaultHttpsServer::start(){if(!configurator_||!address_)throw ::jxx::lang::IllegalStateException();auto factory=configurator_->getSSLContext()->getServerSocketFactory();auto listener=factory->createServerSocket(address_->getPort(),backlog_,address_->getAddress());auto old=delegate_;delegate_=::jxx::NEW<DefaultHttpServer>(listener,address_,configurator_);if(old){delegate_->setExecutor(old->getExecutor());}delegate_->start();}
void DefaultHttpsServer::stop(::jxx::lang::jint d){if(delegate_)delegate_->stop(d);}
void DefaultHttpsServer::setExecutor(const ::jxx::Ptr<::jxx::util::concurrent::Executor>&e){if(!delegate_)delegate_=::jxx::NEW<DefaultHttpServer>();delegate_->setExecutor(e);}
::jxx::Ptr<::jxx::util::concurrent::Executor>DefaultHttpsServer::getExecutor(){return delegate_?delegate_->getExecutor():nullptr;}
::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>DefaultHttpsServer::createContext(const ::jxx::Ptr<::jxx::lang::String>&p,const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpHandler>&h){if(!delegate_)delegate_=::jxx::NEW<DefaultHttpServer>();return delegate_->createContext(p,h);}
::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>DefaultHttpsServer::createContext(const ::jxx::Ptr<::jxx::lang::String>&p){if(!delegate_)delegate_=::jxx::NEW<DefaultHttpServer>();return delegate_->createContext(p);}
void DefaultHttpsServer::removeContext(const ::jxx::Ptr<::jxx::lang::String>&p){delegate_->removeContext(p);}
void DefaultHttpsServer::removeContext(const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>&c){delegate_->removeContext(c);}
::jxx::Ptr<::jxx::net::InetSocketAddress>DefaultHttpsServer::getAddress(){return address_;}
}
