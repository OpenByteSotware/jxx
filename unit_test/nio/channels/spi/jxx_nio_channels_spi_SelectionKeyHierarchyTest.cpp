#include <type_traits>
#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "nio/channels/jxx.nio.channels.SelectableChannel.h"
#include "nio/channels/jxx.nio.channels.SelectionKey.h"
#include "nio/channels/jxx.nio.channels.Selector.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
#include "nio/channels/spi/jxx.nio.channels.spi.AbstractSelectionKey.h"
namespace {
TEST(SelectionKeyHierarchyParity, PublicSelectionKeyIsAbstract){EXPECT_TRUE(std::is_abstract_v<::jxx::nio::channels::SelectionKey>);EXPECT_TRUE(std::is_abstract_v<::jxx::nio::channels::spi::AbstractSelectionKey>);}
TEST(SelectionKeyHierarchyParity, RegisteredKeyUsesSpiHierarchy){auto selector=::jxx::nio::channels::Selector::open();auto channel=::jxx::nio::channels::SocketChannel::open();channel->configureBlocking(false);auto key=channel->register_(selector,::jxx::nio::channels::SelectionKey::OP_CONNECT_);ASSERT_NE(nullptr,key);EXPECT_NE(nullptr,::jxx::CAST<::jxx::nio::channels::spi::AbstractSelectionKey>(key));EXPECT_EQ(channel,key->channel());EXPECT_EQ(selector,key->selector());key->cancel();channel->close();selector->close();}
TEST(SelectionKeyHierarchyParity, AttachmentLivesOnPublicSelectionKey){auto selector=::jxx::nio::channels::Selector::open();auto channel=::jxx::nio::channels::SocketChannel::open();channel->configureBlocking(false);auto key=channel->register_(selector,::jxx::nio::channels::SelectionKey::OP_CONNECT_);auto first=::jxx::NEW<::jxx::lang::String>("first");auto second=::jxx::NEW<::jxx::lang::String>("second");EXPECT_EQ(nullptr,key->attach(first));EXPECT_EQ(first,key->attachment());EXPECT_EQ(first,key->attach(second));EXPECT_EQ(second,key->attachment());key->cancel();channel->close();selector->close();}
TEST(SelectionKeyHierarchyParity, CancelIsIdempotentAndInvalidatesImmediately){auto selector=::jxx::nio::channels::Selector::open();auto channel=::jxx::nio::channels::SocketChannel::open();channel->configureBlocking(false);auto key=channel->register_(selector,::jxx::nio::channels::SelectionKey::OP_CONNECT_);EXPECT_TRUE(key->isValid());key->cancel();EXPECT_FALSE(key->isValid());EXPECT_NO_THROW(key->cancel());channel->close();selector->close();}
}
