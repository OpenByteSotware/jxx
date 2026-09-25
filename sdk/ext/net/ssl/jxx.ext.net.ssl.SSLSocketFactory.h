#pragma once
#include "ext/net/jxx.ext.net.SocketFactory.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::io {class InputStream;}
namespace jxx::ext::net::ssl {
class SSLSocketFactory:public ::jxx::lang::ClassBase<SSLSocketFactory,::jxx::ext::net::SocketFactory>{public:
 using StringArray=::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>,1U>;
 static ::jxx::Ptr<::jxx::ext::net::SocketFactory> getDefault();
 virtual ::jxx::Ptr<StringArray> getDefaultCipherSuites()const=0;
 virtual ::jxx::Ptr<StringArray> getSupportedCipherSuites()const=0;
 virtual ::jxx::Ptr<::jxx::net::Socket> createSocket(const ::jxx::Ptr<::jxx::net::Socket>& socket,const ::jxx::Ptr<::jxx::lang::String>& host,::jxx::lang::jint port,::jxx::lang::jbool autoClose)=0;
 virtual ::jxx::Ptr<::jxx::net::Socket> createSocket(const ::jxx::Ptr<::jxx::net::Socket>& socket,const ::jxx::Ptr<::jxx::io::InputStream>& consumed,::jxx::lang::jbool autoClose);
protected:SSLSocketFactory()=default;};}
