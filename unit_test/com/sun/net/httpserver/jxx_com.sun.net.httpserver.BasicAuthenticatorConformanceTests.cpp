#include <gtest/gtest.h>
#include "lang/jxx.lang.NullPointerException.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.BasicAuthenticator.h"
#include "lang/jxx.lang.String.h"

namespace jxx::com::sun::net::httpserver {

class TestBasicAuthenticator final : public BasicAuthenticator {
public:
    explicit TestBasicAuthenticator(const ::jxx::Ptr<::jxx::lang::String>& realm)
        : BasicAuthenticator(realm) {}

    ::jxx::lang::jbool checkCredentials(
        const ::jxx::Ptr<::jxx::lang::String>&,
        const ::jxx::Ptr<::jxx::lang::String>&) override {
        return true;
    }
};

TEST(BasicAuthenticatorConformanceTests, RetainsRealm)
{
    auto realm = ::jxx::NEW<::jxx::lang::String>("restricted");
    auto authenticator = ::jxx::NEW<TestBasicAuthenticator>(realm);
    EXPECT_EQ(authenticator->getRealm().get(), realm.get());
}

TEST(BasicAuthenticatorConformanceTests, RejectsNullRealm)
{
    EXPECT_THROW((::jxx::NEW<TestBasicAuthenticator>(nullptr)), ::jxx::lang::NullPointerException);
}

} // namespace jxx::com::sun::net::httpserver
