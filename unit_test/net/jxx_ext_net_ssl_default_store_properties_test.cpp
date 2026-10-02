#include <gtest/gtest.h>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPropertyKeyManager.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPropertyTrustManager.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"

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
} // namespace

TEST(DefaultStorePropertiesTest, RejectsUnsupportedKeyStoreType) {
    PropertyRestore path("javax.net.ssl.keyStore");
    PropertyRestore type("javax.net.ssl.keyStoreType");
    setProperty("javax.net.ssl.keyStore", "missing-store");
    setProperty("javax.net.ssl.keyStoreType", "JKS");
    EXPECT_THROW(
        ::jxx::ext::net::ssl::internal::loadDefaultPropertyKeyManagers(),
        ::jxx::lang::IllegalStateException);
}

TEST(DefaultStorePropertiesTest, RejectsUnsupportedKeyStoreProvider) {
    PropertyRestore path("javax.net.ssl.keyStore");
    PropertyRestore type("javax.net.ssl.keyStoreType");
    PropertyRestore provider("javax.net.ssl.keyStoreProvider");
    setProperty("javax.net.ssl.keyStore", "missing-store");
    setProperty("javax.net.ssl.keyStoreType", "PKCS12");
    setProperty("javax.net.ssl.keyStoreProvider", "unsupported-provider");
    EXPECT_THROW(
        ::jxx::ext::net::ssl::internal::loadDefaultPropertyKeyManagers(),
        ::jxx::lang::IllegalStateException);
}

TEST(DefaultStorePropertiesTest, RejectsUnsupportedTrustStoreType) {
    PropertyRestore path("javax.net.ssl.trustStore");
    PropertyRestore type("javax.net.ssl.trustStoreType");
    setProperty("javax.net.ssl.trustStore", "missing-store");
    setProperty("javax.net.ssl.trustStoreType", "JKS");
    EXPECT_THROW(
        ::jxx::ext::net::ssl::internal::loadDefaultPropertyTrustManagers(),
        ::jxx::lang::IllegalStateException);
}

TEST(DefaultStorePropertiesTest, RejectsUnsupportedTrustStoreProvider) {
    PropertyRestore path("javax.net.ssl.trustStore");
    PropertyRestore type("javax.net.ssl.trustStoreType");
    PropertyRestore provider("javax.net.ssl.trustStoreProvider");
    setProperty("javax.net.ssl.trustStore", "missing-store");
    setProperty("javax.net.ssl.trustStoreType", "PEM");
    setProperty("javax.net.ssl.trustStoreProvider", "unsupported-provider");
    EXPECT_THROW(
        ::jxx::ext::net::ssl::internal::loadDefaultPropertyTrustManagers(),
        ::jxx::lang::IllegalStateException);
}
