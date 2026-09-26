#pragma once
#include "ext/net/ssl/jxx.ext.net.ssl.KeyManager.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::security { class KeyStore; }
namespace jxx::ext::net::ssl {
class ManagerFactoryParameters;
class KeyManagerFactorySpi
    : public ::jxx::lang::ClassBase<KeyManagerFactorySpi, ::jxx::lang::Object> {
public:
    using KeyManagerArray = ::jxx::lang::JxxArray<::jxx::Ptr<KeyManager>, 1U>;
    using CharArray = ::jxx::lang::JxxArray<::jxx::lang::jchar, 1U>;
    ~KeyManagerFactorySpi() override = default;
    virtual ::jxx::Ptr<KeyManagerArray> engineGetKeyManagers() = 0;
    virtual void engineInit(
        const ::jxx::Ptr<::jxx::security::KeyStore>& keyStore,
        const ::jxx::Ptr<CharArray>& password) = 0;
    virtual void engineInit(
        const ::jxx::Ptr<ManagerFactoryParameters>& parameters) = 0;
protected:
    KeyManagerFactorySpi() = default;
};
} // namespace jxx::ext::net::ssl
