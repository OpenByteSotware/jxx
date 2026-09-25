#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::net { class Socket; }
namespace jxx::ext::net::ssl {
class SSLSocketFactory : public ::jxx::lang::ClassBase<SSLSocketFactory,::jxx::lang::Object> {
public:
    using StringArray=::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>,1U>;
    ~SSLSocketFactory() override=default;
    virtual ::jxx::Ptr<StringArray> getDefaultCipherSuites() const=0;
    virtual ::jxx::Ptr<StringArray> getSupportedCipherSuites() const=0;
    virtual ::jxx::Ptr<::jxx::net::Socket> createSocket(const ::jxx::Ptr<::jxx::net::Socket>& socket,const ::jxx::Ptr<::jxx::lang::String>& host,::jxx::lang::jint port,::jxx::lang::jbool autoClose)=0;
};
}
