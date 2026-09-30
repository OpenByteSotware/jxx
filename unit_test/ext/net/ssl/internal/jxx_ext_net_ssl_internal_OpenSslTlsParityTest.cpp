#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

#include <gtest/gtest.h>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.HandshakeNotificationTask.h"
#include "ext/net/ssl/jxx.ext.net.ssl.HandshakeCompletedEvent.h"
#include "ext/net/ssl/jxx.ext.net.ssl.HandshakeCompletedListener.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocket.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocketFactory.h"
#include "io/jxx.io.InputStream.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Thread.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.ServerSocket.h"
#include "net/jxx.net.Socket.h"
#include "net/jxx.net.SocketTimeoutException.h"

namespace {

class RecordingHandshakeListener final
    : public ::jxx::lang::ClassBase<
          RecordingHandshakeListener,
          ::jxx::lang::Object,
          ::jxx::ext::net::ssl::HandshakeCompletedListener> {
public:
    void handshakeCompleted(
        const ::jxx::Ptr<
            ::jxx::ext::net::ssl::HandshakeCompletedEvent>&) override {
        const auto current = ::jxx::lang::Thread::currentThread();
        {
            std::lock_guard<std::mutex> lock(mutex_);
            called_ = true;
            managedThread_ = current != nullptr;
            daemonThread_ = current != nullptr && current->isDaemon();
        }
        condition_.notify_all();
    }

    bool wait(std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        return condition_.wait_for(lock, timeout, [this] { return called_; });
    }

    bool managedThread() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return managedThread_;
    }

    bool daemonThread() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return daemonThread_;
    }

private:
    mutable std::mutex mutex_;
    std::condition_variable condition_;
    bool called_ = false;
    bool managedThread_ = false;
    bool daemonThread_ = false;
};

TEST(OpenSslTlsParity, HandshakeNotificationRunsOnManagedDaemonThread) {
    const auto listener = ::jxx::NEW<RecordingHandshakeListener>();
    std::vector<::jxx::Ptr<
        ::jxx::ext::net::ssl::HandshakeCompletedListener>> listeners{
            ::jxx::CAST<
                ::jxx::ext::net::ssl::HandshakeCompletedListener>(listener)};

    const auto task = ::jxx::NEW<
        ::jxx::ext::net::ssl::internal::HandshakeNotificationTask>(
            listeners,
            nullptr);
    const auto thread = ::jxx::NEW<::jxx::lang::Thread>(
        ::jxx::CAST<::jxx::lang::Runnable>(task),
        ::jxx::NEW<::jxx::lang::String>("tls-listener-test"));
    thread->setDaemon(true);
    thread->start();

    ASSERT_TRUE(listener->wait(std::chrono::seconds(3)));
    EXPECT_TRUE(listener->managedThread());
    EXPECT_TRUE(listener->daemonThread());
    thread->join();
}

TEST(OpenSslTlsParity, LayeredSocketFactoryCreatesClientModeSocket) {
    const auto server = ::jxx::NEW<::jxx::net::ServerSocket>(0);
    const auto port = server->getLocalPort();

    std::thread acceptThread([&] {
        const auto peer = server->accept();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        peer->close();
    });

    const auto plain = ::jxx::NEW<::jxx::net::Socket>();
    plain->connect(::jxx::NEW<::jxx::net::InetSocketAddress>(
        ::jxx::NEW<::jxx::lang::String>("127.0.0.1"), port));

    const auto context = ::jxx::ext::net::ssl::SSLContext::getDefault();
    const auto factory = context->getSocketFactory();
    const ::jxx::Ptr<::jxx::io::InputStream> consumed;
    const auto layeredBase = factory->createSocket(plain, consumed, true);
    const auto layered = ::jxx::CAST<::jxx::ext::net::ssl::SSLSocket>(
        layeredBase);

    ASSERT_NE(nullptr, layered);
    EXPECT_TRUE(layered->getUseClientMode());

    layered->close();
    acceptThread.join();
    server->close();
}

TEST(OpenSslTlsParity, HandshakeHonorsConfiguredSocketTimeout) {
    const auto server = ::jxx::NEW<::jxx::net::ServerSocket>(0);
    const auto port = server->getLocalPort();

    std::thread acceptThread([&] {
        const auto peer = server->accept();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        peer->close();
    });

    const auto factory =
        ::jxx::ext::net::ssl::SSLContext::getDefault()->getSocketFactory();
    const auto sslBase = factory->createSocket();
    const auto ssl = ::jxx::CAST<::jxx::ext::net::ssl::SSLSocket>(sslBase);
    ASSERT_NE(nullptr, ssl);

    ssl->setSoTimeout(100);
    ssl->connect(::jxx::NEW<::jxx::net::InetSocketAddress>(
        ::jxx::NEW<::jxx::lang::String>("127.0.0.1"), port), 1000);

    EXPECT_THROW(
        ssl->startHandshake(),
        ::jxx::net::SocketTimeoutException);
    EXPECT_TRUE(ssl->isClosed());

    acceptThread.join();
    server->close();
}

} // namespace
