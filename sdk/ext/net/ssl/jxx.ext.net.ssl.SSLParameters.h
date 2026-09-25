#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::ext::net::ssl {
class SSLParameters : public ::jxx::lang::ClassBase<SSLParameters,::jxx::lang::Object> {
public:
 using StringArray=::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>,1U>;
 SSLParameters();
 ::jxx::Ptr<StringArray> getCipherSuites() const; void setCipherSuites(const ::jxx::Ptr<StringArray>& values);
 ::jxx::Ptr<StringArray> getProtocols() const; void setProtocols(const ::jxx::Ptr<StringArray>& values);
 ::jxx::lang::jbool getNeedClientAuth() const; void setNeedClientAuth(::jxx::lang::jbool value);
 ::jxx::lang::jbool getWantClientAuth() const; void setWantClientAuth(::jxx::lang::jbool value);
private: ::jxx::Ptr<StringArray> ciphers_,protocols_; ::jxx::lang::jbool need_=false,want_=false;
};}
