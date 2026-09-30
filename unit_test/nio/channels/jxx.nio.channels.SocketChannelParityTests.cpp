#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>



#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.OutputStream.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.ServerSocket.h"
#include "net/jxx.net.Socket.h"
#include "nio/channels/jxx.nio.channels.AsynchronousCloseException.h"
#include "nio/channels/jxx.nio.channels.ClosedChannelException.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
#include "nio/jxx.nio.ByteBuffer.h"

namespace {

using Clock = std::chrono::steady_clock;

::jxx::Ptr<::jxx::net::InetSocketAddress> loopback(
    ::jxx::lang::jint port) {
    return ::jxx::NEW<::jxx::net::InetSocketAddress>(
        ::jxx::NEW<::jxx::lang::String>("127.0.0.1"), port);
}

TEST(SocketChannelParity, NonBlockingReadReturnsZeroThenDataThenEndOfStream) {
    const auto server = ::jxx::NEW<::jxx::net::ServerSocket>(0);
    const auto port = server->getLocalPort();

    std::mutex gateMutex;
    std::condition_variable gate;
    bool sendData = false;

    std::thread serverThread([&] {
        const auto peer = server->accept();
        {
            std::unique_lock<std::mutex> lock(gateMutex);
            gate.wait(lock, [&] { return sendData; });
        }
        const auto payload = ::jxx::NEW<::jxx::lang::ByteArrayType>(
            std::vector<::jxx::lang::jbyte>{0x11, 0x22, 0x33});
        peer->getOutputStream()->write(payload);
        peer->getOutputStream()->flush();
        peer->close();
    });

    const auto channel =
        ::jxx::nio::channels::SocketChannel::open(loopback(port));
    channel->configureBlocking(false);
    const auto buffer = ::jxx::nio::ByteBuffer::allocate(16);

    EXPECT_EQ(0, channel->read(buffer));

    {
        std::lock_guard<std::mutex> lock(gateMutex);
        sendData = true;
    }
    gate.notify_all();

    ::jxx::lang::jint count = 0;
    const auto dataDeadline = Clock::now() + std::chrono::seconds(3);
    while (count == 0 && Clock::now() < dataDeadline) {
        count = channel->read(buffer);
        if (count == 0) std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    ASSERT_EQ(3, count);
    EXPECT_EQ(3, buffer->position());
    EXPECT_EQ(0x11, static_cast<unsigned char>(buffer->get(static_cast<::jxx::lang::jint>(0))));
    EXPECT_EQ(0x22, static_cast<unsigned char>(buffer->get(static_cast<::jxx::lang::jint>(1))));
    EXPECT_EQ(0x33, static_cast<unsigned char>(buffer->get(static_cast<::jxx::lang::jint>(2))));

    ::jxx::lang::jint eof = 0;
    const auto eofDeadline = Clock::now() + std::chrono::seconds(3);
    while (eof == 0 && Clock::now() < eofDeadline) {
        eof = channel->read(buffer);
        if (eof == 0) std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    EXPECT_EQ(-1, eof);

    channel->close();
    server->close();
    serverThread.join();
}

TEST(SocketChannelParity, WriteAdvancesSourcePositionByTransferredBytes) {
    const auto server = ::jxx::NEW<::jxx::net::ServerSocket>(0);
    const auto port = server->getLocalPort();
    std::atomic<int> received{0};

    std::thread serverThread([&] {
        const auto peer = server->accept();
        const auto bytes = ::jxx::NEW<::jxx::lang::ByteArrayType>(4);
        received.store(peer->getInputStream()->read(bytes));
        peer->close();
    });

    const auto channel =
        ::jxx::nio::channels::SocketChannel::open(loopback(port));
    const auto source = ::jxx::nio::ByteBuffer::allocate(4);
    source->put(static_cast<::jxx::lang::jbyte>(1));
    source->put(static_cast<::jxx::lang::jbyte>(2));
    source->put(static_cast<::jxx::lang::jbyte>(3));
    source->put(static_cast<::jxx::lang::jbyte>(4));
    source->flip();

    const auto count = channel->write(source);
    EXPECT_GT(count, 0);
    EXPECT_EQ(count, source->position());

    channel->close();
    serverThread.join();
    EXPECT_GT(received.load(), 0);
    server->close();
}

TEST(SocketChannelParity, ClosingChannelUnblocksBlockingRead) {
    const auto server = ::jxx::NEW<::jxx::net::ServerSocket>(0);
    const auto port = server->getLocalPort();

    std::mutex acceptedMutex;
    std::condition_variable acceptedCondition;
    bool accepted = false;
    bool releasePeer = false;

    std::thread serverThread([&] {
        const auto peer = server->accept();
        {
            std::lock_guard<std::mutex> lock(acceptedMutex);
            accepted = true;
        }
        acceptedCondition.notify_all();
        {
            std::unique_lock<std::mutex> lock(acceptedMutex);
            acceptedCondition.wait(lock, [&] { return releasePeer; });
        }
        peer->close();
    });

    const auto channel =
        ::jxx::nio::channels::SocketChannel::open(loopback(port));

    {
        std::unique_lock<std::mutex> lock(acceptedMutex);
        ASSERT_TRUE(acceptedCondition.wait_for(
            lock, std::chrono::seconds(3), [&] { return accepted; }));
    }

    std::atomic<bool> enteredRead{false};
    std::atomic<bool> asynchronousClose{false};
    std::thread reader([&] {
        const auto buffer = ::jxx::nio::ByteBuffer::allocate(8);
        enteredRead.store(true);
        try {
            (void)channel->read(buffer);
        }
        catch (const ::jxx::nio::channels::AsynchronousCloseException&) {
            asynchronousClose.store(true);
        }
    });

    const auto readDeadline = Clock::now() + std::chrono::seconds(3);
    while (!enteredRead.load() && Clock::now() < readDeadline) {
        std::this_thread::yield();
    }
    ASSERT_TRUE(enteredRead.load());
    std::this_thread::sleep_for(std::chrono::milliseconds(25));

    channel->close();
    reader.join();
    EXPECT_TRUE(asynchronousClose.load());

    {
        std::lock_guard<std::mutex> lock(acceptedMutex);
        releasePeer = true;
    }
    acceptedCondition.notify_all();
    serverThread.join();
    server->close();
}

TEST(SocketChannelParity, ClosedChannelRejectsReadAndWrite) {
    const auto channel = ::jxx::nio::channels::SocketChannel::open();
    channel->close();

    EXPECT_THROW(
        channel->read(::jxx::nio::ByteBuffer::allocate(1)),
        ::jxx::nio::channels::ClosedChannelException);
    EXPECT_THROW(
        channel->write(::jxx::nio::ByteBuffer::allocate(1)),
        ::jxx::nio::channels::ClosedChannelException);
}

} // namespace
