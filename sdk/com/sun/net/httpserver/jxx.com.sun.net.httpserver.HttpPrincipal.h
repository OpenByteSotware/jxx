#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "security/jxx.security.Principal.h"
namespace jxx::com::sun::net::httpserver {
class HttpPrincipal final : public ::jxx::lang::ClassBase<HttpPrincipal,::jxx::lang::Object,::jxx::security::Principal> {
public: using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<HttpPrincipal,JxxSuper,::jxx::security::Principal>;
 HttpPrincipal(const ::jxx::Ptr<::jxx::lang::String>& username,const ::jxx::Ptr<::jxx::lang::String>& realm);
 ::jxx::Ptr<::jxx::lang::String> getName() const override; ::jxx::Ptr<::jxx::lang::String> getUsername() const; ::jxx::Ptr<::jxx::lang::String> getRealm() const;
 ::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>& other) const override; ::jxx::lang::jint hashCode() const override; ::jxx::Ptr<::jxx::lang::String> toString() const override;
private: ::jxx::Ptr<::jxx::lang::String> username_,realm_;
}; }
