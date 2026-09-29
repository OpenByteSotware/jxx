#pragma once

#include <mutex>

#include "lang/jxx.lang.Object.h"
#include "net/jxx.net.SocketAddress.h"
#include "net/internal/jxx.net.internal.NetPlatform.h"
#include "nio/channels/jxx.nio.channels.ReadableByteChannel.h"
#include "nio/channels/jxx.nio.channels.WritableByteChannel.h"

namespace jxx::nio::channels {

class SocketChannel final
    : public ::jxx::lang::ClassBase<
          SocketChannel,
          ::jxx::lang::Object,
          ReadableByteChannel,
          WritableByteChannel> {
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

    ::jxx::lang::jint read(
        const ::jxx::Ptr<::jxx::nio::ByteBuffer> destination) override;
    ::jxx::lang::jint write(
        const ::jxx::Ptr<::jxx::nio::ByteBuffer> source) override;

    ::jxx::lang::jbool isOpen() const override;
    void close() override;

private:
    SocketChannel();
    void ensureSocket();

    mutable std::mutex mutex_;
    ::jxx::net::internal::NativeSocket socket_ =
        ::jxx::net::internal::kInvalidSocket;
    ::jxx::lang::jbool blocking_ = true;
    ::jxx::lang::jbool open_ = true;
    ::jxx::lang::jbool connected_ = false;
    ::jxx::lang::jbool pending_ = false;
};

} // namespace jxx::nio::channels
