#pragma once
#include <memory>
#include <mutex>
#include "lang/jxx.lang.ClassInfo.h"
#include "nio/channels/jxx.nio.channels.NetworkChannel.h"
#include "nio/channels/spi/jxx.nio.channels.spi.AbstractSelectableChannel.h"
namespace jxx::net { class ServerSocket; class SocketAddress; namespace internal{struct NativeSocketState;} }
namespace jxx::nio::channels::spi::internal { class NativeSelectorProvider; }
namespace jxx::nio::channels { class Selector; class SelectionKey; class SocketChannel;
class ServerSocketChannel final:public ::jxx::lang::ClassBase<ServerSocketChannel,::jxx::nio::channels::spi::AbstractSelectableChannel,NetworkChannel>{public:using Option=NetworkChannel::Option;static ::jxx::Ptr<ServerSocketChannel> open();ServerSocketChannel();~ServerSocketChannel()override;::jxx::Ptr<NetworkChannel> bind(const ::jxx::Ptr<::jxx::net::SocketAddress>&)override;::jxx::Ptr<NetworkChannel> bind(const ::jxx::Ptr<::jxx::net::SocketAddress>&,::jxx::lang::jint);::jxx::Ptr<::jxx::net::ServerSocket> socket();::jxx::Ptr<SocketChannel> accept();::jxx::lang::jint validOps()const noexcept;::jxx::Ptr<::jxx::net::SocketAddress> getLocalAddress()const override;::jxx::Ptr<NetworkChannel> setOption(const ::jxx::Ptr<Option>&,const ::jxx::Ptr<::jxx::lang::Object>&)override;::jxx::Ptr<::jxx::lang::Object> getOption(const ::jxx::Ptr<Option>&)const override;::jxx::Ptr<::jxx::util::Set<Option>> supportedOptions()const override;private:friend class ::jxx::nio::channels::spi::internal::NativeSelectorProvider;void implCloseSelectableChannel()override;void implConfigureBlocking(::jxx::lang::jbool);void setBlocking_(::jxx::lang::jbool);mutable std::mutex mutex_;std::shared_ptr<::jxx::net::internal::NativeSocketState> state_;::jxx::Ptr<::jxx::net::ServerSocket> socket_;::jxx::lang::jbool blocking_=true;}; }
