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
    PropertyRestore path("jxx.net.ssl.keyStore");
    PropertyRestore type("jxx.net.ssl.keyStoreType");
    setProperty("jxx.net.ssl.keyStore", "missing-store");
    setProperty("jxx.net.ssl.keyStoreType", "JKS");
    EXPECT_THROW(
        ::jxx::ext::net::ssl::internal::loadDefaultPropertyKeyManagers(),
        ::jxx::lang::IllegalStateException);
}

TEST(DefaultStorePropertiesTest, RejectsUnsupportedKeyStoreProvider) {
    PropertyRestore path("jxx.net.ssl.keyStore");
    PropertyRestore type("jxx.net.ssl.keyStoreType");
    PropertyRestore provider("jxx.net.ssl.keyStoreProvider");
    setProperty("jxx.net.ssl.keyStore", "missing-store");
    setProperty("jxx.net.ssl.keyStoreType", "PKCS12");
    setProperty("jxx.net.ssl.keyStoreProvider", "unsupported-provider");
    EXPECT_THROW(
        ::jxx::ext::net::ssl::internal::loadDefaultPropertyKeyManagers(),
        ::jxx::lang::IllegalStateException);
}

TEST(DefaultStorePropertiesTest, RejectsUnsupportedTrustStoreType) {
    PropertyRestore path("jxx.net.ssl.trustStore");
    PropertyRestore type("jxx.net.ssl.trustStoreType");
    setProperty("jxx.net.ssl.trustStore", "missing-store");
    setProperty("jxx.net.ssl.trustStoreType", "JKS");
    EXPECT_THROW(
        ::jxx::ext::net::ssl::internal::loadDefaultPropertyTrustManagers(),
        ::jxx::lang::IllegalStateException);
}

TEST(DefaultStorePropertiesTest, RejectsUnsupportedTrustStoreProvider) {
    PropertyRestore path("jxx.net.ssl.trustStore");
    PropertyRestore type("jxx.net.ssl.trustStoreType");
    PropertyRestore provider("jxx.net.ssl.trustStoreProvider");
    setProperty("jxx.net.ssl.trustStore", "missing-store");
    setProperty("jxx.net.ssl.trustStoreType", "PEM");
    setProperty("jxx.net.ssl.trustStoreProvider", "unsupported-provider");
    EXPECT_THROW(
        ::jxx::ext::net::ssl::internal::loadDefaultPropertyTrustManagers(),
        ::jxx::lang::IllegalStateException);
}
