#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"

namespace jxx::ext::net::ssl {

::jxx::Ptr<SSLParameters::StringArray> SSLParameters::copyOf(
    const ::jxx::Ptr<StringArray>& values) {
    if (values == nullptr) return nullptr;
    const auto copy = ::jxx::NEW<StringArray>(values->length);
    for (::jxx::lang::jint index = 0; index < values->length; ++index)
        (*copy)[index] = (*values)[index];
    return copy;
}

SSLParameters::SSLParameters() = default;
SSLParameters::SSLParameters(const ::jxx::Ptr<StringArray>& cipherSuites)
    : ciphers_(copyOf(cipherSuites)) {
}
SSLParameters::SSLParameters(
    const ::jxx::Ptr<StringArray>& cipherSuites,
    const ::jxx::Ptr<StringArray>& protocols)
    : ciphers_(copyOf(cipherSuites))
    , protocols_(copyOf(protocols)) {
}
::jxx::Ptr<SSLParameters::StringArray> SSLParameters::getCipherSuites() const{return copyOf(ciphers_);}void SSLParameters::setCipherSuites(const ::jxx::Ptr<StringArray>&v){ciphers_=copyOf(v);}::jxx::Ptr<SSLParameters::StringArray> SSLParameters::getProtocols()const{return copyOf(protocols_);}void SSLParameters::setProtocols(const ::jxx::Ptr<StringArray>&v){protocols_=copyOf(v);}::jxx::lang::jbool SSLParameters::getNeedClientAuth()const{return need_;}void SSLParameters::setNeedClientAuth(::jxx::lang::jbool v){need_=v;if(v)want_=false;}::jxx::lang::jbool SSLParameters::getWantClientAuth()const{return want_;}void SSLParameters::setWantClientAuth(::jxx::lang::jbool v){want_=v;if(v)need_=false;}

} // namespace jxx::ext::net::ssl
