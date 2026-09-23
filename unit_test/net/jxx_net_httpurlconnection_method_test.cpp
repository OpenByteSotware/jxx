#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.HttpURLConnection.h"
#include "net/jxx.net.ProtocolException.h"
#include "net/jxx.net.URL.h"
namespace {
class BareHttpConnection final : public ::jxx::net::HttpURLConnection {
public:
 explicit BareHttpConnection(const ::jxx::Ptr<::jxx::net::URL>& url):HttpURLConnection(url){}
 void connect() override {}
 void disconnect() override {}
 ::jxx::lang::jbool usingProxy() const override { return false; }
};
TEST(HttpURLConnectionMethodTest, RejectsNullAndUnsupportedMethodsWithProtocolException) {
    const auto url=::jxx::NEW<::jxx::net::URL>(::jxx::NEW<::jxx::lang::String>("http://localhost/"));
    const auto connection=std::make_shared<BareHttpConnection>(url);
    EXPECT_THROW(connection->setRequestMethod(nullptr),::jxx::net::ProtocolException);
    EXPECT_THROW(connection->setRequestMethod(::jxx::NEW<::jxx::lang::String>("PATCH")),::jxx::net::ProtocolException);
    EXPECT_NO_THROW(connection->setRequestMethod(::jxx::NEW<::jxx::lang::String>("GET")));
}
}
