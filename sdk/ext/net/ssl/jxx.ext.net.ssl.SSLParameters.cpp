#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"

#include "ext/net/ssl/jxx.ext.net.ssl.SNIServerName.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "util/jxx.util.ArrayList.h"

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

SSLParameters::SSLParameters(
    const ::jxx::Ptr<StringArray>& cipherSuites)
    : ciphers_(copyOf(cipherSuites)) {
}

SSLParameters::SSLParameters(
    const ::jxx::Ptr<StringArray>& cipherSuites,
    const ::jxx::Ptr<StringArray>& protocols)
    : ciphers_(copyOf(cipherSuites))
    , protocols_(copyOf(protocols)) {
}

::jxx::Ptr<SSLParameters::StringArray>
SSLParameters::getCipherSuites() const { return copyOf(ciphers_); }
void SSLParameters::setCipherSuites(const ::jxx::Ptr<StringArray>& values) { ciphers_ = copyOf(values); }
::jxx::Ptr<SSLParameters::StringArray>
SSLParameters::getProtocols() const { return copyOf(protocols_); }
void SSLParameters::setProtocols(const ::jxx::Ptr<StringArray>& values) { protocols_ = copyOf(values); }
::jxx::lang::jbool SSLParameters::getNeedClientAuth() const { return need_; }
void SSLParameters::setNeedClientAuth(::jxx::lang::jbool value) { need_ = value; if (value) want_ = false; }
::jxx::lang::jbool SSLParameters::getWantClientAuth() const { return want_; }
void SSLParameters::setWantClientAuth(::jxx::lang::jbool value) { want_ = value; if (value) need_ = false; }

::jxx::Ptr<::jxx::lang::String>
SSLParameters::getEndpointIdentificationAlgorithm() const {
    return endpointIdentificationAlgorithm_;
}

void SSLParameters::setEndpointIdentificationAlgorithm(
    const ::jxx::Ptr<::jxx::lang::String>& algorithm) {
    endpointIdentificationAlgorithm_ = algorithm;
}

::jxx::Ptr<::jxx::util::List<SNIServerName>>
SSLParameters::getServerNames() const {
    if (serverNames_ == nullptr) return nullptr;
    const auto copy = ::jxx::NEW<::jxx::util::ArrayList<SNIServerName>>();
    for (::jxx::lang::jint index = 0; index < serverNames_->size(); ++index)
        copy->add(serverNames_->get(index));
    return copy;
}

void SSLParameters::setServerNames(
    const ::jxx::Ptr<::jxx::util::List<SNIServerName>>& names) {
    if (names == nullptr) {
        serverNames_ = nullptr;
        return;
    }
    const auto copy = ::jxx::NEW<::jxx::util::ArrayList<SNIServerName>>();
    for (::jxx::lang::jint index = 0; index < names->size(); ++index) {
        const ::jxx::Ptr<SNIServerName> candidate = names->get(index);
        if (candidate == nullptr) throw ::jxx::lang::IllegalArgumentException();
        for (::jxx::lang::jint previous = 0; previous < index; ++previous) {
            const ::jxx::Ptr<SNIServerName> existing = names->get(previous);
            if (existing != nullptr &&
                existing->getType() == candidate->getType())
                throw ::jxx::lang::IllegalArgumentException();
        }
        copy->add(candidate);
    }
    serverNames_ = copy;
}

} // namespace jxx::ext::net::ssl
