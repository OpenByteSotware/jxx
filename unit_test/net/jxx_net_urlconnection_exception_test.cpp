#include <gtest/gtest.h>
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "net/jxx.net.UnknownServiceException.h"
#include "net/jxx.net.URL.h"
#include "net/jxx.net.URLConnection.h"
namespace {
class BareConnection final : public ::jxx::net::URLConnection {
public:
 explicit BareConnection(const ::jxx::Ptr<::jxx::net::URL>& url):URLConnection(url){}
 void connect() override {}
};
TEST(URLConnectionExceptionTest, BaseStreamsThrowJxxUnknownServiceException) {
    const auto url=::jxx::NEW<::jxx::net::URL>(::jxx::NEW<::jxx::lang::String>("http://localhost/"));
    const auto connection=std::make_shared<BareConnection>(url);
    EXPECT_THROW(connection->getInputStream(),::jxx::net::UnknownServiceException);
    EXPECT_THROW(connection->getOutputStream(),::jxx::net::UnknownServiceException);
}
TEST(URLConnectionExceptionTest, NullRequestPropertyKeyThrowsJxxIllegalArgumentException) {
    const auto url=::jxx::NEW<::jxx::net::URL>(::jxx::NEW<::jxx::lang::String>("http://localhost/"));
    const auto connection=std::make_shared<BareConnection>(url);
    EXPECT_THROW(connection->setRequestProperty(nullptr,::jxx::NEW<::jxx::lang::String>("x")),::jxx::lang::IllegalArgumentException);
}
}
