#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslContextConfig.h"

namespace jxx::ext::net::ssl::internal {

OpenSslContextConfig::OpenSslContextConfig(
    const ::jxx::Ptr<::jxx::lang::String>& protocolValue,
    const ::jxx::Ptr<::jxx::ext::net::ssl::SSLContext::KeyManagerArray>& keyManagerValues,
    const ::jxx::Ptr<::jxx::ext::net::ssl::SSLContext::TrustManagerArray>& trustManagerValues,
    const ::jxx::Ptr<::jxx::security::SecureRandom>& secureRandomValue,
    const ::jxx::Ptr<OpenSslSessionContext>& clientSessionContextValue,
    const ::jxx::Ptr<OpenSslSessionContext>& serverSessionContextValue)
    : protocol(protocolValue)
    , keyManagers(keyManagerValues)
    , trustManagers(trustManagerValues)
    , secureRandom(secureRandomValue)
    , clientSessionContext(clientSessionContextValue)
    , serverSessionContext(serverSessionContextValue) {
}

} // namespace jxx::ext::net::ssl::internal
