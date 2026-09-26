#pragma once

#include "ext/net/ssl/jxx.ext.net.ssl.TrustManagerFactorySpi.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "security/jxx.security.Provider.h"

namespace jxx::ext::net::ssl {

class TrustManagerFactory final
    : public ::jxx::lang::ClassBase<
          TrustManagerFactory,
          ::jxx::lang::Object> {
public:
    using TrustManagerArray =
        TrustManagerFactorySpi::TrustManagerArray;

    static ::jxx::Ptr<::jxx::lang::String>
    getDefaultAlgorithm();

    static ::jxx::Ptr<TrustManagerFactory>
    getInstance(
        const ::jxx::Ptr<::jxx::lang::String>& algorithm);

    static ::jxx::Ptr<TrustManagerFactory>
    getInstance(
        const ::jxx::Ptr<::jxx::lang::String>& algorithm,
        const ::jxx::Ptr<::jxx::lang::String>& provider);

    static ::jxx::Ptr<TrustManagerFactory>
    getInstance(
        const ::jxx::Ptr<::jxx::lang::String>& algorithm,
        const ::jxx::Ptr<::jxx::security::Provider>& provider);

    TrustManagerFactory(
        const ::jxx::Ptr<TrustManagerFactorySpi>& spi,
        const ::jxx::Ptr<::jxx::security::Provider>& provider,
        const ::jxx::Ptr<::jxx::lang::String>& algorithm);

    ::jxx::Ptr<::jxx::lang::String>
    getAlgorithm() const;

    ::jxx::Ptr<::jxx::security::Provider>
    getProvider() const;

    ::jxx::Ptr<TrustManagerArray>
    getTrustManagers();

    void init(
        const ::jxx::Ptr<::jxx::security::KeyStore>& keyStore);

    void init(
        const ::jxx::Ptr<ManagerFactoryParameters>& parameters);

private:
    ::jxx::Ptr<TrustManagerFactorySpi> spi_;
    ::jxx::Ptr<::jxx::security::Provider> provider_;
    ::jxx::Ptr<::jxx::lang::String> algorithm_;
};

} // namespace jxx::ext::net::ssl
