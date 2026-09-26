#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpPrincipal.h"
#include "lang/jxx.lang.Exceptions.h"
namespace jxx::com::sun::net::httpserver {
HttpPrincipal::HttpPrincipal(const ::jxx::Ptr<::jxx::lang::String>& u,const ::jxx::Ptr<::jxx::lang::String>& r):Super(),username_(u),realm_(r){if(!u||!r)throw ::jxx::lang::NullPointerException();}
::jxx::Ptr<::jxx::lang::String> HttpPrincipal::getName()const{return ::jxx::NEW<::jxx::lang::String>((username_->utf8()+"@"+realm_->utf8()).c_str());}
::jxx::Ptr<::jxx::lang::String> HttpPrincipal::getUsername()const{return username_;} ::jxx::Ptr<::jxx::lang::String> HttpPrincipal::getRealm()const{return realm_;}
::jxx::lang::jbool HttpPrincipal::equals(const ::jxx::Ptr<::jxx::lang::Object>& o)const{auto p=::jxx::CAST<HttpPrincipal>(o);return p&&username_->equals(p->username_)&&realm_->equals(p->realm_);} ::jxx::lang::jint HttpPrincipal::hashCode()const{return username_->hashCode()+realm_->hashCode();} ::jxx::Ptr<::jxx::lang::String> HttpPrincipal::toString()const{return getName();}
}
