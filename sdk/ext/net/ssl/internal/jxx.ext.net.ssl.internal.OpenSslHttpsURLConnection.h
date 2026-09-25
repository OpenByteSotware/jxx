#pragma once
#include "ext/net/ssl/jxx.ext.net.ssl.HttpsURLConnection.h"
namespace jxx::ext::net::ssl::internal {
::jxx::Ptr<::jxx::net::URLConnection> openHttpsConnection(
    const ::jxx::Ptr<::jxx::net::URL>& url);
}
