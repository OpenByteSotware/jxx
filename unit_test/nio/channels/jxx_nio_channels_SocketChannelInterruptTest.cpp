#include <atomic>
#include <chrono>
#include <stdexcept>
#include <thread>
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

namespace {

using namespace std::chrono_literals;
using Channel = ::jxx::nio::channels::SocketChannel;

enum class Result {
    None,
    Interrupted,
    AsyncClosed,
    Closed,
    Other,
    Completed
};

struct Outcome {
    std::atomic<bool> entered{false};
    std::atomic<Result> result{Result::None};
};

::jxx::Ptr<::jxx::net::InetSocketAddress> address(
    ::jxx::lang::jint port) {
    return ::jxx::NEW<::jxx::net::InetSocketAddress>(
        ::jxx::NEW<::jxx::lang::String>("127.0.0.1"), port);
}

class PendingLoopback final {
public:
    PendingLoopback() {
        ::jxx::net::internal::ensureNetworkInitialized();
        listener_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listener_ == ::jxx::net::internal::kInvalidSocket)
            throw std::runtime_error("listener socket");

        int one = 1;
        ::setsockopt(
            listener_, SOL_SOCKET, SO_REUSEADDR,
            reinterpret_cast<const char*>(&one), sizeof(one));

        sockaddr_in value{};
        value.sin_family = AF_INET;
        value.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        value.sin_port = 0;
        if (::bind(listener_, reinterpret_cast<sockaddr*>(&value),
                   sizeof(value)) != 0 ||
            ::listen(listener_, 1) != 0)
            throw std::runtime_error("listener bind");

        socklen_t length = sizeof(value);
        if (::getsockname(listener_, reinterpret_cast<sockaddr*>(&value),
                          &length) != 0)
            throw std::runtime_error("listener name");
        port_ = ntohs(value.sin_port);

        for (int index = 0; index < 64; ++index) {
            auto socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (socket == ::jxx::net::internal::kInvalidSocket) break;
#if defined(_WIN32)
            u_long mode = 1;
            ::ioctlsocket(socket, FIONBIO, &mode);
#else
            const int flags = ::fcntl(socket, F_GETFL, 0);
            ::fcntl(socket, F_SETFL, flags | O_NONBLOCK);
#endif
            ::connect(
                socket, reinterpret_cast<sockaddr*>(&value), sizeof(value));
            fillers_.push_back(socket);
        }
        std::this_thread::sleep_for(100ms);
    }

    ~PendingLoopback() {
        for (auto socket : fillers_)
            ::jxx::net::internal::closeNativeSocket(socket);
        ::jxx::net::internal::closeNativeSocket(listener_);
    }

    ::jxx::lang::jint port() const { return port_; }

private:
    ::jxx::net::internal::NativeSocket listener_ =
        ::jxx::net::internal::kInvalidSocket;
    std::vector<::jxx::net::internal::NativeSocket> fillers_;
    ::jxx::lang::jint port_ = 0;
};

void runConnect(
    const ::jxx::Ptr<Channel>& channel,
    ::jxx::lang::jint port,
    Outcome& outcome) {
    outcome.entered = true;
    try {
        channel->connect(address(port));
        outcome.result = Result::Completed;
    } catch (const ::jxx::nio::channels::ClosedByInterruptException&) {
        outcome.result = Result::Interrupted;
    } catch (const ::jxx::nio::channels::AsynchronousCloseException&) {
        outcome.result = Result::AsyncClosed;
    } catch (...) {
        outcome.result = Result::Other;
    }
}

void runRead(const ::jxx::Ptr<Channel>& channel, Outcome& outcome) {
    outcome.entered = true;
    try {
        channel->read(::jxx::nio::ByteBuffer::allocate(32));
        outcome.result = Result::Completed;
    } catch (const ::jxx::nio::channels::ClosedByInterruptException&) {
        outcome.result = Result::Interrupted;
    } catch (const ::jxx::nio::channels::AsynchronousCloseException&) {
        outcome.result = Result::AsyncClosed;
    } catch (...) {
        outcome.result = Result::Other;
    }
}

void runWrite(const ::jxx::Ptr<Channel>& channel, Outcome& outcome) {
    outcome.entered = true;
    try {
        for (;;) {
            const auto buffer =
                ::jxx::nio::ByteBuffer::allocate(1024 * 1024);
            while (buffer->hasRemaining())
                buffer->put(static_cast<::jxx::lang::jbyte>(1));
            buffer->flip();
            while (buffer->hasRemaining()) channel->write(buffer);
        }
    } catch (const ::jxx::nio::channels::ClosedByInterruptException&) {
        outcome.result = Result::Interrupted;
    } catch (const ::jxx::nio::channels::AsynchronousCloseException&) {
        outcome.result = Result::AsyncClosed;
    } catch (const ::jxx::nio::channels::ClosedChannelException&) {
        outcome.result = Result::Closed;
    } catch (...) {
        outcome.result = Result::Other;
    }
}

class OperationTask final
    : public ::jxx::lang::ClassBase<
          OperationTask,
          ::jxx::lang::Object,
          ::jxx::lang::Runnable> {
public:
    enum class Kind { Connect, Read, Write };

    OperationTask(
        Kind kind,
        const ::jxx::Ptr<Channel>& channel,
        Outcome& outcome,
        ::jxx::lang::jint port = 0)
        : kind(kind), channel(channel), outcome(outcome), port(port) {}

    void run() override {
        if (kind == Kind::Connect) runConnect(channel, port, outcome);
        else if (kind == Kind::Read) runRead(channel, outcome);
        else runWrite(channel, outcome);
    }

    Kind kind;
    ::jxx::Ptr<Channel> channel;
    Outcome& outcome;
    ::jxx::lang::jint port;
};

void waitEntered(Outcome& outcome) {
    for (int index = 0; index < 500 && !outcome.entered; ++index)
        std::this_thread::sleep_for(1ms);
    ASSERT_TRUE(outcome.entered.load());
}

struct ConnectedPair {
    ::jxx::Ptr<::jxx::net::ServerSocket> server =
        ::jxx::NEW<::jxx::net::ServerSocket>(0);
    ::jxx::Ptr<Channel> channel =
        Channel::open(address(server->getLocalPort()));
    ::jxx::Ptr<::jxx::net::Socket> peer = server->accept();

    ~ConnectedPair() {
        try { channel->close(); } catch (...) {}
        try { peer->close(); } catch (...) {}
        try { server->close(); } catch (...) {}
    }
};

TEST(
    SocketChannelInterruptParity,
    InterruptBlockingConnectOnSaturatedLoopbackBacklog) {
    PendingLoopback pending;
    auto channel = Channel::open();
    Outcome outcome;
    auto thread = ::jxx::NEW<::jxx::lang::Thread>(
        ::jxx::CAST<::jxx::lang::Runnable>(
            ::jxx::NEW<OperationTask>(
                OperationTask::Kind::Connect,
                channel,
                outcome,
                pending.port())));
    thread->start();
    waitEntered(outcome);
    std::this_thread::sleep_for(75ms);
    ASSERT_EQ(Result::None, outcome.result.load());
    thread->interrupt();
    thread->join(3000);
    EXPECT_EQ(Result::Interrupted, outcome.result.load());
    EXPECT_FALSE(channel->isOpen());
    EXPECT_TRUE(thread->isInterrupted());
}

TEST(
    SocketChannelInterruptParity,
    CloseDuringContinuousBlockingWritesIsDistinctFromInterrupt) {
    ConnectedPair pair;
    pair.channel->socket()->setSendBufferSize(1024);
    pair.peer->setReceiveBufferSize(1024);
    Outcome outcome;
    std::thread worker([&] { runWrite(pair.channel, outcome); });
    waitEntered(outcome);
    std::this_thread::sleep_for(100ms);
    pair.channel->close();
    worker.join();

    const auto result = outcome.result.load();
    EXPECT_TRUE(result == Result::AsyncClosed || result == Result::Closed);
    EXPECT_NE(Result::Interrupted, result);
    EXPECT_NE(Result::Other, result);
    EXPECT_FALSE(pair.channel->isOpen());
}

TEST(SocketChannelInterruptParity, InterruptBlockingRead) {
    ConnectedPair pair;
    Outcome outcome;
    auto thread = ::jxx::NEW<::jxx::lang::Thread>(
        ::jxx::CAST<::jxx::lang::Runnable>(
            ::jxx::NEW<OperationTask>(
                OperationTask::Kind::Read, pair.channel, outcome)));
    thread->start();
    waitEntered(outcome);
    std::this_thread::sleep_for(50ms);
    thread->interrupt();
    thread->join(3000);
    EXPECT_EQ(Result::Interrupted, outcome.result.load());
    EXPECT_TRUE(thread->isInterrupted());
}

TEST(
    SocketChannelInterruptParity,
    AsynchronousCloseDuringBlockingReadIsDistinct) {
    ConnectedPair pair;
    Outcome outcome;
    std::thread worker([&] { runRead(pair.channel, outcome); });
    waitEntered(outcome);
    std::this_thread::sleep_for(50ms);
    pair.channel->close();
    worker.join();
    EXPECT_EQ(Result::AsyncClosed, outcome.result.load());
}

TEST(SocketChannelInterruptParity, InterruptBlockingWrite) {
    ConnectedPair pair;
    pair.channel->socket()->setSendBufferSize(1024);
    Outcome outcome;
    auto thread = ::jxx::NEW<::jxx::lang::Thread>(
        ::jxx::CAST<::jxx::lang::Runnable>(
            ::jxx::NEW<OperationTask>(
                OperationTask::Kind::Write, pair.channel, outcome)));
    thread->start();
    waitEntered(outcome);
    std::this_thread::sleep_for(100ms);
    thread->interrupt();
    thread->join(3000);
    EXPECT_EQ(Result::Interrupted, outcome.result.load());
    EXPECT_TRUE(thread->isInterrupted());
}

TEST(
    SocketChannelInterruptParity,
    AsynchronousCloseDuringBlockingWriteIsDistinct) {
    ConnectedPair pair;
    pair.channel->socket()->setSendBufferSize(1024);
    Outcome outcome;
    std::thread worker([&] { runWrite(pair.channel, outcome); });
    waitEntered(outcome);
    std::this_thread::sleep_for(100ms);
    pair.channel->close();
    worker.join();

    const auto result = outcome.result.load();
    EXPECT_TRUE(result == Result::AsyncClosed || result == Result::Closed);
    EXPECT_NE(Result::Interrupted, result);
    EXPECT_NE(Result::Other, result);
    EXPECT_FALSE(pair.channel->isOpen());
}

} // namespace
