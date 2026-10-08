#include <chrono>
#include <thread>
#include <gtest/gtest.h>
#include "lang/jxx.lang.Boolean.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.ServerSocket.h"
#include "net/jxx.net.Socket.h"
#include "net/jxx.net.StandardSocketOptions.h"
#include "nio/channels/jxx.nio.channels.ServerSocketChannel.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
namespace
{
	::jxx::Ptr<::jxx::net::InetSocketAddress> loopback(::jxx::lang::jint p)
	{
		return ::jxx::NEW<::jxx::net::InetSocketAddress>(::jxx::NEW<::jxx::lang::String>("127.0.0.1"), p);
	}
	TEST(ServerSocketChannelParity, SocketViewSharesCloseState)
	{
		auto c = ::jxx::nio::channels::ServerSocketChannel::open(); auto s = c->socket(); ASSERT_NE(nullptr, s); EXPECT_EQ(c, s->getChannel()); s->close(); EXPECT_FALSE(c->isOpen());
	}
	TEST(ServerSocketChannelParity, NonBlockingAcceptReturnsNull)
	{
		auto c = ::jxx::nio::channels::ServerSocketChannel::open();
		c->bind(loopback(0)); c->configureBlocking(false); 
		EXPECT_EQ(nullptr, c->accept()); c->close();
	}
	TEST(ServerSocketChannelParity, AcceptReturnsConnectedSharedChannel)
	{
		auto s = ::jxx::nio::channels::ServerSocketChannel::open();
		s->bind(loopback(0)); auto p = s->socket()->getLocalPort(); std::thread t([p]
	   {
				  auto c = ::jxx::NEW<::jxx::net::Socket>(); 
				  c->connect(loopback(p)); std::this_thread::sleep_for(std::chrono::milliseconds(20));
				  c->close();
	   }); 
		
		auto a = s->accept(); ASSERT_NE(nullptr, a);
		EXPECT_TRUE(a->isConnected()); EXPECT_EQ(a, a->socket()->getChannel()); 
		a->close(); s->close(); t.join();
	}
	TEST(ServerSocketChannelParity, SupportsServerOptions)
	{
		auto c = ::jxx::nio::channels::ServerSocketChannel::open(); 
		c->bind(loopback(0)); c->setOption(::jxx::net::StandardSocketOptions::SO_REUSEADDR_, 
			::jxx::CAST<::jxx::lang::Object>(::jxx::lang::Boolean::valueOf(true)));
		auto v = ::jxx::CAST<::jxx::lang::Boolean>(c->getOption(::jxx::net::StandardSocketOptions::SO_REUSEADDR_));
		ASSERT_NE(nullptr, v);
		EXPECT_TRUE(v->booleanValue()); c->close();
	}
} // namespace
