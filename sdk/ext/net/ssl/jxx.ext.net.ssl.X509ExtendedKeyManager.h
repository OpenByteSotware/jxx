#pragma once

#include "ext/net/ssl/jxx.ext.net.ssl.X509KeyManager.h"

namespace jxx::ext::net::ssl {
class SSLEngine;
class X509ExtendedKeyManager
    : public ::jxx::lang::ClassBase<
          X509ExtendedKeyManager,
          ::jxx::lang::Object,
          X509KeyManager> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        X509ExtendedKeyManager, JxxSuper, X509KeyManager>;
    ~X509ExtendedKeyManager() override = default;
    virtual ::jxx::Ptr<::jxx::lang::String> chooseEngineClientAlias(
        const ::jxx::Ptr<StringArray>& keyTypes,
        const ::jxx::Ptr<PrincipalArray>& issuers,
        const ::jxx::Ptr<SSLEngine>& engine);
    virtual ::jxx::Ptr<::jxx::lang::String> chooseEngineServerAlias(
        const ::jxx::Ptr<::jxx::lang::String>& keyType,
        const ::jxx::Ptr<PrincipalArray>& issuers,
        const ::jxx::Ptr<SSLEngine>& engine);
protected:
    X509ExtendedKeyManager() = default;
};
} // namespace jxx::ext::net::ssl
