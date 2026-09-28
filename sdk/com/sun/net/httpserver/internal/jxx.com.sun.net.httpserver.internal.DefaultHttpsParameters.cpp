#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpsParameters.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "lang/jxx.lang.Exceptions.h"
namespace jxx::com::sun::net::httpserver::internal {
DefaultHttpsParameters::DefaultHttpsParameters(const ::jxx::Ptr<::jxx::net::InetSocketAddress>& a,const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsConfigurator>& c):Super(),clientAddress_(a),configurator_(c){if(!a||!c)throw ::jxx::lang::NullPointerException();}
::jxx::Ptr<::jxx::net::InetSocketAddress> DefaultHttpsParameters::getClientAddress(){return clientAddress_;}
::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpsConfigurator> DefaultHttpsParameters::getHttpsConfigurator(){return configurator_;}
::jxx::Ptr<::jxx::ext::net::ssl::SSLParameters> DefaultHttpsParameters::copyParameters_(const ::jxx::Ptr<::jxx::ext::net::ssl::SSLParameters>& source)
{
    if (source == nullptr) return nullptr;
    auto copy = ::jxx::NEW<::jxx::ext::net::ssl::SSLParameters>();
    copy->setCipherSuites(source->getCipherSuites());
    copy->setProtocols(source->getProtocols());
    if (source->getNeedClientAuth()) copy->setNeedClientAuth(true);
    else if (source->getWantClientAuth()) copy->setWantClientAuth(true);
    copy->setEndpointIdentificationAlgorithm(source->getEndpointIdentificationAlgorithm());
    copy->setServerNames(source->getServerNames());
    copy->setSNIMatchers(source->getSNIMatchers());
    copy->setAlgorithmConstraints(source->getAlgorithmConstraints());
    copy->setUseCipherSuitesOrder(source->getUseCipherSuitesOrder());
    return copy;
}
void DefaultHttpsParameters::setSSLParameters(const ::jxx::Ptr<::jxx::ext::net::ssl::SSLParameters>& parameters)
{
    if (parameters == nullptr) throw ::jxx::lang::NullPointerException();
    parameters_ = copyParameters_(parameters);
    setCipherSuites(parameters_->getCipherSuites());
    setProtocols(parameters_->getProtocols());
    if (parameters_->getNeedClientAuth()) setNeedClientAuth(true);
    else if (parameters_->getWantClientAuth()) setWantClientAuth(true);
}
::jxx::Ptr<::jxx::ext::net::ssl::SSLParameters> DefaultHttpsParameters::getAppliedSSLParameters() const
{
    return copyParameters_(parameters_);
}
}
