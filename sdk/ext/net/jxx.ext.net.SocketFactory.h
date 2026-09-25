#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::lang { class String; }
namespace jxx::net { class Socket; class InetAddress; }
namespace jxx::ext::net {
class SocketFactory : public ::jxx::lang::ClassBase<SocketFactory,::jxx::lang::Object> {
public:
 virtual ~SocketFactory() override=default;
 static ::jxx::Ptr<SocketFactory> getDefault();
 virtual ::jxx::Ptr<::jxx::net::Socket> createSocket()=0;
 virtual ::jxx::Ptr<::jxx::net::Socket> createSocket(const ::jxx::Ptr<::jxx::lang::String>& host,::jxx::lang::jint port)=0;
 virtual ::jxx::Ptr<::jxx::net::Socket> createSocket(const ::jxx::Ptr<::jxx::lang::String>& host,::jxx::lang::jint port,const ::jxx::Ptr<::jxx::net::InetAddress>& local,::jxx::lang::jint localPort)=0;
 virtual ::jxx::Ptr<::jxx::net::Socket> createSocket(const ::jxx::Ptr<::jxx::net::InetAddress>& host,::jxx::lang::jint port)=0;
 virtual ::jxx::Ptr<::jxx::net::Socket> createSocket(const ::jxx::Ptr<::jxx::net::InetAddress>& host,::jxx::lang::jint port,const ::jxx::Ptr<::jxx::net::InetAddress>& local,::jxx::lang::jint localPort)=0;
protected: SocketFactory()=default;
};
}
