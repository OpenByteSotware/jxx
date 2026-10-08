#include <atomic>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <vector>
#include <gtest/gtest.h>
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Thread.h"
#include "net/internal/jxx.net.internal.NetPlatform.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.ServerSocket.h"
#include "net/jxx.net.Socket.h"
#include "nio/jxx.nio.ByteBuffer.h"
#include "nio/channels/jxx.nio.channels.AsynchronousCloseException.h"
#include "nio/channels/jxx.nio.channels.ClosedByInterruptException.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
#if !defined(_WIN32)
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif
namespace
{
	using namespace std::chrono_literals;
	using Channel = ::jxx::nio::channels::SocketChannel;
	enum class Result
	{
		None, Interrupted, AsyncClosed, Other, Completed
	};
	struct Outcome
	{
		std::atomic<bool>entered{ false }; std::atomic<Result>result{ Result::None };
	};
	::jxx::Ptr<::jxx::net::InetSocketAddress> address(::jxx::lang::jint port)
	{
		return ::jxx::NEW<::jxx::net::InetSocketAddress>(::jxx::NEW<::jxx::lang::String>("127.0.0.1"), port);
	}
	class PendingLoopback final
	{
	public:PendingLoopback()
	{
		::jxx::net::internal::ensureNetworkInitialized(); listener_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); if (listener_ == ::jxx::net::internal::kInvalidSocket)throw std::runtime_error("listener socket"); int one = 1; ::setsockopt(listener_, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&one), sizeof(one)); sockaddr_in a{}; a.sin_family = AF_INET; a.sin_addr.s_addr = htonl(INADDR_LOOPBACK); a.sin_port = 0; if (::bind(listener_, reinterpret_cast<sockaddr*>(&a), sizeof(a)) != 0 || ::listen(listener_, 1) != 0)throw std::runtime_error("listener bind"); socklen_t n = sizeof(a); if (::getsockname(listener_, reinterpret_cast<sockaddr*>(&a), &n) != 0)throw std::runtime_error("listener name"); port_ = ntohs(a.sin_port); for (int i = 0; i < 64; ++i) {
			auto s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); if (s == ::jxx::net::internal::kInvalidSocket)break;
#if defined(_WIN32)
			u_long mode = 1; ::ioctlsocket(s, FIONBIO, &mode);
#else
			int flags = ::fcntl(s, F_GETFL, 0); ::fcntl(s, F_SETFL, flags | O_NONBLOCK);
#endif
			::connect(s, reinterpret_cast<sockaddr*>(&a), sizeof(a)); fillers_.push_back(s);
		}std::this_thread::sleep_for(100ms);
	}~PendingLoopback()
	{
		for (auto s : fillers_)::jxx::net::internal::closeNativeSocket(s); ::jxx::net::internal::closeNativeSocket(listener_);
	}::jxx::lang::jint port()const
	{
		return port_;
	}private: ::jxx::net::internal::NativeSocket listener_ = ::jxx::net::internal::kInvalidSocket; std::vector<::jxx::net::internal::NativeSocket>fillers_; ::jxx::lang::jint port_ = 0;
	};
	void runConnect(const ::jxx::Ptr<Channel>& c, ::jxx::lang::jint port, Outcome& o)
	{
		o.entered = true; try {
			c->connect(address(port)); o.result = Result::Completed;
		}
		catch (const ::jxx::nio::channels::ClosedByInterruptException&) {
			o.result = Result::Interrupted;
		}
		catch (const ::jxx::nio::channels::AsynchronousCloseException&) {
			o.result = Result::AsyncClosed;
		}
		catch (...) {
			o.result = Result::Other;
		}
	}
	void runRead(const ::jxx::Ptr<Channel>& c, Outcome& o)
	{
		o.entered = true; try {
			c->read(::jxx::nio::ByteBuffer::allocate(32)); o.result = Result::Completed;
		}
		catch (const ::jxx::nio::channels::ClosedByInterruptException&) {
			o.result = Result::Interrupted;
		}
		catch (const ::jxx::nio::channels::AsynchronousCloseException&) {
			o.result = Result::AsyncClosed;
		}
		catch (...) {
			o.result = Result::Other;
		}
	}
	void runWrite(const ::jxx::Ptr<Channel>& c, Outcome& o)
	{
		o.entered = true; try {
			for (;;) {
				auto b = ::jxx::nio::ByteBuffer::allocate(1024 * 1024); while (b->hasRemaining())b->put(static_cast<::jxx::lang::jbyte>(1)); b->flip(); while (b->hasRemaining())c->write(b);
			}
		}
		catch (const ::jxx::nio::channels::ClosedByInterruptException&) {
			o.result = Result::Interrupted;
		}
		catch (const ::jxx::nio::channels::AsynchronousCloseException&) {
			o.result = Result::AsyncClosed;
		}
		catch (...) {
			o.result = Result::Other;
		}
	}
	class OperationTask final :public ::jxx::lang::ClassBase<OperationTask, ::jxx::lang::Object, ::jxx::lang::Runnable>
	{
	public:enum class Kind
	{
		Connect, Read, Write
	}; OperationTask(Kind k, const ::jxx::Ptr<Channel>& c, Outcome& o, ::jxx::lang::jint p = 0) :kind(k), channel(c), outcome(o), port(p)
	{
	}void run()override
	{
		if (kind == Kind::Connect)runConnect(channel, port, outcome); else if (kind == Kind::Read)runRead(channel, outcome); else runWrite(channel, outcome);
	}Kind kind; ::jxx::Ptr<Channel>channel; Outcome& outcome; ::jxx::lang::jint port;
	};
	void waitEntered(Outcome& o)
	{
		for (int i = 0; i < 500 && !o.entered; i++)std::this_thread::sleep_for(1ms); ASSERT_TRUE(o.entered.load());
	}
	struct ConnectedPair
	{
		::jxx::Ptr<::jxx::net::ServerSocket>server = ::jxx::NEW<::jxx::net::ServerSocket>(0); ::jxx::Ptr<Channel>channel = Channel::open(address(server->getLocalPort())); ::jxx::Ptr<::jxx::net::Socket>peer = server->accept(); ~ConnectedPair()
		{
			try {
				channel->close();
			}
			catch (...) {
			}try {
				peer->close();
			}
			catch (...) {
			}try {
				server->close();
			}
			catch (...) {
			}
		}
	};
	TEST(SocketChannelInterruptParity, InterruptBlockingConnectOnSaturatedLoopbackBacklog)
	{
		PendingLoopback pending; auto c = Channel::open(); Outcome o; auto t = ::jxx::NEW<::jxx::lang::Thread>(::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<OperationTask>(OperationTask::Kind::Connect, c, o, pending.port()))); t->start(); waitEntered(o); std::this_thread::sleep_for(75ms); ASSERT_EQ(Result::None, o.result.load()); t->interrupt(); t->join(3000); EXPECT_EQ(Result::Interrupted, o.result.load()); EXPECT_FALSE(c->isOpen()); EXPECT_TRUE(t->isInterrupted());
	}
	TEST(SocketChannelInterruptParity, AsynchronousCloseDuringBlockingConnectIsDistinct)
	{
		PendingLoopback pending; auto c = Channel::open(); Outcome o; std::thread worker([&]
	   {
				  runConnect(c, pending.port(), o);
	   }); waitEntered(o); std::this_thread::sleep_for(75ms); ASSERT_EQ(Result::None, o.result.load()); c->close(); worker.join(); EXPECT_EQ(Result::AsyncClosed, o.result.load());
	}
	TEST(SocketChannelInterruptParity, InterruptBlockingRead)
	{
		ConnectedPair p; Outcome o; auto t = ::jxx::NEW<::jxx::lang::Thread>(::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<OperationTask>(OperationTask::Kind::Read, p.channel, o))); t->start(); waitEntered(o); std::this_thread::sleep_for(50ms); t->interrupt(); t->join(3000); EXPECT_EQ(Result::Interrupted, o.result.load()); EXPECT_TRUE(t->isInterrupted());
	}
	TEST(SocketChannelInterruptParity, AsynchronousCloseDuringBlockingReadIsDistinct)
	{
		ConnectedPair p; Outcome o; std::thread worker([&]
	   {
				  runRead(p.channel, o);
	   }); waitEntered(o); std::this_thread::sleep_for(50ms); p.channel->close(); worker.join(); EXPECT_EQ(Result::AsyncClosed, o.result.load());
	}
	TEST(SocketChannelInterruptParity, InterruptBlockingWrite)
	{
		ConnectedPair p; p.channel->socket()->setSendBufferSize(1024); Outcome o; auto t = ::jxx::NEW<::jxx::lang::Thread>(::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<OperationTask>(OperationTask::Kind::Write, p.channel, o))); t->start(); waitEntered(o); std::this_thread::sleep_for(100ms); t->interrupt(); t->join(3000); EXPECT_EQ(Result::Interrupted, o.result.load()); EXPECT_TRUE(t->isInterrupted());
	}
	TEST(SocketChannelInterruptParity, AsynchronousCloseDuringBlockingWriteIsDistinct)
	{
		ConnectedPair p; p.channel->socket()->setSendBufferSize(1024); Outcome o; std::thread worker([&]
	   {
				  runWrite(p.channel, o);
	   }); waitEntered(o); std::this_thread::sleep_for(100ms); p.channel->close(); worker.join(); EXPECT_EQ(Result::AsyncClosed, o.result.load());
	}
}
