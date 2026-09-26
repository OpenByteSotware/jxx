#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpsServer.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpsConfigurator.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLServerSocketFactory.h"
#include "lang/jxx.lang.Exceptions.h"
#include "net/jxx.net.InetSocketAddress.h"
namespace jxx::com::sun::net::httpserver::internal {
DefaultHttpsServer::DefaultHttpsServer():Super(){}
DefaultHttpsServer::DefaultHttpsServer(const ::jxx::Ptr<::jxx::net::InetSocketAddress>&a,::jxx::lang::jint b):Super(),address_(a),backlog_(b<=0?50:b){if(!a)throw ::jxx::lang::NullPointerException();}
DefaultHttpsServer::~DefaultHttpsServer(){try{stop(0);}catch(...) {}}
void DefaultHttpsServer::setHttpsConfigurator(const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsConfigurator>&c){if(!c)throw ::jxx::lang::NullPointerException();if(started_)throw ::jxx::lang::IllegalStateException();configurator_=c;}
::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsConfigurator>DefaultHttpsServer::getHttpsConfigurator(){return configurator_;}
void DefaultHttpsServer::bind(const ::jxx::Ptr<::jxx::net::InetSocketAddress>&a,::jxx::lang::jint b){if(!a)throw ::jxx::lang::NullPointerException();if(started_||address_)throw ::jxx::lang::IllegalStateException();address_=a;backlog_=b<=0?50:b;}
void DefaultHttpsServer::rebuildTlsDelegate_(){if(!configurator_||!address_)throw ::jxx::lang::IllegalStateException();auto factory=configurator_->getSSLContext()->getServerSocketFactory();auto listener=factory->createServerSocket(address_->getPort(),backlog_,address_->getAddress());delegate_=::jxx::NEW<DefaultHttpServer>(listener,address_);}
void DefaultHttpsServer::start(){if(started_)throw ::jxx::lang::IllegalStateException();rebuildTlsDelegate_();started_=true;delegate_->start();}
void DefaultHttpsServer::stop(::jxx::lang::jint d){if(d<0)throw ::jxx::lang::IllegalArgumentException();if(delegate_)delegate_->stop(d);}
void DefaultHttpsServer::setExecutor(const ::jxx::Ptr<::jxx::util::concurrent::Executor>&e){if(started_)throw ::jxx::lang::IllegalStateException();if(!delegate_)delegate_=::jxx::NEW<DefaultHttpServer>();delegate_->setExecutor(e);}
::jxx::Ptr<::jxx::util::concurrent::Executor>DefaultHttpsServer::getExecutor(){return delegate_?delegate_->getExecutor():nullptr;}
::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>DefaultHttpsServer::createContext(const ::jxx::Ptr<::jxx::lang::String>&p,const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpHandler>&h){if(!delegate_)delegate_=::jxx::NEW<DefaultHttpServer>();return delegate_->createContext(p,h);}
::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>DefaultHttpsServer::createContext(const ::jxx::Ptr<::jxx::lang::String>&p){if(!delegate_)delegate_=::jxx::NEW<DefaultHttpServer>();return delegate_->createContext(p);}
void DefaultHttpsServer::removeContext(const ::jxx::Ptr<::jxx::lang::String>&p){if(!delegate_)throw ::jxx::lang::IllegalArgumentException();delegate_->removeContext(p);}
void DefaultHttpsServer::removeContext(const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpContext>&c){if(!delegate_)throw ::jxx::lang::IllegalArgumentException();delegate_->removeContext(c);}
::jxx::Ptr<::jxx::net::InetSocketAddress>DefaultHttpsServer::getAddress(){return address_;}
}
