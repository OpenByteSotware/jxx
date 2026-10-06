#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "net/jxx.net.HttpURLConnection.h"
#include "net/jxx.net.ProtocolException.h"
#include "net/jxx.net.URL.h"

namespace
{

    class ConnectedHttp final
        : public ::jxx::net::HttpURLConnection
    {
    public:
        explicit ConnectedHttp(
            const ::jxx::Ptr<::jxx::net::URL>& url)
            : HttpURLConnection(url)
        {
        }

        void connect() override
        {
            connected_ = true;
        }

        void disconnect() override
        {
            connected_ = false;
        }

        ::jxx::lang::jbool usingProxy() const override
        {
            return false;
        }
    };

    TEST(
        HttpURLConnectionConnectedMethodTest,
        RequestMethodCannotChangeAfterConnect)
    {

        const auto url =
            ::jxx::NEW<::jxx::net::URL>(
                ::jxx::NEW<::jxx::lang::String>(
                    "http://localhost/"));

        const auto connection =
            ::jxx::NEW<ConnectedHttp>(url);

        connection->setRequestMethod(
            ::jxx::NEW<::jxx::lang::String>("POST"));

        connection->connect();

        EXPECT_THROW(
            connection->setRequestMethod(
                ::jxx::NEW<::jxx::lang::String>("GET")),
            ::jxx::net::ProtocolException);
    }

} // namespace