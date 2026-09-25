#pragma once
#include "ext/net/jxx.ext.net.SocketFactory.h"
namespace jxx::ext::net::internal {
class DefaultSocketFactory final:public ::jxx::lang::ClassBase<DefaultSocketFactory,::jxx::ext::net::SocketFactory>{public:
::jxx::Ptr<::jxx::net::Socket> createSocket()override;
::jxx::Ptr<::jxx::net::Socket> createSocket(const ::jxx::Ptr<::jxx::lang::String>&,::jxx::lang::jint)override;
::jxx::Ptr<::jxx::net::Socket> createSocket(const ::jxx::Ptr<::jxx::lang::String>&,::jxx::lang::jint,const ::jxx::Ptr<::jxx::net::InetAddress>&,::jxx::lang::jint)override;
::jxx::Ptr<::jxx::net::Socket> createSocket(const ::jxx::Ptr<::jxx::net::InetAddress>&,::jxx::lang::jint)override;
::jxx::Ptr<::jxx::net::Socket> createSocket(const ::jxx::Ptr<::jxx::net::InetAddress>&,::jxx::lang::jint,const ::jxx::Ptr<::jxx::net::InetAddress>&,::jxx::lang::jint)override;};}
