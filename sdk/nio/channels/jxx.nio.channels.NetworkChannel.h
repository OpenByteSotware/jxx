#pragma once
#include "net/jxx.net.SocketAddress.h"
#include "net/jxx.net.SocketOption.h"
#include "nio/channels/jxx.nio.channels.Channel.h"
#include "util/jxx.util.Set.h"
namespace jxx::nio::channels {
class NetworkChannel:public ::jxx::lang::InterfaceBase<NetworkChannel,Channel>{public:using Option=::jxx::net::SocketOption<::jxx::lang::Object>;~NetworkChannel()override=default;virtual ::jxx::Ptr<NetworkChannel> bind(const ::jxx::Ptr<::jxx::net::SocketAddress>&)=0;virtual ::jxx::Ptr<::jxx::net::SocketAddress> getLocalAddress()const=0;virtual ::jxx::Ptr<NetworkChannel> setOption(const ::jxx::Ptr<Option>&,const ::jxx::Ptr<::jxx::lang::Object>&)=0;virtual ::jxx::Ptr<::jxx::lang::Object> getOption(const ::jxx::Ptr<Option>&)const=0;virtual ::jxx::Ptr<::jxx::util::Set<Option>> supportedOptions() const = 0;};}
