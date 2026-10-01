#include <openssl/ssl.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslServerSocket.h"

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslContextConfig.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocket.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslCipherSuites.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslProtocolPolicy.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "io/jxx.io.IOException.h"

namespace jxx::ext::net::ssl::internal {
namespace {
::jxx::Ptr<OpenSslServerSocket::StringArray> toArray(const std::vector<std::string>& values){auto r=::jxx::NEW<OpenSslServerSocket::StringArray>(static_cast<::jxx::lang::jint>(values.size()));for(std::size_t i=0;i<values.size();++i)(*r)[static_cast<::jxx::lang::jint>(i)]=::jxx::NEW<::jxx::lang::String>(values[i]);return r;}
std::vector<std::string> toVector(const ::jxx::Ptr<OpenSslServerSocket::StringArray>& values){if(values==nullptr)throw ::jxx::lang::IllegalArgumentException();std::vector<std::string> r;for(::jxx::lang::jint i=0;i<values->length;++i){if((*values)[i]==nullptr)throw ::jxx::lang::IllegalArgumentException();r.push_back((*values)[i]->utf8());}return r;}
}
OpenSslServerSocket::OpenSslServerSocket(
    ::jxx::lang::jint port, ::jxx::lang::jint backlog,
    const ::jxx::Ptr<::jxx::net::InetAddress>& address,
    const std::shared_ptr<OpenSslContextConfig>& config)
    : Super(port, backlog, address), config_(config),
      enabledProtocols_(contextProtocolNames(config == nullptr ? nullptr : config->protocol)) {
    const auto suites = getSupportedCipherSuites();
    for (::jxx::lang::jint index = 0; index < suites->length; ++index)
        enabledCipherSuites_.push_back((*suites)[index]->utf8());
}
::jxx::Ptr<::jxx::net::Socket>
OpenSslServerSocket::accept() {
    const auto transport = ::jxx::net::ServerSocket::accept();
    const auto socket = ::jxx::NEW<OpenSslSocket>(
        transport,
        ::jxx::NEW<::jxx::lang::String>(""),
        transport->getPort(),
        true,
        std::vector<unsigned char>(),
        config_);
    socket->setUseClientMode(useClientMode_);
    socket->setEnableSessionCreation(enableSessionCreation_);
    socket->setSSLParameters(getSSLParameters());
    return socket;
}
::jxx::Ptr<OpenSslServerSocket::StringArray> OpenSslServerSocket::getEnabledCipherSuites()const{return toArray(enabledCipherSuites_);}void OpenSslServerSocket::setEnabledCipherSuites(
    const ::jxx::Ptr<StringArray>& values) {
    const auto suites = toVector(values);
    validateEnabledCipherSuites(suites);
    enabledCipherSuites_ = suites;
}::jxx::Ptr<OpenSslServerSocket::StringArray>
OpenSslServerSocket::getSupportedCipherSuites() const {
    return serverSupportedCipherSuites();
}::jxx::Ptr<OpenSslServerSocket::StringArray> OpenSslServerSocket::getEnabledProtocols()const{return toArray(enabledProtocols_);}void OpenSslServerSocket::setEnabledProtocols(const ::jxx::Ptr<StringArray>&v){const auto values=toVector(v);(void)enabledProtocolRange(values);enabledProtocols_=values;}::jxx::Ptr<OpenSslServerSocket::StringArray> OpenSslServerSocket::getSupportedProtocols()const{return toArray(supportedProtocolNames());}
void OpenSslServerSocket::setNeedClientAuth(::jxx::lang::jbool v){needClientAuth_=v;if(v)wantClientAuth_=false;}::jxx::lang::jbool OpenSslServerSocket::getNeedClientAuth()const{return needClientAuth_;}void OpenSslServerSocket::setWantClientAuth(::jxx::lang::jbool v){wantClientAuth_=v;if(v)needClientAuth_=false;}::jxx::lang::jbool OpenSslServerSocket::getWantClientAuth()const{return wantClientAuth_;}void OpenSslServerSocket::setUseClientMode(::jxx::lang::jbool v){useClientMode_=v;}::jxx::lang::jbool OpenSslServerSocket::getUseClientMode()const{return useClientMode_;}void OpenSslServerSocket::setEnableSessionCreation(::jxx::lang::jbool v){enableSessionCreation_=v;}::jxx::lang::jbool OpenSslServerSocket::getEnableSessionCreation()const{return enableSessionCreation_;}
::jxx::Ptr<::jxx::ext::net::ssl::SSLParameters>
OpenSslServerSocket::getSSLParameters() const {
    const auto parameters = SSLServerSocket::getSSLParameters();
    parameters->setAlgorithmConstraints(algorithmConstraints_);
    parameters->setSNIMatchers(sniMatchers_);
    parameters->setUseCipherSuitesOrder(useCipherSuitesOrder_);
    return parameters;
}

void OpenSslServerSocket::setSSLParameters(
    const ::jxx::Ptr<::jxx::ext::net::ssl::SSLParameters>& parameters) {
    SSLServerSocket::setSSLParameters(parameters);
    algorithmConstraints_ = parameters->getAlgorithmConstraints();
    sniMatchers_ = parameters->getSNIMatchers();
    useCipherSuitesOrder_ = parameters->getUseCipherSuitesOrder();
}

}
