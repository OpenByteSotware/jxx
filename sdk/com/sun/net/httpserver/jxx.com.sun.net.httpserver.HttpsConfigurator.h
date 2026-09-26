#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::ext::net::ssl { class SSLContext; }
namespace jxx::com::sun::net::httpserver { class HttpsParameters;
class HttpsConfigurator : public ::jxx::lang::ClassBase<HttpsConfigurator,::jxx::lang::Object> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<HttpsConfigurator,JxxSuper>;
 explicit HttpsConfigurator(const ::jxx::Ptr<::jxx::ext::net::ssl::SSLContext>& context);
 ::jxx::Ptr<::jxx::ext::net::ssl::SSLContext> getSSLContext() const;
 virtual void configure(const ::jxx::Ptr<HttpsParameters>& parameters);
private:
 ::jxx::Ptr<::jxx::ext::net::ssl::SSLContext> context_;
}; }
