#include "nio/channels/jxx.nio.channels.SocketChannel.h"

#include <cerrno>
#include <cstring>
#include <string>
#include <vector>

#if !defined(_WIN32)
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#endif

#include "io/jxx.io.IOException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "net/jxx.net.InetSocketAddress.h"

namespace jxx::nio::channels {
namespace {

int lastSocketError() {
#if defined(_WIN32)
    return WSAGetLastError();
#else
    return errno;
#endif
}

bool wouldBlock(int error) {
#if defined(_WIN32)
    return error == WSAEWOULDBLOCK || error == WSAEINPROGRESS ||
        error == WSAEALREADY;
#else
    return error == EWOULDBLOCK || error == EAGAIN ||
        error == EINPROGRESS || error == EALREADY;
#endif
}

void setBlocking(::jxx::net::internal::NativeSocket socket, bool blocking) {
#if defined(_WIN32)
    u_long mode = blocking ? 0UL : 1UL;
    if (ioctlsocket(socket, FIONBIO, &mode) != 0)
        throw ::jxx::io::IOException("could not configure socket blocking mode");
#else
    const int flags = fcntl(socket, F_GETFL, 0);
    if (flags < 0 || fcntl(socket, F_SETFL,
        blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK)) < 0)
        throw ::jxx::io::IOException("could not configure socket blocking mode");
#endif
}

addrinfo* resolve(const ::jxx::Ptr<::jxx::net::InetSocketAddress>& remote) {
    if (remote == nullptr || remote->isUnresolved())
        throw ::jxx::lang::IllegalArgumentException("unresolved remote address");
    const auto host = remote->getHostString();
    if (host == nullptr || host->utf8().empty())
        throw ::jxx::lang::IllegalArgumentException("remote host is empty");
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    addrinfo* result = nullptr;
    const auto service = std::to_string(remote->getPort());
    if (getaddrinfo(host->utf8().c_str(), service.c_str(), &hints, &result) != 0 ||
        result == nullptr)
        throw ::jxx::io::IOException("could not resolve remote address");
    return result;
}

} // namespace

SocketChannel::SocketChannel() {
    ::jxx::net::internal::ensureNetworkInitialized();
}

::jxx::Ptr<SocketChannel> SocketChannel::open() {
    return ::jxx::Ptr<SocketChannel>(new SocketChannel());
}

::jxx::Ptr<SocketChannel> SocketChannel::open(
    const ::jxx::Ptr<::jxx::net::SocketAddress>& remote) {
    const auto channel = open();
    channel->connect(remote);
    return channel;
}

void SocketChannel::ensureSocket() {
    if (!open_) throw ::jxx::io::IOException("channel is closed");
    if (socket_ == ::jxx::net::internal::kInvalidSocket) {
        socket_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (socket_ == ::jxx::net::internal::kInvalidSocket)
            throw ::jxx::io::IOException("could not create socket channel");
        setBlocking(socket_, blocking_);
    }
}

::jxx::Ptr<SocketChannel> SocketChannel::configureBlocking(
    ::jxx::lang::jbool block) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!open_) throw ::jxx::io::IOException("channel is closed");
    if (pending_)
        throw ::jxx::lang::IllegalStateException(
            "cannot change blocking mode while connection is pending");
    if (socket_ != ::jxx::net::internal::kInvalidSocket)
        setBlocking(socket_, block);
    blocking_ = block;
    return ::jxx::CAST<SocketChannel>(thisPtr());
}

::jxx::lang::jbool SocketChannel::isBlocking() const noexcept {
    return blocking_;
}

::jxx::lang::jbool SocketChannel::connect(
    const ::jxx::Ptr<::jxx::net::SocketAddress>& remoteAddress) {
    if (remoteAddress == nullptr) throw ::jxx::lang::NullPointerException();
    std::lock_guard<std::mutex> lock(mutex_);
    if (connected_ || pending_)
        throw ::jxx::lang::IllegalStateException("connection already established or pending");
    const auto remote = ::jxx::CAST<::jxx::net::InetSocketAddress>(remoteAddress);
    addrinfo* addresses = resolve(remote);
    const std::shared_ptr<void> addressGuard(addresses, [](void* value) {
        freeaddrinfo(static_cast<addrinfo*>(value));
    });

    for (addrinfo* current = addresses; current != nullptr; current = current->ai_next) {
        if (socket_ != ::jxx::net::internal::kInvalidSocket)
            ::jxx::net::internal::closeNativeSocket(socket_);
        socket_ = ::socket(current->ai_family, current->ai_socktype, current->ai_protocol);
        if (socket_ == ::jxx::net::internal::kInvalidSocket) continue;
        setBlocking(socket_, blocking_);
        const int result = ::connect(socket_, current->ai_addr,
            static_cast<socklen_t>(current->ai_addrlen));
        if (result == 0) { connected_ = true; pending_ = false; return true; }
        const int error = lastSocketError();
        if (!blocking_ && wouldBlock(error)) { pending_ = true; return false; }
    }
    ::jxx::net::internal::closeNativeSocket(socket_);
    socket_ = ::jxx::net::internal::kInvalidSocket;
    throw ::jxx::io::IOException("socket channel connect failed");
}

::jxx::lang::jbool SocketChannel::finishConnect() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (connected_) return true;
    if (!pending_) throw ::jxx::lang::IllegalStateException("no connection is pending");
    int error = 0;
#if defined(_WIN32)
    int length = sizeof(error);
    if (getsockopt(socket_, SOL_SOCKET, SO_ERROR,
        reinterpret_cast<char*>(&error), &length) != 0)
        throw ::jxx::io::IOException("finishConnect failed");
#else
    socklen_t length = sizeof(error);
    if (getsockopt(socket_, SOL_SOCKET, SO_ERROR, &error, &length) != 0)
        throw ::jxx::io::IOException("finishConnect failed");
#endif
    if (error == 0) { pending_ = false; connected_ = true; return true; }
    if (wouldBlock(error)) return false;
    pending_ = false;
    throw ::jxx::io::IOException("socket channel connection failed");
}

::jxx::lang::jbool SocketChannel::isConnectionPending() const noexcept { return pending_; }
::jxx::lang::jbool SocketChannel::isConnected() const noexcept { return connected_; }
::jxx::lang::jbool SocketChannel::isOpen() const { return open_; }

::jxx::lang::jint SocketChannel::read(
    const ::jxx::Ptr<::jxx::nio::ByteBuffer> destination) {
    if (destination == nullptr) throw ::jxx::lang::NullPointerException();
    std::lock_guard<std::mutex> lock(mutex_);
    if (!connected_) throw ::jxx::io::IOException("channel is not connected");
    const auto remaining = destination->remaining();
    if (remaining == 0) return 0;
    std::vector<unsigned char> bytes(static_cast<std::size_t>(remaining));
#if defined(_WIN32)
    const int count = ::recv(socket_, reinterpret_cast<char*>(bytes.data()), remaining, 0);
#else
    const int count = static_cast<int>(::recv(socket_, bytes.data(), remaining, 0));
#endif
    if (count > 0) {
        for (int index = 0; index < count; ++index)
            destination->put(static_cast<::jxx::lang::jbyte>(bytes[index]));
        return count;
    }
    if (count == 0) { connected_ = false; return -1; }
    if (!blocking_ && wouldBlock(lastSocketError())) return 0;
    throw ::jxx::io::IOException("socket channel read failed");
}

::jxx::lang::jint SocketChannel::write(
    const ::jxx::Ptr<::jxx::nio::ByteBuffer> source) {
    if (source == nullptr) throw ::jxx::lang::NullPointerException();
    std::lock_guard<std::mutex> lock(mutex_);
    if (!connected_) throw ::jxx::io::IOException("channel is not connected");
    const auto remaining = source->remaining();
    if (remaining == 0) return 0;
    const auto position = source->position();
    std::vector<unsigned char> bytes(static_cast<std::size_t>(remaining));
    for (::jxx::lang::jint index = 0; index < remaining; ++index)
        bytes[static_cast<std::size_t>(index)] =
            static_cast<unsigned char>(source->get(position + index));
#if defined(_WIN32)
    const int count = ::send(socket_, reinterpret_cast<const char*>(bytes.data()), remaining, 0);
#else
    const int count = static_cast<int>(::send(socket_, bytes.data(), remaining, 0));
#endif
    if (count > 0) { source->position(position + count); return count; }
    if (!blocking_ && wouldBlock(lastSocketError())) return 0;
    throw ::jxx::io::IOException("socket channel write failed");
}

void SocketChannel::close() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!open_) return;
    if (socket_ != ::jxx::net::internal::kInvalidSocket)
        ::jxx::net::internal::closeNativeSocket(socket_);
    socket_ = ::jxx::net::internal::kInvalidSocket;
    open_ = false;
    connected_ = false;
    pending_ = false;
}

} // namespace jxx::nio::channels
