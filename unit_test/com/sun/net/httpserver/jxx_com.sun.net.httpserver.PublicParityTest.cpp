#include <gtest/gtest.h>

#include <type_traits>

#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.BasicAuthenticator.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpExchange.h"
#include "io/jxx.io.Closeable.h"
#include "lang/jxx.lang.Exceptions.h"

namespace jxx::com::sun::net::httpserver {
namespace {

class TestBasicAuthenticator final : public BasicAuthenticator {
public:
    explicit TestBasicAuthenticator(
        const ::jxx::Ptr<::jxx::lang::String>& value)
        : BasicAuthenticator(value)
    {
    }

    ::jxx::lang::jbool checkCredentials(
        const ::jxx::Ptr<::jxx::lang::String>&,
        const ::jxx::Ptr<::jxx::lang::String>&) override
    {
        return true;
    }

    ::jxx::Ptr<::jxx::lang::String> protectedRealm() const
    {
        return realm;
    }
};

TEST(HttpServerPublicParityTest, BasicAuthenticatorExposesProtectedRealm) {
    const auto expected = ::jxx::NEW<::jxx::lang::String>("test-realm");
    const auto authenticator = ::jxx::NEW<TestBasicAuthenticator>(expected);

    EXPECT_EQ(expected.get(), authenticator->getRealm().get());
    EXPECT_EQ(expected.get(), authenticator->protectedRealm().get());
}

TEST(HttpServerPublicParityTest, BasicAuthenticatorRejectsNullRealm) {
    const ::jxx::Ptr<::jxx::lang::String> nullRealm;
    EXPECT_THROW(
        ::jxx::NEW<TestBasicAuthenticator>(nullRealm),
        ::jxx::lang::NullPointerException);
}

TEST(HttpServerPublicParityTest, BasicAuthenticatorRejectsEmptyRealm) {
    const auto emptyRealm = ::jxx::NEW<::jxx::lang::String>("");
    EXPECT_THROW(
        ::jxx::NEW<TestBasicAuthenticator>(emptyRealm),
        ::jxx::lang::NullPointerException);
}

TEST(HttpServerPublicParityTest, HttpExchangeDoesNotImplementCloseable) {
    EXPECT_FALSE((std::is_base_of_v<::jxx::io::Closeable, HttpExchange>));
}

} // namespace
} // namespace jxx::com::sun::net::httpserver
