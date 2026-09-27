#include "ext/net/ssl/jxx.ext.net.ssl.SSLContextSpi.h"

#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocketFactory.h"

namespace jxx::ext::net::ssl {

::jxx::Ptr<SSLParameters>
SSLContextSpi::engineGetDefaultSSLParameters() {
    const auto factory = engineGetSocketFactory();
    const auto parameters = ::jxx::NEW<SSLParameters>();
    parameters->setCipherSuites(factory->getDefaultCipherSuites());
    return parameters;
}

::jxx::Ptr<SSLParameters>
SSLContextSpi::engineGetSupportedSSLParameters() {
    const auto factory = engineGetSocketFactory();
    const auto parameters = ::jxx::NEW<SSLParameters>();
    parameters->setCipherSuites(factory->getSupportedCipherSuites());
    return parameters;
}

} // namespace jxx::ext::net::ssl
