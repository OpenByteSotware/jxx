#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocket.h"

#include "ext/net/ssl/jxx.ext.net.ssl.SNIHostName.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "util/jxx.util.ArrayList.h"

namespace jxx::ext::net::ssl::internal {

::jxx::Ptr<::jxx::ext::net::ssl::SSLParameters>
OpenSslSocket::getSSLParameters() const {
    const auto parameters = SSLSocket::getSSLParameters();
    parameters->setEndpointIdentificationAlgorithm(
        endpointIdentificationAlgorithm_);

    if (sniHost_ != nullptr) {
        const auto names = ::jxx::NEW<
            ::jxx::util::ArrayList<
                ::jxx::ext::net::ssl::SNIServerName>>();
        names->add(::jxx::NEW<
            ::jxx::ext::net::ssl::SNIHostName>(sniHost_));
        parameters->setServerNames(names);
    }

    return parameters;
}

void OpenSslSocket::setSSLParameters(
    const ::jxx::Ptr<
        ::jxx::ext::net::ssl::SSLParameters>& parameters) {
    if (session_ != nullptr)
        throw ::jxx::lang::IllegalStateException(
            "SSL parameters cannot change after handshake");

    SSLSocket::setSSLParameters(parameters);

    endpointIdentificationAlgorithm_ =
        parameters->getEndpointIdentificationAlgorithm();
    if (endpointIdentificationAlgorithm_ != nullptr &&
        !endpointIdentificationAlgorithm_->utf8().empty() &&
        endpointIdentificationAlgorithm_->utf8() != "HTTPS")
        throw ::jxx::lang::IllegalArgumentException(
            "unsupported endpoint identification algorithm");

    sniHost_ = nullptr;
    const auto names = parameters->getServerNames();
    if (names != nullptr) {
        for (::jxx::lang::jint index = 0;
             index < names->size();
             ++index) {
            const auto host = ::jxx::CAST<
                ::jxx::ext::net::ssl::SNIHostName>(
                    names->get(index));
            if (host != nullptr) {
                sniHost_ = host->getAsciiName();
                break;
            }
        }
    }
}

} // namespace jxx::ext::net::ssl::internal
