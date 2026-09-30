#pragma once
#include <memory>
#include <mutex>
#include "lang/jxx.lang.ClassInfo.h"
#include "nio/channels/jxx.nio.channels.NetworkChannel.h"
namespace jxx::net { class ServerSocket; class SocketAddress; namespace internal { struct NativeSocketState; } }
namespace jxx::nio::channels {
class SocketChannel;
class ServerSocketChannel final : public ::jxx::lang::ClassBase<ServerSocketChannel,::jxx::lang::Object,NetworkChannel> {
public:
 using JxxSuper=::jxx::lang::Object; using Option=NetworkChannel::Option;
 static ::jxx::Ptr<ServerSocketChannel> open();
 ServerSocketChannel(); ~ServerSocketChannel() override;
 ::jxx::Ptr<ServerSocketChannel> configureBlocking(::jxx::lang::jbool block);
 ::jxx::lang::jbool isBlocking() const noexcept;
 ::jxx::Ptr<NetworkChannel> bind(const ::jxx::Ptr<::jxx::net::SocketAddress>& local) override;
 ::jxx::Ptr<NetworkChannel> bind(const ::jxx::Ptr<::jxx::net::SocketAddress>& local,::jxx::lang::jint backlog);
 ::jxx::Ptr<::jxx::net::ServerSocket> socket();
 ::jxx::Ptr<SocketChannel> accept();
 ::jxx::Ptr<::jxx::net::SocketAddress> getLocalAddress() const override;
 ::jxx::Ptr<NetworkChannel> setOption(const ::jxx::Ptr<Option>&,const ::jxx::Ptr<::jxx::lang::Object>&) override;
 ::jxx::Ptr<::jxx::lang::Object> getOption(const ::jxx::Ptr<Option>&) const override;
 ::jxx::Ptr<::jxx::util::Set<Option>> supportedOptions() const override;
 ::jxx::lang::jbool isOpen() const override; void close() override;
private:
 void setNativeBlocking_(::jxx::lang::jbool);
 mutable std::mutex mutex_; std::shared_ptr<::jxx::net::internal::NativeSocketState> state_;
 ::jxx::Ptr<::jxx::net::ServerSocket> socketView_; ::jxx::lang::jbool blocking_=true;
};
} // namespace jxx::nio::channels
