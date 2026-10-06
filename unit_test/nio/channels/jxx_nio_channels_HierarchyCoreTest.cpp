#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "nio/channels/jxx.nio.channels.IllegalBlockingModeException.h"
#include "nio/channels/jxx.nio.channels.SelectableChannel.h"
#include "nio/channels/jxx.nio.channels.SelectionKey.h"
#include "nio/channels/jxx.nio.channels.Selector.h"
#include "nio/channels/jxx.nio.channels.ServerSocketChannel.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
#include "nio/channels/spi/jxx.nio.channels.spi.AbstractInterruptibleChannel.h"
#include "nio/channels/spi/jxx.nio.channels.spi.AbstractSelectableChannel.h"
namespace
{
	TEST(HierarchyCoreParity, SpiPackageAndAssignableHierarchy)
	{
		auto c = ::jxx::nio::channels::SocketChannel::open(); 
		EXPECT_NE(nullptr, ::jxx::CAST<::jxx::nio::channels::SelectableChannel>(c)); 
		EXPECT_NE(nullptr, ::jxx::CAST<::jxx::nio::channels::spi::AbstractSelectableChannel>(c)); 
		EXPECT_NE(nullptr, ::jxx::CAST<::jxx::nio::channels::spi::AbstractInterruptibleChannel>(c));
		c->close();
	}
	TEST(HierarchyCoreParity, ConfigureBlockingReturnsSelectableChannel)
	{
		auto c = ::jxx::nio::channels::SocketChannel::open(); 
		::jxx::Ptr<::jxx::nio::channels::SelectableChannel> selectable = c->configureBlocking(false);
		EXPECT_EQ(c.get(), selectable.get()); EXPECT_FALSE(selectable->isBlocking()); c->close();
	}
	TEST(HierarchyCoreParity, MultipleSelectorRegistrationsAndKeyLookup)
	{
		auto c = ::jxx::nio::channels::SocketChannel::open(); c->configureBlocking(false); 
		auto a = ::jxx::nio::channels::Selector::open(); auto b = ::jxx::nio::channels::Selector::open(); 
		auto ka = c->register_(a, ::jxx::nio::channels::SelectionKey::OP_CONNECT_); 
		auto kb = c->register_(b, ::jxx::nio::channels::SelectionKey::OP_CONNECT_); 
		EXPECT_TRUE(c->isRegistered()); EXPECT_EQ(ka, c->keyFor(a)); 
		EXPECT_EQ(kb, c->keyFor(b)); ka->cancel(); kb->cancel();
		EXPECT_FALSE(c->isRegistered());
		c->close(); a->close(); 
		b->close();
	}
	TEST(HierarchyCoreParity, RegisteredChannelCannotReturnToBlockingMode)
	{
		auto c = ::jxx::nio::channels::SocketChannel::open(); 
		c->configureBlocking(false); 
		auto s = ::jxx::nio::channels::Selector::open(); 
		auto k = c->register_(s, ::jxx::nio::channels::SelectionKey::OP_CONNECT_);
		EXPECT_THROW((void)c->configureBlocking(true), 
			::jxx::nio::channels::IllegalBlockingModeException);
		k->cancel(); EXPECT_NO_THROW((void)c->configureBlocking(true)); 
		c->close(); s->close();
	}

	TEST(HierarchyCoreParity, CloseCancelsEveryRegistration)
	{
		auto c = ::jxx::nio::channels::SocketChannel::open(); 
		c->configureBlocking(false); 
		auto a = ::jxx::nio::channels::Selector::open(); 
		auto b = ::jxx::nio::channels::Selector::open(); 
		auto ka = c->register_(a, ::jxx::nio::channels::SelectionKey::OP_CONNECT_); 
		auto kb = c->register_(b, ::jxx::nio::channels::SelectionKey::OP_CONNECT_);
		c->close(); EXPECT_FALSE(ka->isValid()); EXPECT_FALSE(kb->isValid());
		EXPECT_FALSE(c->isOpen()); a->close(); b->close();
	}

	TEST(HierarchyCoreParity, OpenAndDestroyWithoutExplicitClose)
	{
		auto channel =
			::jxx::nio::channels::SocketChannel::open();

		ASSERT_NE(nullptr, channel);
	}

	TEST(HierarchyCoreParity, OpenAndCloseOnly)
	{
		auto channel =
			::jxx::nio::channels::SocketChannel::open();

		ASSERT_NE(nullptr, channel);
		EXPECT_NO_THROW(channel->close());
		EXPECT_FALSE(channel->isOpen());
	}

	TEST(HierarchyCoreParity, HierarchyCastsWithoutClose)
	{
		auto channel =
			::jxx::nio::channels::SocketChannel::open();

		ASSERT_NE(
			nullptr,
			::jxx::CAST<
				::jxx::nio::channels::SelectableChannel>(
					channel));

		ASSERT_NE(
			nullptr,
			::jxx::CAST<
				::jxx::nio::channels::spi::
					AbstractSelectableChannel>(
						channel));

		ASSERT_NE(
			nullptr,
			::jxx::CAST<
				::jxx::nio::channels::spi::
					AbstractInterruptibleChannel>(
						channel));
	}
}
