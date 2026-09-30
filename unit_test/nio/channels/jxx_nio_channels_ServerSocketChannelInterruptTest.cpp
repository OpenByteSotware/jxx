#include <atomic>
#include <chrono>
#include <thread>
#include <gtest/gtest.h>
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Thread.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "nio/channels/jxx.nio.channels.AsynchronousCloseException.h"
#include "nio/channels/jxx.nio.channels.ClosedByInterruptException.h"
#include "nio/channels/jxx.nio.channels.ServerSocketChannel.h"
namespace {
using namespace std::chrono_literals;
enum class Result{None,Interrupted,AsyncClosed,Completed,Other};
struct Outcome{std::atomic<bool>entered{false};std::atomic<Result>result{Result::None};};
::jxx::Ptr<::jxx::net::InetSocketAddress> loopback(){return ::jxx::NEW<::jxx::net::InetSocketAddress>(::jxx::NEW<::jxx::lang::String>("127.0.0.1"),0);}
void acceptOnce(const ::jxx::Ptr<::jxx::nio::channels::ServerSocketChannel>&channel,Outcome&outcome){outcome.entered=true;try{(void)channel->accept();outcome.result=Result::Completed;}catch(const ::jxx::nio::channels::ClosedByInterruptException&){outcome.result=Result::Interrupted;}catch(const ::jxx::nio::channels::AsynchronousCloseException&){outcome.result=Result::AsyncClosed;}catch(...){outcome.result=Result::Other;}}
class AcceptTask final:public ::jxx::lang::ClassBase<AcceptTask,::jxx::lang::Object,::jxx::lang::Runnable>{public:AcceptTask(const ::jxx::Ptr<::jxx::nio::channels::ServerSocketChannel>&channel,Outcome&outcome,std::atomic<bool>*release=nullptr):channel_(channel),outcome_(outcome),release_(release){}void run()override{if(release_!=nullptr){outcome_.entered=true;while(!release_->load())std::this_thread::yield();}acceptOnce(channel_,outcome_);}private: ::jxx::Ptr<::jxx::nio::channels::ServerSocketChannel>channel_;Outcome&outcome_;std::atomic<bool>*release_;};
void waitEntered(Outcome&outcome){for(int i=0;i<500&&!outcome.entered;i++)std::this_thread::sleep_for(1ms);ASSERT_TRUE(outcome.entered.load());}
TEST(ServerSocketChannelInterruptParity, InterruptBlockingAcceptClosesChannel){auto channel=::jxx::nio::channels::ServerSocketChannel::open();channel->bind(loopback());Outcome outcome;auto thread=::jxx::NEW<::jxx::lang::Thread>(::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<AcceptTask>(channel,outcome)));thread->start();waitEntered(outcome);std::this_thread::sleep_for(50ms);thread->interrupt();thread->join(3000);EXPECT_EQ(Result::Interrupted,outcome.result.load());EXPECT_FALSE(channel->isOpen());EXPECT_TRUE(thread->isInterrupted());}
TEST(ServerSocketChannelInterruptParity, PreInterruptedThreadClosesBeforeAccept){auto channel=::jxx::nio::channels::ServerSocketChannel::open();channel->bind(loopback());Outcome outcome;std::atomic<bool>release{false};auto thread=::jxx::NEW<::jxx::lang::Thread>(::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<AcceptTask>(channel,outcome,&release)));thread->start();waitEntered(outcome);thread->interrupt();release=true;thread->join(3000);EXPECT_EQ(Result::Interrupted,outcome.result.load());EXPECT_FALSE(channel->isOpen());EXPECT_TRUE(thread->isInterrupted());}
TEST(ServerSocketChannelInterruptParity, AsynchronousCloseRemainsDistinct){auto channel=::jxx::nio::channels::ServerSocketChannel::open();channel->bind(loopback());Outcome outcome;std::thread accepting([&]{acceptOnce(channel,outcome);});waitEntered(outcome);std::this_thread::sleep_for(50ms);channel->close();accepting.join();EXPECT_EQ(Result::AsyncClosed,outcome.result.load());EXPECT_FALSE(channel->isOpen());}
}
