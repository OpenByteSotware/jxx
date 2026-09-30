#include <atomic>
#include <chrono>
#include <thread>
#include <gtest/gtest.h>
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Thread.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.ServerSocket.h"
#include "net/jxx.net.Socket.h"
#include "nio/jxx.nio.ByteBuffer.h"
#include "nio/channels/jxx.nio.channels.ClosedByInterruptException.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
namespace {
using namespace std::chrono_literals;
::jxx::Ptr<::jxx::net::InetSocketAddress> address(const char* host,::jxx::lang::jint port){return ::jxx::NEW<::jxx::net::InetSocketAddress>(::jxx::NEW<::jxx::lang::String>(host),port);}
struct Outcome { std::atomic<bool> entered{false}; std::atomic<bool> interrupted{false}; std::atomic<bool> finished{false}; };
class ConnectTask final:public ::jxx::lang::ClassBase<ConnectTask,::jxx::lang::Object,::jxx::lang::Runnable>{public:ConnectTask(const ::jxx::Ptr<::jxx::nio::channels::SocketChannel>&c,Outcome&o):channel(c),outcome(o){}void run()override{outcome.entered=true;try{channel->connect(address("192.0.2.1",65000));}catch(const ::jxx::nio::channels::ClosedByInterruptException&){outcome.interrupted=true;}catch(...){ }outcome.finished=true;}::jxx::Ptr<::jxx::nio::channels::SocketChannel>channel;Outcome&outcome;};
class ReadTask final:public ::jxx::lang::ClassBase<ReadTask,::jxx::lang::Object,::jxx::lang::Runnable>{public:ReadTask(const ::jxx::Ptr<::jxx::nio::channels::SocketChannel>&c,Outcome&o):channel(c),outcome(o){}void run()override{outcome.entered=true;try{channel->read(::jxx::nio::ByteBuffer::allocate(32));}catch(const ::jxx::nio::channels::ClosedByInterruptException&){outcome.interrupted=true;}catch(...){ }outcome.finished=true;}::jxx::Ptr<::jxx::nio::channels::SocketChannel>channel;Outcome&outcome;};
class WriteTask final:public ::jxx::lang::ClassBase<WriteTask,::jxx::lang::Object,::jxx::lang::Runnable>{public:WriteTask(const ::jxx::Ptr<::jxx::nio::channels::SocketChannel>&c,Outcome&o):channel(c),outcome(o){}void run()override{outcome.entered=true;try{for(;;){auto b=::jxx::nio::ByteBuffer::allocate(1024*1024);while(b->hasRemaining())b->put(static_cast<::jxx::lang::jbyte>(1));b->flip();while(b->hasRemaining())channel->write(b);}}catch(const ::jxx::nio::channels::ClosedByInterruptException&){outcome.interrupted=true;}catch(...){ }outcome.finished=true;}::jxx::Ptr<::jxx::nio::channels::SocketChannel>channel;Outcome&outcome;};
void waitEntered(Outcome&o){for(int i=0;i<200&&!o.entered;i++)std::this_thread::sleep_for(1ms);ASSERT_TRUE(o.entered.load());}
TEST(SocketChannelInterruptParity, InterruptBlockingConnectClosesChannel){auto c=::jxx::nio::channels::SocketChannel::open();Outcome o;auto t=::jxx::NEW<::jxx::lang::Thread>(::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<ConnectTask>(c,o)));t->start();waitEntered(o);std::this_thread::sleep_for(50ms);t->interrupt();t->join(3000);EXPECT_TRUE(o.finished.load());EXPECT_TRUE(o.interrupted.load());EXPECT_FALSE(c->isOpen());EXPECT_TRUE(t->isInterrupted());}
TEST(SocketChannelInterruptParity, InterruptBlockingReadClosesChannel){auto server=::jxx::NEW<::jxx::net::ServerSocket>(0);auto c=::jxx::nio::channels::SocketChannel::open(address("127.0.0.1",server->getLocalPort()));auto peer=server->accept();Outcome o;auto t=::jxx::NEW<::jxx::lang::Thread>(::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<ReadTask>(c,o)));t->start();waitEntered(o);std::this_thread::sleep_for(50ms);t->interrupt();t->join(3000);EXPECT_TRUE(o.interrupted.load());EXPECT_FALSE(c->isOpen());EXPECT_TRUE(t->isInterrupted());peer->close();server->close();}
TEST(SocketChannelInterruptParity, InterruptBlockingWriteClosesChannel){auto server=::jxx::NEW<::jxx::net::ServerSocket>(0);auto c=::jxx::nio::channels::SocketChannel::open(address("127.0.0.1",server->getLocalPort()));auto peer=server->accept();c->socket()->setSendBufferSize(1024);Outcome o;auto t=::jxx::NEW<::jxx::lang::Thread>(::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<WriteTask>(c,o)));t->start();waitEntered(o);std::this_thread::sleep_for(100ms);t->interrupt();t->join(3000);EXPECT_TRUE(o.interrupted.load());EXPECT_FALSE(c->isOpen());EXPECT_TRUE(t->isInterrupted());peer->close();server->close();}
}
