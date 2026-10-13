#include "nio/channels/jxx.nio.channels.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "nio/channels/jxx.nio.channels.AlreadyBoundException.h"
#include "nio/channels/jxx.nio.channels.AlreadyConnectedException.h"
#include "nio/channels/jxx.nio.channels.AsynchronousCloseException.h"
#include "nio/channels/jxx.nio.channels.ByteChannel.h"
#include "nio/channels/jxx.nio.channels.ClosedByInterruptException.h"
#include "nio/channels/jxx.nio.channels.ClosedChannelException.h"
#include "nio/channels/jxx.nio.channels.ConnectionPendingException.h"
#include "nio/channels/jxx.nio.channels.GatheringByteChannel.h"
#include "nio/channels/jxx.nio.channels.IllegalBlockingModeException.h"
#include "nio/channels/jxx.nio.channels.InterruptibleChannel.h"
#include "nio/channels/jxx.nio.channels.NetworkChannel.h"
#include "nio/channels/jxx.nio.channels.NoConnectionPendingException.h"
#include "nio/channels/jxx.nio.channels.NotYetBoundException.h"
#include "nio/channels/jxx.nio.channels.NotYetConnectedException.h"
#include "nio/channels/jxx.nio.channels.ScatteringByteChannel.h"
#include "nio/channels/jxx.nio.channels.SelectableChannel.h"
#include "nio/channels/jxx.nio.channels.SelectionKey.h"
#include "nio/channels/jxx.nio.channels.Selector.h"
#include "nio/channels/jxx.nio.channels.ServerSocketChannel.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
#include "nio/channels/jxx.nio.channels.UnresolvedAddressException.h"
#include "nio/channels/jxx.nio.channels.UnsupportedAddressTypeException.h"
#include "nio/channels/jxx.nio.channels.CancelledKeyException.h"
#include "nio/channels/jxx.nio.channels.Channel.h"
#include "nio/channels/jxx.nio.channels.ClosedSelectorException.h"
#include "nio/channels/jxx.nio.channels.FileChannel.h"
#include "nio/channels/jxx.nio.channels.ReadableByteChannel.h"
#include "nio/channels/jxx.nio.channels.SeekableByteChannel.h"
#include "nio/channels/jxx.nio.channels.WritableByteChannel.h"

namespace jxx::nio::channels {
namespace {

template <typename T>
void registerType()
{
    ::jxx::lang::class_info_detail::registerClassInfo<T>();
}

} // namespace

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        registerType<AlreadyBoundException>();
        registerType<AlreadyConnectedException>();
        registerType<AsynchronousCloseException>();
        registerType<ByteChannel>();
        registerType<ClosedByInterruptException>();
        registerType<ClosedChannelException>();
        registerType<ConnectionPendingException>();
        registerType<GatheringByteChannel>();
        registerType<IllegalBlockingModeException>();
        registerType<InterruptibleChannel>();
        registerType<NetworkChannel>();
        registerType<NoConnectionPendingException>();
        registerType<NotYetBoundException>();
        registerType<NotYetConnectedException>();
        registerType<ScatteringByteChannel>();
        registerType<SelectableChannel>();
        registerType<SelectionKey>();
        registerType<Selector>();
        registerType<ServerSocketChannel>();
        registerType<SocketChannel>();
        registerType<UnresolvedAddressException>();
        registerType<UnsupportedAddressTypeException>();
        registerType<CancelledKeyException>();
        registerType<Channel>();
        registerType<ClosedSelectorException>();
        registerType<FileChannel>();
        registerType<ReadableByteChannel>();
        registerType<SeekableByteChannel>();
        registerType<WritableByteChannel>();
    });
}

} // namespace jxx::nio::channels
