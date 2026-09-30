#include <atomic>
#include <chrono>
#include <thread>
#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.ServerSocket.h"
#include "net/jxx.net.Socket.h"
#include "nio/channels/jxx.nio.channels.SelectionKey.h"
#include "nio/channels/jxx.nio.channels.Selector.h"
#include "nio/channels/jxx.nio.channels.ServerSocketChannel.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
namespace {
using namespace std::chrono_literals;
::jxx::Ptr<::jxx::net::InetSocketAddress> loopback(::jxx::lang::jint port){return ::jxx::NEW<::jxx::net::InetSocketAddress>(::jxx::NEW<::jxx::lang::String>("127.0.0.1"),port);}
TEST(SelectorClientParity, CrossThreadWakeupUnblocksSelect){auto selector=::jxx::nio::channels::Selector::open();std::atomic<bool>started{false};std::atomic<long long>elapsed{0};std::thread selecting([&]{started=true;auto begin=std::chrono::steady_clock::now();EXPECT_EQ(0,selector->select(5000));elapsed=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-begin).count();});while(!started)std::this_thread::yield();std::this_thread::sleep_for(50ms);selector->wakeup();selecting.join();EXPECT_LT(elapsed.load(),1000);selector->close();}
TEST(SelectorClientParity, CancelRemovesKeyAndWakesSelector){auto selector=::jxx::nio::channels::Selector::open();auto server=::jxx::nio::channels::ServerSocketChannel::open();server->bind(loopback(0));server->configureBlocking(false);auto key=server->registerChannel(selector,::jxx::nio::channels::SelectionKey::OP_ACCEPT_);std::atomic<bool>returned{false};std::thread selecting([&]{selector->select(5000);returned=true;});std::this_thread::sleep_for(50ms);key->cancel();selecting.join();EXPECT_TRUE(returned.load());EXPECT_EQ(0,selector->keys()->size());server->close();selector->close();}
TEST(SelectorClientParity, ExistingSelectedKeyDoesNotInflateCount){auto selector=::jxx::nio::channels::Selector::open();auto server=::jxx::nio::channels::ServerSocketChannel::open();server->bind(loopback(0));server->configureBlocking(false);auto key=server->registerChannel(selector,::jxx::nio::channels::SelectionKey::OP_ACCEPT_);auto port=server->socket()->getLocalPort();auto client=::jxx::NEW<::jxx::net::Socket>();client->connect(loopback(port));ASSERT_GT(selector->select(3000),0);EXPECT_TRUE(key->isAcceptable());EXPECT_EQ(0,selector->selectNow());auto accepted=server->accept();if(accepted)accepted->close();client->close();server->close();selector->close();}
}
