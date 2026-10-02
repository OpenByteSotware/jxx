#include "ext/net/ssl/jxx.ext.net.ssl.KeyManagerFactory.h"

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslKeyManagerFactorySpi.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "security/jxx.security.NoSuchAlgorithmException.h"
#include "security/jxx.security.NoSuchProviderException.h"

namespace jxx::ext::net::ssl {
namespace {

void validateAlgorithm(const ::jxx::Ptr<::jxx::lang::String>& algorithm) {
    if (algorithm == nullptr) throw ::jxx::lang::NullPointerException();
    const auto value = algorithm->utf8();
    if (value != "SunX509" && value != "NewSunX509" &&
        value != "PKIX" && value != "OpenSSL")
        throw ::jxx::security::NoSuchAlgorithmException(
            "KeyManagerFactory algorithm is not available");
}

void validateProviderName(const ::jxx::Ptr<::jxx::lang::String>& provider) {
    if (provider == nullptr || provider->utf8().empty())
        throw ::jxx::lang::IllegalArgumentException();
    if (provider->utf8() != "OpenSSL")
        throw ::jxx::security::NoSuchProviderException(
            "KeyManagerFactory provider is not installed");
}

void validateProvider(const ::jxx::Ptr<::jxx::security::Provider>& provider) {
    if (provider == nullptr) throw ::jxx::lang::IllegalArgumentException();
    const auto name = provider->getName();
    if (name == nullptr || name->utf8() != "OpenSSL")
        throw ::jxx::security::NoSuchAlgorithmException(
            "Provider does not implement the requested KeyManagerFactory algorithm");
}

::jxx::Ptr<::jxx::security::Provider> openSslProvider() {
    return ::jxx::NEW<::jxx::security::Provider>(
        ::jxx::NEW<::jxx::lang::String>("OpenSSL"));
}

::jxx::Ptr<KeyManagerFactory> newFactory(
    const ::jxx::Ptr<::jxx::lang::String>& algorithm,
    const ::jxx::Ptr<::jxx::security::Provider>& provider) {
    return ::jxx::NEW<KeyManagerFactory>(
        ::jxx::NEW<internal::OpenSslKeyManagerFactorySpi>(),
        provider,
        algorithm);
}

} // namespace

KeyManagerFactory::KeyManagerFactory(
    const ::jxx::Ptr<KeyManagerFactorySpi>& spi,
    const ::jxx::Ptr<::jxx::security::Provider>& provider,
    const ::jxx::Ptr<::jxx::lang::String>& algorithm)
    : spi_(spi), provider_(provider), algorithm_(algorithm) {
    if (spi_ == nullptr || provider_ == nullptr || algorithm_ == nullptr)
        throw ::jxx::lang::NullPointerException();
}

::jxx::Ptr<::jxx::lang::String> KeyManagerFactory::getDefaultAlgorithm() {
    return ::jxx::NEW<::jxx::lang::String>("SunX509");
}

::jxx::Ptr<KeyManagerFactory> KeyManagerFactory::getInstance(
    const ::jxx::Ptr<::jxx::lang::String>& algorithm) {
    validateAlgorithm(algorithm);
    return newFactory(algorithm, openSslProvider());
}

::jxx::Ptr<KeyManagerFactory> KeyManagerFactory::getInstance(
    const ::jxx::Ptr<::jxx::lang::String>& algorithm,
    const ::jxx::Ptr<::jxx::lang::String>& provider) {
    validateAlgorithm(algorithm);
    validateProviderName(provider);
    return newFactory(algorithm, openSslProvider());
}

::jxx::Ptr<KeyManagerFactory> KeyManagerFactory::getInstance(
    const ::jxx::Ptr<::jxx::lang::String>& algorithm,
    const ::jxx::Ptr<::jxx::security::Provider>& provider) {
    validateAlgorithm(algorithm);
    validateProvider(provider);
    return newFactory(algorithm, provider);
}

::jxx::Ptr<::jxx::lang::String> KeyManagerFactory::getAlgorithm() const { return algorithm_; }
::jxx::Ptr<::jxx::security::Provider> KeyManagerFactory::getProvider() const { return provider_; }
::jxx::Ptr<KeyManagerFactory::KeyManagerArray> KeyManagerFactory::getKeyManagers() { return spi_->engineGetKeyManagers(); }
void KeyManagerFactory::init(const ::jxx::Ptr<::jxx::security::KeyStore>& store, const ::jxx::Ptr<CharArray>& password) { spi_->engineInit(store, password); }
void KeyManagerFactory::init(const ::jxx::Ptr<ManagerFactoryParameters>& parameters) { spi_->engineInit(parameters); }

} // namespace jxx::ext::net::ssl
