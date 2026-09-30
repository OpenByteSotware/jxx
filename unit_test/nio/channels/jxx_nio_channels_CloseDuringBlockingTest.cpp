#include <atomic>
#include <chrono>
#include <thread>
#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "nio/channels/jxx.nio.channels.AsynchronousCloseException.h"
#include "nio/channels/jxx.nio.channels.Selector.h"
#include "nio/channels/jxx.nio.channels.ServerSocketChannel.h"
namespace {
using namespace std::chrono_literals;
::jxx::Ptr<::jxx::net::InetSocketAddress> loopback(::jxx::lang::jint port){return ::jxx::NEW<::jxx::net::InetSocketAddress>(::jxx::NEW<::jxx::lang::String>("127.0.0.1"),port);}
void waitEntered(const std::atomic<bool>&entered){for(int i=0;i<500&&!entered.load();++i)std::this_thread::sleep_for(1ms);ASSERT_TRUE(entered.load());}
TEST(SelectorCloseParity, CloseDuringSelectUnblocksDeterministically){auto selector=::jxx::nio::channels::Selector::open();std::atomic<bool>entered{false};std::atomic<bool>returned{false};std::atomic<::jxx::lang::jint>result{-1};std::thread selecting([&]{entered=true;try{result=selector->select();returned=true;}catch(...){returned=true;}});waitEntered(entered);std::this_thread::sleep_for(50ms);auto started=std::chrono::steady_clock::now();selector->close();selecting.join();auto elapsed=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-started).count();EXPECT_TRUE(returned.load());EXPECT_EQ(0,result.load());EXPECT_FALSE(selector->isOpen());EXPECT_LT(elapsed,1000);}
TEST(ServerSocketChannelCloseParity, CloseDuringAcceptThrowsAsynchronousClose){auto channel=::jxx::nio::channels::ServerSocketChannel::open();channel->bind(loopback(0));std::atomic<bool>entered{false};std::atomic<bool>asyncClosed{false};std::atomic<bool>returnedNormally{false};std::thread accepting([&]{entered=true;try{(void)channel->accept();returnedNormally=true;}catch(const ::jxx::nio::channels::AsynchronousCloseException&){asyncClosed=true;}catch(...){}});waitEntered(entered);std::this_thread::sleep_for(50ms);auto started=std::chrono::steady_clock::now();channel->close();accepting.join();auto elapsed=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-started).count();EXPECT_TRUE(asyncClosed.load());EXPECT_FALSE(returnedNormally.load());EXPECT_FALSE(channel->isOpen());EXPECT_LT(elapsed,1000);}
}
