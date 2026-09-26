#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpsParameters.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "lang/jxx.lang.Exceptions.h"
namespace jxx::com::sun::net::httpserver::internal {
DefaultHttpsParameters::DefaultHttpsParameters(const ::jxx::Ptr<::jxx::net::InetSocketAddress>& a,const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsConfigurator>& c):Super(),clientAddress_(a),configurator_(c){if(!a||!c)throw ::jxx::lang::NullPointerException();}
::jxx::Ptr<::jxx::net::InetSocketAddress> DefaultHttpsParameters::getClientAddress(){return clientAddress_;}
::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsConfigurator> DefaultHttpsParameters::getHttpsConfigurator(){return configurator_;}
void DefaultHttpsParameters::setSSLParameters(const ::jxx::Ptr<::jxx::ext::net::ssl::SSLParameters>& p){if(!p)throw ::jxx::lang::NullPointerException();parameters_=p;setCipherSuites(p->getCipherSuites());setProtocols(p->getProtocols());setNeedClientAuth(p->getNeedClientAuth());setWantClientAuth(p->getWantClientAuth());}
::jxx::Ptr<::jxx::ext::net::ssl::SSLParameters> DefaultHttpsParameters::getAppliedSSLParameters()const{return parameters_;}
}
