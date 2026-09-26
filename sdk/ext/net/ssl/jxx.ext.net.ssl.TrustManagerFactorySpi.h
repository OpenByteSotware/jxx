#pragma once
#include "ext/net/ssl/jxx.ext.net.ssl.TrustManager.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::security { class KeyStore; }
namespace jxx::ext::net::ssl {
class ManagerFactoryParameters;
class TrustManagerFactorySpi
    : public ::jxx::lang::ClassBase<TrustManagerFactorySpi, ::jxx::lang::Object> {
public:
    using TrustManagerArray = ::jxx::lang::JxxArray<::jxx::Ptr<TrustManager>, 1U>;
    ~TrustManagerFactorySpi() override = default;
    virtual ::jxx::Ptr<TrustManagerArray> engineGetTrustManagers() = 0;
    virtual void engineInit(
        const ::jxx::Ptr<::jxx::security::KeyStore>& keyStore) = 0;
    virtual void engineInit(
        const ::jxx::Ptr<ManagerFactoryParameters>& parameters) = 0;
protected:
    TrustManagerFactorySpi() = default;
};
} // namespace jxx::ext::net::ssl
