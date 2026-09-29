#include <gtest/gtest.h>
#include <type_traits>
#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "nio/channels/jxx.nio.channels.AlreadyBoundException.h"
#include "nio/channels/jxx.nio.channels.AlreadyConnectedException.h"
#include "nio/channels/jxx.nio.channels.ClosedChannelException.h"
#include "nio/channels/jxx.nio.channels.ConnectionPendingException.h"
#include "nio/channels/jxx.nio.channels.NoConnectionPendingException.h"
#include "nio/channels/jxx.nio.channels.NotYetConnectedException.h"
#include "nio/channels/jxx.nio.channels.UnresolvedAddressException.h"
#include "nio/channels/jxx.nio.channels.UnsupportedAddressTypeException.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
namespace {
TEST(SocketChannelStateExceptionParityTest, Hierarchy) {
 EXPECT_TRUE((std::is_base_of_v<::jxx::lang::IllegalStateException,::jxx::nio::channels::AlreadyConnectedException>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::lang::IllegalStateException,::jxx::nio::channels::ConnectionPendingException>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::lang::IllegalStateException,::jxx::nio::channels::NoConnectionPendingException>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::lang::IllegalStateException,::jxx::nio::channels::NotYetConnectedException>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::lang::IllegalStateException,::jxx::nio::channels::AlreadyBoundException>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::lang::IllegalArgumentException,::jxx::nio::channels::UnsupportedAddressTypeException>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::lang::IllegalArgumentException,::jxx::nio::channels::UnresolvedAddressException>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::io::IOException,::jxx::nio::channels::ClosedChannelException>));
}
TEST(SocketChannelStateExceptionParityTest, UnconnectedAndClosedStates) {
 const auto c=::jxx::nio::channels::SocketChannel::open();
 const auto b=::jxx::nio::ByteBuffer::allocate(4);
 EXPECT_THROW(c->read(b),::jxx::nio::channels::NotYetConnectedException);
 EXPECT_THROW(c->finishConnect(),::jxx::nio::channels::NoConnectionPendingException);
 c->close();
 EXPECT_THROW(c->write(b),::jxx::nio::channels::ClosedChannelException);
}
}
