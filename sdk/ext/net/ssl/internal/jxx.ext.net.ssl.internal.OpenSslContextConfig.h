#pragma once

#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"

namespace jxx::ext::net::ssl::internal {

class OpenSslContextConfig final {
public:
    OpenSslContextConfig(
        const ::jxx::Ptr<::jxx::lang::String>& protocol,
        const ::jxx::Ptr<::jxx::ext::net::ssl::SSLContext::KeyManagerArray>& keyManagers,
        const ::jxx::Ptr<::jxx::ext::net::ssl::SSLContext::TrustManagerArray>& trustManagers,
        const ::jxx::Ptr<::jxx::security::SecureRandom>& secureRandom);

    ::jxx::Ptr<::jxx::lang::String> protocol;
    ::jxx::Ptr<::jxx::ext::net::ssl::SSLContext::KeyManagerArray> keyManagers;
    ::jxx::Ptr<::jxx::ext::net::ssl::SSLContext::TrustManagerArray> trustManagers;
    ::jxx::Ptr<::jxx::security::SecureRandom> secureRandom;
};

} // namespace jxx::ext::net::ssl::internal
