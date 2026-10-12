#include <gtest/gtest.h>
#include <cstdio>
#include <fstream>
#include <string>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPropertyKeyManager.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPropertyTrustManager.h"
#include "io/jxx.io.FileNotFoundException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "security/jxx.security.KeyStoreException.h"
#include "security/jxx.security.NoSuchProviderException.h"

namespace {
class PropertyRestore final {
public:
    explicit PropertyRestore(const char* name)
        : name_(::jxx::NEW<::jxx::lang::String>(name)),
          value_(::jxx::lang::System::getProperty(name_)) {}
    ~PropertyRestore() {
        if (value_ == nullptr) ::jxx::lang::System::clearProperty(name_);
        else ::jxx::lang::System::setProperty(name_, value_);
    }
private:
    ::jxx::Ptr<::jxx::lang::String> name_;
    ::jxx::Ptr<::jxx::lang::String> value_;
};
void setProperty(const char* name, const char* value) {
    ::jxx::lang::System::setProperty(
        ::jxx::NEW<::jxx::lang::String>(name),
        ::jxx::NEW<::jxx::lang::String>(value));
}

class TemporaryFile final
{
public:
    explicit TemporaryFile(const char* path)
        : path_(path)
    {
        std::ofstream stream(path_, std::ios::binary);
        if (stream.good()) {
            stream.put('\0');
        }
    }

    ~TemporaryFile()
    {
        (void)std::remove(path_.c_str());
    }

    const std::string& path() const
    {
        return path_;
    }

    bool exists() const
    {
        std::ifstream stream(path_, std::ios::binary);
        return stream.good();
    }

private:
    std::string path_;
};

}
TEST(
    StoreExceptionFidelityTest,
    UnsupportedKeyStoreTypeThrowsKeyStoreException)
{
    PropertyRestore path(
        "jxx.ext.net.ssl.keyStore");

    PropertyRestore type(
        "jxx.ext.net.ssl.keyStoreType");

    setProperty(
        "jxx.ext.net.ssl.keyStore",
        "missing-store");

    setProperty(
        "jxx.ext.net.ssl.keyStoreType",
        "JKS");

    EXPECT_THROW(
        ::jxx::ext::net::ssl::internal::loadDefaultPropertyKeyManagers(), ::jxx::security::KeyStoreException);
}

TEST(StoreExceptionFidelityTest, UnsupportedKeyStoreProviderThrowsNoSuchProvider) {
    PropertyRestore path("jxx.ext.net.ssl.keyStore");
    PropertyRestore type("jxx.ext.net.ssl.keyStoreType");
    PropertyRestore provider("jxx.ext.net.ssl.keyStoreProvider");
    setProperty("jxx.ext.net.ssl.keyStore", "missing-store");
    setProperty("jxx.ext.net.ssl.keyStoreType", "PKCS12");
    setProperty("jxx.ext.net.ssl.keyStoreProvider", "missing-provider");
    EXPECT_THROW(
        ::jxx::ext::net::ssl::internal::loadDefaultPropertyKeyManagers(),
        ::jxx::security::NoSuchProviderException);
}

TEST(StoreExceptionFidelityTest, MissingKeyStoreThrowsFileNotFound)
{
    PropertyRestore path(
        "jxx.ext.net.ssl.keyStore");

    PropertyRestore type(
        "jxx.ext.net.ssl.keyStoreType");

    PropertyRestore provider(
        "jxx.ext.net.ssl.keyStoreProvider");

    setProperty(
        "jxx.ext.net.ssl.keyStore",
        "missing-store");

    setProperty(
        "jxx.ext.net.ssl.keyStoreType",
        "PKCS12");

    ::jxx::lang::System::clearProperty(
        ::jxx::NEW<::jxx::lang::String>(
            "jxx.ext.net.ssl.keyStoreProvider"));

    EXPECT_THROW(
        ::jxx::ext::net::ssl::internal::
            loadDefaultPropertyKeyManagers(),
        ::jxx::io::FileNotFoundException);
}
TEST(
    StoreExceptionFidelityTest,
    UnsupportedTrustStoreTypeThrowsKeyStoreException)
{
    PropertyRestore path(
        "jxx.ext.net.ssl.trustStore");

    PropertyRestore type(
        "jxx.ext.net.ssl.trustStoreType");

    const TemporaryFile temporaryStore(
        "jxx-unsupported-trust-store-type.tmp");

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