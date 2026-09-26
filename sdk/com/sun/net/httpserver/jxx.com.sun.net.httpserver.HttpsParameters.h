#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::net { class InetSocketAddress; }
namespace jxx::ext::net::ssl { class SSLParameters; }
namespace jxx::com::sun::net::httpserver { class HttpsConfigurator;
class HttpsParameters : public ::jxx::lang::ClassBase<HttpsParameters,::jxx::lang::Object> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<HttpsParameters,JxxSuper>; using StringArray=::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>,1U>;
 virtual ~HttpsParameters() override=default;
 virtual ::jxx::Ptr<::jxx::net::InetSocketAddress> getClientAddress()=0;
 virtual ::jxx::Ptr<HttpsConfigurator> getHttpsConfigurator()=0;
 ::jxx::Ptr<StringArray> getCipherSuites() const; void setCipherSuites(const ::jxx::Ptr<StringArray>& suites);
 ::jxx::Ptr<StringArray> getProtocols() const; void setProtocols(const ::jxx::Ptr<StringArray>& protocols);
 ::jxx::lang::jbool getNeedClientAuth() const noexcept; void setNeedClientAuth(::jxx::lang::jbool value) noexcept;
 ::jxx::lang::jbool getWantClientAuth() const noexcept; void setWantClientAuth(::jxx::lang::jbool value) noexcept;
 virtual void setSSLParameters(const ::jxx::Ptr<::jxx::ext::net::ssl::SSLParameters>& parameters)=0;
protected: HttpsParameters()=default;
private: static ::jxx::Ptr<StringArray> copy(const ::jxx::Ptr<StringArray>& source); ::jxx::Ptr<StringArray> suites_,protocols_; ::jxx::lang::jbool need_=false,want_=false;
}; }
