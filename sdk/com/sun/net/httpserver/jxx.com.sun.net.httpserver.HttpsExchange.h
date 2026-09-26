#pragma once
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpExchange.h"
namespace jxx::ext::net::ssl { class SSLSession; }
namespace jxx::com::sun::net::httpserver {
class HttpsExchange : public ::jxx::lang::ClassBase<HttpsExchange,HttpExchange> {
public: using JxxSuper=HttpExchange; using Super=::jxx::lang::ClassBase<HttpsExchange,JxxSuper>; virtual ~HttpsExchange()override=default; virtual ::jxx::Ptr<::jxx::ext::net::ssl::SSLSession> getSSLSession()=0;
protected: HttpsExchange()=default;
}; }
