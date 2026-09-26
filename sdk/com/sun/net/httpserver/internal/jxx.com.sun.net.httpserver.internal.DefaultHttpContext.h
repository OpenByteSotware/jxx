#pragma once
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpContext.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.HashMap.h"
namespace jxx::com::sun::net::httpserver::internal {
class DefaultHttpContext final : public ::jxx::lang::ClassBase<DefaultHttpContext,::jxx::com::sun::net::httpserver::HttpContext> {
public:
 using JxxSuper=::jxx::com::sun::net::httpserver::HttpContext; using Super=::jxx::lang::ClassBase<DefaultHttpContext,JxxSuper>;
 DefaultHttpContext(const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpServer>& server,const ::jxx::Ptr<::jxx::lang::String>& path,const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpHandler>& handler);
 ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpHandler> getHandler() override; void setHandler(const ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpHandler>& handler) override;
 ::jxx::Ptr<::jxx::lang::String> getPath() override; ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpServer> getServer() override;
 ::jxx::Ptr<::jxx::util::Map<::jxx::lang::String,::jxx::lang::Object>> getAttributes() override; ::jxx::Ptr<::jxx::util::List<::jxx::com::sun::net::httpserver::Filter>> getFilters() override;
 ::jxx::Ptr<::jxx::com::sun::net::httpserver::Authenticator> setAuthenticator(const ::jxx::Ptr<::jxx::com::sun::net::httpserver::Authenticator>& auth) override; ::jxx::Ptr<::jxx::com::sun::net::httpserver::Authenticator> getAuthenticator() override;
private:
 ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpServer> server_; ::jxx::Ptr<::jxx::lang::String> path_; ::jxx::Ptr<::jxx::com::sun::net::httpserver::HttpHandler> handler_; ::jxx::Ptr<::jxx::com::sun::net::httpserver::Authenticator> authenticator_;
 ::jxx::Ptr<::jxx::util::Map<::jxx::lang::String,::jxx::lang::Object>> attributes_; ::jxx::Ptr<::jxx::util::List<::jxx::com::sun::net::httpserver::Filter>> filters_;
}; }
