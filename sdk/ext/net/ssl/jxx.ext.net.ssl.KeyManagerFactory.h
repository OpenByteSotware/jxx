#pragma once

#include "ext/net/ssl/jxx.ext.net.ssl.KeyManagerFactorySpi.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "security/jxx.security.Provider.h"

namespace jxx::ext::net::ssl {

class KeyManagerFactory final
    : public ::jxx::lang::ClassBase<
          KeyManagerFactory,
          ::jxx::lang::Object> {
public:
    using KeyManagerArray =
        KeyManagerFactorySpi::KeyManagerArray;
    using CharArray =
        KeyManagerFactorySpi::CharArray;

    static ::jxx::Ptr<::jxx::lang::String>
    getDefaultAlgorithm();

    static ::jxx::Ptr<KeyManagerFactory>
    getInstance(
        const ::jxx::Ptr<::jxx::lang::String>& algorithm);

    static ::jxx::Ptr<KeyManagerFactory>
    getInstance(
        const ::jxx::Ptr<::jxx::lang::String>& algorithm,
        const ::jxx::Ptr<::jxx::lang::String>& provider);

    static ::jxx::Ptr<KeyManagerFactory>
    getInstance(
        const ::jxx::Ptr<::jxx::lang::String>& algorithm,
        const ::jxx::Ptr<::jxx::security::Provider>& provider);

    KeyManagerFactory(
        const ::jxx::Ptr<KeyManagerFactorySpi>& spi,
        const ::jxx::Ptr<::jxx::security::Provider>& provider,
        const ::jxx::Ptr<::jxx::lang::String>& algorithm);

    ::jxx::Ptr<::jxx::lang::String>
    getAlgorithm() const;

    ::jxx::Ptr<::jxx::security::Provider>
    getProvider() const;

    ::jxx::Ptr<KeyManagerArray>
    getKeyManagers();

    void init(
        const ::jxx::Ptr<::jxx::security::KeyStore>& keyStore,
        const ::jxx::Ptr<CharArray>& password);

    void init(
        const ::jxx::Ptr<ManagerFactoryParameters>& parameters);

private:
    ::jxx::Ptr<KeyManagerFactorySpi> spi_;
    ::jxx::Ptr<::jxx::security::Provider> provider_;
    ::jxx::Ptr<::jxx::lang::String> algorithm_;
};

} // namespace jxx::ext::net::ssl
