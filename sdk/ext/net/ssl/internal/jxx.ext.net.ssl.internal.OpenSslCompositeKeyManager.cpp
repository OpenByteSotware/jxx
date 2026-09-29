#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslCompositeKeyManager.h"

#include <string>
#include <vector>

namespace jxx::ext::net::ssl::internal {
namespace {

std::string compositeAlias(std::size_t managerIndex, const std::string& alias) {
    return "jxx-builder-" + std::to_string(managerIndex) + ":" + alias;
}

bool parseCompositeAlias(
    const ::jxx::Ptr<::jxx::lang::String>& alias,
    std::size_t& managerIndex,
    ::jxx::Ptr<::jxx::lang::String>& childAlias) {
    if (alias == nullptr) return false;
    const std::string value = alias->utf8();
    const std::string prefix = "jxx-builder-";
    if (value.rfind(prefix, 0) != 0) return false;
    const auto colon = value.find(':', prefix.size());
    if (colon == std::string::npos) return false;
    try {
        managerIndex = static_cast<std::size_t>(
            std::stoul(value.substr(prefix.size(), colon - prefix.size())));
    } catch (...) { return false; }
    childAlias = ::jxx::NEW<::jxx::lang::String>(value.substr(colon + 1));
    return true;
}

::jxx::Ptr<OpenSslCompositeKeyManager::StringArray> collectAliases(
    const std::vector<OpenSslCompositeKeyManager::Manager>& managers,
    const ::jxx::Ptr<::jxx::lang::String>& keyType,
    const ::jxx::Ptr<OpenSslCompositeKeyManager::PrincipalArray>& issuers,
    bool client) {
    std::vector<::jxx::Ptr<::jxx::lang::String>> values;
    for (std::size_t index = 0; index < managers.size(); ++index) {
        const auto aliases = client
            ? managers[index]->getClientAliases(keyType, issuers)
            : managers[index]->getServerAliases(keyType, issuers);
        if (aliases == nullptr) continue;
        for (::jxx::lang::jint aliasIndex = 0; aliasIndex < aliases->length; ++aliasIndex)
            if ((*aliases)[aliasIndex] != nullptr)
                values.push_back(::jxx::NEW<::jxx::lang::String>(
                    compositeAlias(index, (*aliases)[aliasIndex]->utf8())));
    }
    if (values.empty()) return nullptr;
    const auto result = ::jxx::NEW<OpenSslCompositeKeyManager::StringArray>(
        static_cast<::jxx::lang::jint>(values.size()));
    for (std::size_t index = 0; index < values.size(); ++index)
        (*result)[static_cast<::jxx::lang::jint>(index)] = values[index];
    return result;
}

} // namespace

OpenSslCompositeKeyManager::OpenSslCompositeKeyManager(
    const std::vector<Manager>& managers) : managers_(managers) {}

::jxx::Ptr<::jxx::lang::String> OpenSslCompositeKeyManager::chooseClientAlias(
    const ::jxx::Ptr<StringArray>& types, const ::jxx::Ptr<PrincipalArray>& issuers,
    const ::jxx::Ptr<::jxx::net::Socket>& socket) {
    for (std::size_t index=0; index<managers_.size(); ++index) { const auto alias=managers_[index]->chooseClientAlias(types,issuers,socket); if(alias!=nullptr)return ::jxx::NEW<::jxx::lang::String>(compositeAlias(index,alias->utf8())); } return nullptr;
}
::jxx::Ptr<::jxx::lang::String> OpenSslCompositeKeyManager::chooseServerAlias(const ::jxx::Ptr<::jxx::lang::String>&type,const ::jxx::Ptr<PrincipalArray>&issuers,const ::jxx::Ptr<::jxx::net::Socket>&socket){for(std::size_t i=0;i<managers_.size();++i){const auto a=managers_[i]->chooseServerAlias(type,issuers,socket);if(a!=nullptr)return ::jxx::NEW<::jxx::lang::String>(compositeAlias(i,a->utf8()));}return nullptr;}
::jxx::Ptr<OpenSslCompositeKeyManager::CertificateArray> OpenSslCompositeKeyManager::getCertificateChain(const ::jxx::Ptr<::jxx::lang::String>&alias){std::size_t i=0;::jxx::Ptr<::jxx::lang::String>a;if(parseCompositeAlias(alias,i,a)&&i<managers_.size())return managers_[i]->getCertificateChain(a);for(const auto&m:managers_){const auto c=m->getCertificateChain(alias);if(c!=nullptr)return c;}return nullptr;}
::jxx::Ptr<::jxx::security::PrivateKey> OpenSslCompositeKeyManager::getPrivateKey(const ::jxx::Ptr<::jxx::lang::String>&alias){std::size_t i=0;::jxx::Ptr<::jxx::lang::String>a;if(parseCompositeAlias(alias,i,a)&&i<managers_.size())return managers_[i]->getPrivateKey(a);for(const auto&m:managers_){const auto k=m->getPrivateKey(alias);if(k!=nullptr)return k;}return nullptr;}
::jxx::Ptr<OpenSslCompositeKeyManager::StringArray> OpenSslCompositeKeyManager::getClientAliases(const ::jxx::Ptr<::jxx::lang::String>&t,const ::jxx::Ptr<PrincipalArray>&i){return collectAliases(managers_,t,i,true);}
::jxx::Ptr<OpenSslCompositeKeyManager::StringArray> OpenSslCompositeKeyManager::getServerAliases(const ::jxx::Ptr<::jxx::lang::String>&t,const ::jxx::Ptr<PrincipalArray>&i){return collectAliases(managers_,t,i,false);}
::jxx::Ptr<::jxx::lang::String> OpenSslCompositeKeyManager::chooseEngineClientAlias(const ::jxx::Ptr<StringArray>&t,const ::jxx::Ptr<PrincipalArray>&i,const ::jxx::Ptr<::jxx::ext::net::ssl::SSLEngine>&e){for(std::size_t n=0;n<managers_.size();++n){const auto a=managers_[n]->chooseEngineClientAlias(t,i,e);if(a!=nullptr)return ::jxx::NEW<::jxx::lang::String>(compositeAlias(n,a->utf8()));}return nullptr;}
::jxx::Ptr<::jxx::lang::String> OpenSslCompositeKeyManager::chooseEngineServerAlias(const ::jxx::Ptr<::jxx::lang::String>&t,const ::jxx::Ptr<PrincipalArray>&i,const ::jxx::Ptr<::jxx::ext::net::ssl::SSLEngine>&e){for(std::size_t n=0;n<managers_.size();++n){const auto a=managers_[n]->chooseEngineServerAlias(t,i,e);if(a!=nullptr)return ::jxx::NEW<::jxx::lang::String>(compositeAlias(n,a->utf8()));}return nullptr;}
}
