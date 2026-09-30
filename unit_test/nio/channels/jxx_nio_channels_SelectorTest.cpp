#include <chrono>
#include <thread>
#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.ServerSocket.h"
#include "net/jxx.net.Socket.h"
#include "nio/channels/jxx.nio.channels.CancelledKeyException.h"
#include "nio/channels/jxx.nio.channels.SelectionKey.h"
#include "nio/channels/jxx.nio.channels.Selector.h"
#include "nio/channels/jxx.nio.channels.ServerSocketChannel.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
namespace {
::jxx::Ptr<::jxx::net::InetSocketAddress> loopback(::jxx::lang::jint port){return ::jxx::NEW<::jxx::net::InetSocketAddress>(::jxx::NEW<::jxx::lang::String>("127.0.0.1"),port);}
TEST(SelectorParity, AcceptReadinessAndAttachment){auto selector=::jxx::nio::channels::Selector::open();auto server=::jxx::nio::channels::ServerSocketChannel::open();server->bind(loopback(0));server->configureBlocking(false);auto attachment=::jxx::NEW<::jxx::lang::String>("accept");auto key=server->registerChannel(selector,::jxx::nio::channels::SelectionKey::OP_ACCEPT_,attachment);auto port=server->socket()->getLocalPort();std::thread client([port]{auto s=::jxx::NEW<::jxx::net::Socket>();s->connect(loopback(port));std::this_thread::sleep_for(std::chrono::milliseconds(30));s->close();});ASSERT_GT(selector->select(3000),0);EXPECT_TRUE(key->isAcceptable());EXPECT_EQ(attachment,key->attachment());auto accepted=server->accept();ASSERT_NE(nullptr,accepted);accepted->close();server->close();selector->close();client.join();}
TEST(SelectorParity, CancelledKeyBecomesInvalid){auto selector=::jxx::nio::channels::Selector::open();auto server=::jxx::nio::channels::ServerSocketChannel::open();server->bind(loopback(0));server->configureBlocking(false);auto key=server->registerChannel(selector,::jxx::nio::channels::SelectionKey::OP_ACCEPT_);EXPECT_TRUE(key->isValid());key->cancel();EXPECT_FALSE(key->isValid());EXPECT_THROW(key->readyOps(),::jxx::nio::channels::CancelledKeyException);server->close();selector->close();}
TEST(SelectorParity, WakeupBeforeSelectReturnsImmediately){auto selector=::jxx::nio::channels::Selector::open();selector->wakeup();auto started=std::chrono::steady_clock::now();EXPECT_EQ(0,selector->select(3000));auto elapsed=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-started).count();EXPECT_LT(elapsed,500);selector->close();}
TEST(SelectorParity, BlockingChannelRegistrationIsRejected){auto selector=::jxx::nio::channels::Selector::open();auto server=::jxx::nio::channels::ServerSocketChannel::open();server->bind(loopback(0));EXPECT_THROW(server->registerChannel(selector,::jxx::nio::channels::SelectionKey::OP_ACCEPT_),::jxx::lang::IllegalStateException);server->close();selector->close();}
}
