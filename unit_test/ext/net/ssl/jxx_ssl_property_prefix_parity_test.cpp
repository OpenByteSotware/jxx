#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <string>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPropertyKeyManager.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPropertyTrustManager.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "security/jxx.security.KeyStoreException.h"

namespace {

class PropertyRestore final {
public:
    explicit PropertyRestore(const char* name)
        : name_(::jxx::NEW<::jxx::lang::String>(name)),
          previous_(::jxx::lang::System::getProperty(name_)) {
    }

    ~PropertyRestore() {
        try {
            if (previous_ == nullptr) {
                (void)::jxx::lang::System::clearProperty(name_);
            }
            else {
                (void)::jxx::lang::System::setProperty(
                    name_,
                    previous_);
            }
        }
        catch (...) {
        }
    }

private:
    ::jxx::Ptr<::jxx::lang::String> name_;
    ::jxx::Ptr<::jxx::lang::String> previous_;
};

class TemporaryFile final {
public:
    explicit TemporaryFile(const char* path)
        : path_(path) {
        std::ofstream stream(path_, std::ios::binary);
        if (stream.good()) {
            stream.put('\0');
        }
    }

    ~TemporaryFile() {
        (void)std::remove(path_.c_str());
    }

    const std::string& path() const {
        return path_;
    }

    bool exists() const {
        std::ifstream stream(path_, std::ios::binary);
        return stream.good();
    }

private:
    std::string path_;
};

void setProperty(const char* name, const char* value) {
    (void)::jxx::lang::System::setProperty(
        ::jxx::NEW<::jxx::lang::String>(name),
        ::jxx::NEW<::jxx::lang::String>(value));
}

TEST(
    SSLPropertyPrefixParity,
    MissingTranslatedTrustStoreReturnsEmptyManagers)
{
    PropertyRestore path(
        "jxx.ext.net.ssl.trustStore");

    setProperty(
        "jxx.ext.net.ssl.trustStore",
        "jxx-missing-trust-store.pem");

    const auto managers =
        ::jxx::ext::net::ssl::internal::
            loadDefaultPropertyTrustManagers();

    ASSERT_NE(nullptr, managers);
    ASSERT_EQ(1U, managers->length);
    EXPECT_NE(nullptr, (*managers)[0]);
}

TEST(
    SSLPropertyPrefixParity,
    TranslatedTrustStoreTypeIsObserved)
{
    PropertyRestore path(
        "jxx.ext.net.ssl.trustStore");

    PropertyRestore type(
        "jxx.ext.net.ssl.trustStoreType");

    const TemporaryFile temporaryStore(
        "jxx-trust-store-type-validation.tmp");

    ASSERT_TRUE(temporaryStore.exists());

    setProperty(
        "jxx.ext.net.ssl.trustStore",
        temporaryStore.path().c_str());

    setProperty(
        "jxx.ext.net.ssl.trustStoreType",
        "JKS");

    EXPECT_THROW(
        ::jxx::ext::net::ssl::internal::
            loadDefaultPropertyTrustManagers(),
        ::jxx::security::KeyStoreException);
}

} // namespace
