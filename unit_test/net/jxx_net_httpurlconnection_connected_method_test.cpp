#include <gtest/gtest.h>
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.HttpURLConnection.h"
#include "net/jxx.net.URL.h"
namespace {
class ConnectedHttp final : public ::jxx::net::HttpURLConnection {
public:
 explicit ConnectedHttp(const ::jxx::Ptr<::jxx::net::URL>& url):HttpURLConnection(url){}
 void connect() override { connected_=true; }
 void disconnect() override { connected_=false; }
 ::jxx::lang::jbool usingProxy() const override { return false; }
};
TEST(HttpURLConnectionConnectedMethodTest, RequestMethodCannotChangeAfterConnect) {
 auto url=::jxx::NEW<::jxx::net::URL>(::jxx::NEW<::jxx::lang::String>("http://localhost/"));
 auto c=std::make_shared<ConnectedHttp>(url);
 c->setRequestMethod(::jxx::NEW<::jxx::lang::String>("POST")); c->connect();
 EXPECT_THROW(c->setRequestMethod(::jxx::NEW<::jxx::lang::String>("GET")),::jxx::lang::IllegalStateException);
}
}
