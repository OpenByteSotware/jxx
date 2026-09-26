#include "ext/net/ssl/jxx.ext.net.ssl.X509ExtendedKeyManager.h"
namespace jxx::ext::net::ssl {
::jxx::Ptr<::jxx::lang::String>
X509ExtendedKeyManager::chooseEngineClientAlias(
    const ::jxx::Ptr<StringArray>&,
    const ::jxx::Ptr<PrincipalArray>&,
    const ::jxx::Ptr<SSLEngine>&) { return nullptr; }
::jxx::Ptr<::jxx::lang::String>
X509ExtendedKeyManager::chooseEngineServerAlias(
    const ::jxx::Ptr<::jxx::lang::String>&,
    const ::jxx::Ptr<PrincipalArray>&,
    const ::jxx::Ptr<SSLEngine>&) { return nullptr; }
} // namespace jxx::ext::net::ssl
