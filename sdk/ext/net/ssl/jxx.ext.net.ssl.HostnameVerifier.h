#pragma once
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSession.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
namespace jxx::ext::net::ssl {
class HostnameVerifier : public ::jxx::lang::InterfaceBase<HostnameVerifier> {
public:
    ~HostnameVerifier() override = default;
    virtual ::jxx::lang::jbool verify(const ::jxx::Ptr<::jxx::lang::String>& hostname,const ::jxx::Ptr<SSLSession>& session)=0;
};
}
