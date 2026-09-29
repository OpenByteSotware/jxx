#pragma once

#include <mutex>

#include "lang/jxx.lang.Object.h"
#include "net/jxx.net.SocketAddress.h"
#include "net/internal/jxx.net.internal.NetPlatform.h"
#include "nio/channels/jxx.nio.channels.ByteChannel.h"
#include "nio/channels/jxx.nio.channels.ScatteringByteChannel.h"
#include "nio/channels/jxx.nio.channels.GatheringByteChannel.h"
#include "nio/channels/jxx.nio.channels.NetworkChannel.h"
#include "net/jxx.net.Socket.h"

namespace jxx::nio::channels {

class SocketChannel final
    : public ::jxx::lang::ClassBase<
          SocketChannel,
          ::jxx::lang::Object,
          ByteChannel,
          ScatteringByteChannel,
          GatheringByteChannel,
          NetworkChannel> {
public:
    static ::jxx::Ptr<SocketChannel> open();
    static ::jxx::Ptr<SocketChannel> open(
        const ::jxx::Ptr<::jxx::net::SocketAddress>& remote);

    ::jxx::Ptr<SocketChannel> configureBlocking(
        ::jxx::lang::jbool block);
    ::jxx::lang::jbool isBlocking() const noexcept;

    ::jxx::lang::jbool connect(
        const ::jxx::Ptr<::jxx::net::SocketAddress>& remote);
    ::jxx::lang::jbool finishConnect();
    ::jxx::lang::jbool isConnectionPending() const noexcept;
    ::jxx::lang::jbool isConnected() const noexcept;
    ::jxx::Ptr<NetworkChannel> bind(
        const ::jxx::Ptr<::jxx::net::SocketAddress>& local) override;
    ::jxx::Ptr<::jxx::net::SocketAddress> getLocalAddress() const override;
    ::jxx::Ptr<::jxx::net::SocketAddress> getRemoteAddress() const;
    ::jxx::Ptr<::jxx::net::Socket> socket();
    ::jxx::Ptr<SocketChannel> shutdownInput();
    ::jxx::Ptr<SocketChannel> shutdownOutput();
    ::jxx::lang::jint validOps() const noexcept;

    using BufferArray = ScatteringByteChannel::BufferArray;
    using Option = NetworkChannel::Option;
    ::jxx::lang::jlong read(const ::jxx::Ptr<BufferArray>& destinations) override;
    ::jxx::lang::jlong read(const ::jxx::Ptr<BufferArray>& destinations,::jxx::lang::jint offset,::jxx::lang::jint length) override;
    ::jxx::lang::jlong write(const ::jxx::Ptr<BufferArray>& sources) override;
    ::jxx::lang::jlong write(const ::jxx::Ptr<BufferArray>& sources,::jxx::lang::jint offset,::jxx::lang::jint length) override;
    ::jxx::Ptr<NetworkChannel> setOption(const ::jxx::Ptr<Option>& name,const ::jxx::Ptr<::jxx::lang::Object>& value) override;
    ::jxx::Ptr<::jxx::lang::Object> getOption(const ::jxx::Ptr<Option>& name) const override;
    ::jxx::Ptr<::jxx::util::Set<Option>> supportedOptions() const override;

    ::jxx::lang::jint read(
        const ::jxx::Ptr<::jxx::nio::ByteBuffer> destination) override;
    ::jxx::lang::jint write(
        const ::jxx::Ptr<::jxx::nio::ByteBuffer> source) override;

    ::jxx::lang::jbool isOpen() const override;
    void close() override;

public:
    SocketChannel();

private:
    void ensureSocket();

    mutable std::mutex mutex_;
    ::jxx::net::internal::NativeSocket socket_ =
        ::jxx::net::internal::kInvalidSocket;
    ::jxx::lang::jbool blocking_ = true;
    ::jxx::lang::jbool open_ = true;
    ::jxx::lang::jbool connected_ = false;
    ::jxx::lang::jbool pending_ = false;
    ::jxx::lang::jbool bound_ = false;
    ::jxx::lang::jbool inputShutdown_ = false;
    ::jxx::lang::jbool outputShutdown_ = false;
};

} // namespace jxx::nio::channels
