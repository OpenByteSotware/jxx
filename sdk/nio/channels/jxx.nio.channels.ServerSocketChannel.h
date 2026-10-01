#pragma once

#include <memory>
#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "nio/channels/jxx.nio.channels.NetworkChannel.h"
#include "nio/channels/spi/jxx.nio.channels.spi.AbstractSelectableChannel.h"

namespace jxx::net {
class ServerSocket;
class SocketAddress;
namespace internal { struct NativeSocketState; }
}
namespace jxx::nio::channels::spi::internal {
class NativeSelectorProvider;
}
namespace jxx::nio::channels {
class Selector;
class SelectionKey;
class SocketChannel;

class ServerSocketChannel final
    : public ::jxx::lang::ClassBase<
          ServerSocketChannel,
          ::jxx::nio::channels::spi::AbstractSelectableChannel,
          NetworkChannel> {
public:
    using Option = NetworkChannel::Option;

    static ::jxx::Ptr<ServerSocketChannel> open();
    ServerSocketChannel();
    ~ServerSocketChannel() override;

    ::jxx::Ptr<NetworkChannel> bind(
        const ::jxx::Ptr<::jxx::net::SocketAddress>& local) override;
    ::jxx::Ptr<NetworkChannel> bind(
        const ::jxx::Ptr<::jxx::net::SocketAddress>& local,
        ::jxx::lang::jint backlog);
    ::jxx::Ptr<::jxx::net::ServerSocket> socket();
    ::jxx::Ptr<SocketChannel> accept();
    ::jxx::lang::jint validOps() const noexcept;
    ::jxx::Ptr<::jxx::net::SocketAddress> getLocalAddress() const override;
    ::jxx::Ptr<NetworkChannel> setOption(
        const ::jxx::Ptr<Option>& name,
        const ::jxx::Ptr<::jxx::lang::Object>& value) override;
    ::jxx::Ptr<::jxx::lang::Object> getOption(
        const ::jxx::Ptr<Option>& name) const override;
    ::jxx::Ptr<::jxx::util::Set<Option>> supportedOptions() const override;

private:
    friend class ::jxx::nio::channels::spi::internal::NativeSelectorProvider;

    void implCloseSelectableChannel() override;
    void implConfigureBlocking(::jxx::lang::jbool block) override;
    void setBlocking_(::jxx::lang::jbool block);
    void applyPendingOptions_();

    mutable std::mutex mutex_;
    std::shared_ptr<::jxx::net::internal::NativeSocketState> state_;
    ::jxx::Ptr<::jxx::net::ServerSocket> socket_;
    ::jxx::lang::jbool blocking_ = true;
    ::jxx::lang::jbool reuseAddress_ = false;
    ::jxx::lang::jint receiveBufferSize_ = 0;
    ::jxx::lang::jbool reuseAddressSet_ = false;
    ::jxx::lang::jbool receiveBufferSizeSet_ = false;
};

} // namespace jxx::nio::channels
