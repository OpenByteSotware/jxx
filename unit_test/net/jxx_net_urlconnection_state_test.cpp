#include <gtest/gtest.h>
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.URL.h"
#include "net/jxx.net.URLConnection.h"
namespace {
class StateConnection final : public ::jxx::net::URLConnection {
public:
 explicit StateConnection(const ::jxx::Ptr<::jxx::net::URL>& url):URLConnection(url){}
 void connect() override { connected_=true; }
};
TEST(URLConnectionStateTest, RejectsNegativeTimeouts) {
 auto url=::jxx::NEW<::jxx::net::URL>(::jxx::NEW<::jxx::lang::String>("http://localhost/"));
 auto c=std::make_shared<StateConnection>(url);
 EXPECT_THROW(c->setConnectTimeout(-1),::jxx::lang::IllegalArgumentException);
 EXPECT_THROW(c->setReadTimeout(-1),::jxx::lang::IllegalArgumentException);
 EXPECT_NO_THROW(c->setConnectTimeout(0)); 
 EXPECT_NO_THROW(c->setReadTimeout(1));
}
TEST(URLConnectionStateTest, ConfigurationCannotChangeAfterConnect) {
 auto url=::jxx::NEW<::jxx::net::URL>(::jxx::NEW<::jxx::lang::String>("http://localhost/"));
 auto c=std::make_shared<StateConnection>(url); c->connect();
 EXPECT_THROW(c->setDoInput(false),::jxx::lang::IllegalStateException);
 EXPECT_THROW(c->setDoOutput(true),::jxx::lang::IllegalStateException);
 EXPECT_THROW(c->setUseCaches(false),::jxx::lang::IllegalStateException);
 EXPECT_THROW(c->setIfModifiedSince(1),::jxx::lang::IllegalStateException);
 EXPECT_THROW(c->setAllowUserInteraction(true),::jxx::lang::IllegalStateException);
}
}
