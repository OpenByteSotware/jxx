#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslKeyStoreKeyManager.h"

#include "security/cert/jxx.security.cert.X509Certificate.h"

namespace jxx::ext::net::ssl::internal {
namespace {

::jxx::lang::jbool issuerMatches(
    const ::jxx::Ptr<
        OpenSslKeyStoreKeyManager::CertificateArray>& chain,
    const ::jxx::Ptr<
        OpenSslKeyStoreKeyManager::PrincipalArray>& issuers)
{
    if (issuers == nullptr || issuers->length == 0) {
        return true;
    }
    if (chain == nullptr || chain->length == 0 || (*chain)[0] == nullptr) {
        return false;
    }

    const auto certificateIssuer = (*chain)[0]->getIssuerDN();
    if (certificateIssuer == nullptr || certificateIssuer->getName() == nullptr) {
        return false;
    }
    const auto certificateIssuerName = certificateIssuer->getName()->utf8();

    for (::jxx::lang::jint index = 0; index < issuers->length; ++index) {
        const auto issuer = (*issuers)[index];
        if (issuer != nullptr && issuer->getName() != nullptr &&
            issuer->getName()->utf8() == certificateIssuerName)
        {
            return true;
        }
    }
    return false;
}

} // namespace

OpenSslKeyStoreKeyManager::OpenSslKeyStoreKeyManager(
    const ::jxx::Ptr<::jxx::security::KeyStore>& store,
    const ::jxx::Ptr<::jxx::security::KeyStore::CharArray>& password)
    : store_(store), password_(password) {
}

::jxx::Ptr<::jxx::lang::String>
OpenSslKeyStoreKeyManager::chooseServerAlias(
    const ::jxx::Ptr<::jxx::lang::String>& keyType,
    const ::jxx::Ptr<PrincipalArray>& issuers,
    const ::jxx::Ptr<::jxx::net::Socket>&)
{
    if (keyType == nullptr) return nullptr;
    const auto aliases = store_->aliases();
    for (::jxx::lang::jint index = 0; index < aliases->length; ++index) {
        const auto alias = (*aliases)[index];
        const auto key = store_->getKey(alias, password_);
        if (key == nullptr || key->getAlgorithm() == nullptr ||
            key->getAlgorithm()->utf8() != keyType->utf8())
        {
            continue;
        }
        const auto chain = getCertificateChain(alias);
        if (issuerMatches(chain, issuers)) return alias;
    }
    return nullptr;
}

::jxx::Ptr<::jxx::lang::String>
OpenSslKeyStoreKeyManager::chooseClientAlias(
    const ::jxx::Ptr<StringArray>& keyTypes,
    const ::jxx::Ptr<PrincipalArray>& issuers,
    const ::jxx::Ptr<::jxx::net::Socket>& socket)
{
    if (keyTypes == nullptr) return nullptr;
    for (::jxx::lang::jint index = 0; index < keyTypes->length; ++index) {
        const auto alias = chooseServerAlias((*keyTypes)[index], issuers, socket);
        if (alias != nullptr) return alias;
    }
    return nullptr;
}

::jxx::Ptr<OpenSslKeyStoreKeyManager::CertificateArray>
OpenSslKeyStoreKeyManager::getCertificateChain(
    const ::jxx::Ptr<::jxx::lang::String>& alias)
{
    const auto chain = store_->getCertificateChain(alias);
    if (chain == nullptr) return nullptr;
    const auto result = ::jxx::NEW<CertificateArray>(chain->length);
    for (::jxx::lang::jint index = 0; index < chain->length; ++index)
        (*result)[index] = ::jxx::CAST<::jxx::security::cert::X509Certificate>((*chain)[index]);
    return result;
}

::jxx::Ptr<OpenSslKeyStoreKeyManager::StringArray>
OpenSslKeyStoreKeyManager::getServerAliases(
    const ::jxx::Ptr<::jxx::lang::String>& keyType,
    const ::jxx::Ptr<PrincipalArray>& issuers)
{
    const auto alias = chooseServerAlias(keyType, issuers, nullptr);
    if (alias == nullptr) return nullptr;
    const auto result = ::jxx::NEW<StringArray>(1);
    (*result)[0] = alias;
    return result;
}

::jxx::Ptr<OpenSslKeyStoreKeyManager::StringArray>
OpenSslKeyStoreKeyManager::getClientAliases(
    const ::jxx::Ptr<::jxx::lang::String>& keyType,
    const ::jxx::Ptr<PrincipalArray>& issuers)
{
    return getServerAliases(keyType, issuers);
}

::jxx::Ptr<::jxx::security::PrivateKey>
OpenSslKeyStoreKeyManager::getPrivateKey(
    const ::jxx::Ptr<::jxx::lang::String>& alias)
{
    return store_->getKey(alias, password_);
}

::jxx::Ptr<::jxx::lang::String>
OpenSslKeyStoreKeyManager::chooseEngineClientAlias(
    const ::jxx::Ptr<StringArray>& keyTypes,
    const ::jxx::Ptr<PrincipalArray>& issuers,
    const ::jxx::Ptr<::jxx::ext::net::ssl::SSLEngine>&)
{
    return chooseClientAlias(keyTypes, issuers, nullptr);
}

::jxx::Ptr<::jxx::lang::String>
OpenSslKeyStoreKeyManager::chooseEngineServerAlias(
    const ::jxx::Ptr<::jxx::lang::String>& keyType,
    const ::jxx::Ptr<PrincipalArray>& issuers,
    const ::jxx::Ptr<::jxx::ext::net::ssl::SSLEngine>&)
{
    return chooseServerAlias(keyType, issuers, nullptr);
}

} // namespace jxx::ext::net::ssl::internal
