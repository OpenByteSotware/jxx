#pragma once
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.Authenticator.h"
#include "lang/jxx.lang.String.h"
namespace jxx::com::sun::net::httpserver { class BasicAuthenticator:public ::jxx::lang::ClassBase<BasicAuthenticator,Authenticator>{public:using JxxSuper=Authenticator;using Super=::jxx::lang::ClassBase<BasicAuthenticator,JxxSuper>;explicit BasicAuthenticator(const ::jxx::Ptr<::jxx::lang::String>& realm);::jxx::Ptr<::jxx::lang::String> getRealm()const;::jxx::Ptr<Result> authenticate(const ::jxx::Ptr<HttpExchange>& exchange)override;virtual ::jxx::lang::jbool checkCredentials(const ::jxx::Ptr<::jxx::lang::String>& username,const ::jxx::Ptr<::jxx::lang::String>& password)=0;private: ::jxx::Ptr<::jxx::lang::String> realm_;}; }
