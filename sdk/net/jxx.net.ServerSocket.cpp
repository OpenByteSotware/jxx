#include "net/jxx.net.ServerSocket.h"

#if defined(_WIN32)
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
#endif

#include <cstring>
#include <stdexcept>

#include "net/internal/jxx.net.internal.NativeSocketState.h"
#include "net/internal/jxx.net.internal.NetPlatform.h"
#include "net/jxx.net.BindException.h"
#include "net/jxx.net.Inet4Address.h"
#include "net/jxx.net.Inet6Address.h"
#include "net/jxx.net.InetAddress.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.Socket.h"
#include "net/jxx.net.SocketException.h"
#include "net/jxx.net.SocketImplFactory.h"

namespace
{
    jxx::Ptr<jxx::net::SocketImplFactory> g_serverFactory;

    [[noreturn]] void throwSE_(const char* msg)
    {
        throw jxx::net::SocketException(msg);
    }

    inline bool isIpv6_(jxx::Ptr<jxx::net::InetAddress> addr)
    {
        return std::dynamic_pointer_cast<jxx::net::Inet6Address>(addr) != nullptr;
    }

    inline sockaddr_storage toSockaddr_(jxx::Ptr<jxx::net::InetAddress> addr,
                                        jxx::lang::jint port,
                                        socklen_t& len)
    {
        sockaddr_storage ss{};
        if (std::dynamic_pointer_cast<jxx::net::Inet6Address>(addr))
        {
            auto bytes = addr->getAddress();
            auto* sa = reinterpret_cast<sockaddr_in6*>(&ss);
            sa->sin6_family = AF_INET6;
            sa->sin6_port = htons(static_cast<uint16_t>(port));
            for (int i = 0; i < 16; ++i)
                reinterpret_cast<unsigned char*>(&sa->sin6_addr)[i] = static_cast<unsigned char>((*bytes)[i]);
            len = sizeof(sockaddr_in6);
        }
        else
        {
            auto bytes = addr->getAddress();
            auto* sa = reinterpret_cast<sockaddr_in*>(&ss);
            sa->sin_family = AF_INET;
            sa->sin_port = htons(static_cast<uint16_t>(port));
            for (int i = 0; i < 4; ++i)
                reinterpret_cast<unsigned char*>(&sa->sin_addr)[i] = static_cast<unsigned char>((*bytes)[i]);
            len = sizeof(sockaddr_in);
        }
        return ss;
    }

    inline jxx::Ptr<jxx::net::InetAddress> fromSockaddr_(const sockaddr_storage& ss)
    {
        if (ss.ss_family == AF_INET6)
        {
            auto* sa = reinterpret_cast<const sockaddr_in6*>(&ss);
            std::vector<jxx::lang::jbyte> bytes(16);
            std::memcpy(bytes.data(), &sa->sin6_addr, 16);
            char buf[INET6_ADDRSTRLEN] = {0};
            ::inet_ntop(AF_INET6, &sa->sin6_addr, buf, sizeof(buf));
            return jxx::NEW<jxx::net::Inet6Address>(nullptr, jxx::NEW<jxx::lang::String>(std::string(buf)), bytes, static_cast<jxx::lang::jint>(sa->sin6_scope_id), nullptr);
        }
        auto* sa = reinterpret_cast<const sockaddr_in*>(&ss);
        std::vector<jxx::lang::jbyte> bytes(4);
        std::memcpy(bytes.data(), &sa->sin_addr, 4);
        char buf[INET_ADDRSTRLEN] = {0};
        ::inet_ntop(AF_INET, &sa->sin_addr, buf, sizeof(buf));
        return jxx::NEW<jxx::net::Inet4Address>(nullptr, jxx::NEW<jxx::lang::String>(std::string(buf)), bytes);
    }

    inline jxx::lang::jint portFromSockaddr_(const sockaddr_storage& ss)
    {
        if (ss.ss_family == AF_INET6)
            return ntohs(reinterpret_cast<const sockaddr_in6*>(&ss)->sin6_port);
        return ntohs(reinterpret_cast<const sockaddr_in*>(&ss)->sin_port);
    }
}

namespace jxx::net
{
    ServerSocket::ServerSocket()
        : state_(jxx::NEW<internal::NativeSocketState>())
    {
    }

    ServerSocket::ServerSocket(jxx::lang::jint port)
        : ServerSocket(port, 50, nullptr)
    {
    }

    ServerSocket::ServerSocket(jxx::lang::jint port,
                               jxx::lang::jint backlog)
        : ServerSocket(port, backlog, nullptr)
    {
    }

    ServerSocket::ServerSocket(jxx::lang::jint port,
                               jxx::lang::jint backlog,
                               jxx::Ptr<InetAddress> bindAddr)
        : ServerSocket()
    {
        bind(jxx::NEW<InetSocketAddress>(std::move(bindAddr), port), backlog);
    }

    ServerSocket::~ServerSocket()
    {
        try { close(); } catch (...) {}
    }

    void ServerSocket::setSocketFactory(jxx::Ptr<SocketImplFactory> fac)
    {
        g_serverFactory = std::move(fac);
    }

    void ServerSocket::ensureCreated_()
    {
        if (!state_)
            state_ = jxx::NEW<internal::NativeSocketState>();
        if (state_->socket != internal::kInvalidSocket)
            return;
        internal::ensureNetworkInitialized();

        const int family = (localAddr_ && isIpv6_(localAddr_)) ? AF_INET6 : AF_INET;
        state_->socket = ::socket(family, SOCK_STREAM, IPPROTO_TCP);
        if (state_->socket == internal::kInvalidSocket)
            throwSE_("server socket creation failed");
    }

    void ServerSocket::bind(jxx::Ptr<SocketAddress> endpoint)
    {
        bind(std::move(endpoint), 50);
    }

    void ServerSocket::bind(const jxx::Ptr<SocketAddress> endpoint,
                            jxx::lang::jint backlog)
    {
        auto isa = std::dynamic_pointer_cast<InetSocketAddress>(endpoint);
        if (!isa)
            throw std::invalid_argument("unsupported socket address");

        localAddr_ = isa->getAddress();
        localPort_ = isa->getPort();
        if (!localAddr_)
        {
            auto any = jxx::NEW<jxx::lang::ByteArrayType>(4);
            (*any)[0] = 0; (*any)[1] = 0; (*any)[2] = 0; (*any)[3] = 0;
            localAddr_ = InetAddress::getByAddress(any);
        }

        ensureCreated_();
        setReuseAddress(true);

        socklen_t len = 0;
        auto ss = toSockaddr_(localAddr_, localPort_, len);
        if (::bind(state_->socket, reinterpret_cast<sockaddr*>(&ss), len) != 0)
            throw BindException("bind failed");
        if (::listen(state_->socket, static_cast<int>(backlog)) != 0)
            throwSE_("listen failed");

        sockaddr_storage local{};
        socklen_t llen = sizeof(local);
        if (::getsockname(state_->socket, reinterpret_cast<sockaddr*>(&local), &llen) == 0)
        {
            localAddr_ = fromSockaddr_(local);
            localPort_ = portFromSockaddr_(local);
        }
        bound_ = true;
    }

    jxx::Ptr<InetAddress> ServerSocket::getInetAddress() const { return localAddr_; }
    jxx::lang::jint ServerSocket::getLocalPort() const noexcept { return localPort_; }
    jxx::Ptr<SocketAddress> ServerSocket::getLocalSocketAddress() const
    {
        return bound_ ? jxx::NEW<InetSocketAddress>(localAddr_, localPort_) : nullptr;
    }

    jxx::Ptr<Socket> ServerSocket::accept()
    {
        ensureCreated_();
        sockaddr_storage remote{};
        socklen_t len = sizeof(remote);
        const auto s = ::accept(state_->socket, reinterpret_cast<sockaddr*>(&remote), &len);
        if (s == internal::kInvalidSocket)
            throwSE_("accept failed");

        sockaddr_storage local{};
        socklen_t llen = sizeof(local);
        ::getsockname(s, reinterpret_cast<sockaddr*>(&local), &llen);

        return jxx::NEW<Socket>(
            s,
            fromSockaddr_(remote),
            portFromSockaddr_(remote),
            fromSockaddr_(local),
            portFromSockaddr_(local));
    }

    void ServerSocket::close()
    {
        if (!state_ || state_->closed)
            return;
        std::lock_guard<std::mutex> lock(state_->m);
        if (!state_->closed)
        {
            internal::closeNativeSocket(state_->socket);
            state_->socket = internal::kInvalidSocket;
            state_->closed = true;
        }
    }

    jxx::Ptr<jxx::nio::channels::ServerSocketChannel> ServerSocket::getChannel() const { return nullptr; }
    jxx::lang::jbool ServerSocket::isBound() const noexcept { return bound_; }
    jxx::lang::jbool ServerSocket::isClosed() const noexcept { return !state_ || state_->closed; }

    void ServerSocket::setSockOptBool_(jxx::lang::jint level,
                                       jxx::lang::jint name,
                                       jxx::lang::jbool value) const
    {
        int v = value ? 1 : 0;
        if (::setsockopt(state_->socket, level, name, reinterpret_cast<const char*>(&v), sizeof(v)) != 0)
            throwSE_("setsockopt(bool) failed");
    }

    jxx::lang::jbool ServerSocket::getSockOptBool_(jxx::lang::jint level,
                                                   jxx::lang::jint name) const
    {
        int v = 0;
        socklen_t len = sizeof(v);
        if (::getsockopt(state_->socket, level, name, reinterpret_cast<char*>(&v), &len) != 0)
            throwSE_("getsockopt(bool) failed");
        return v != 0;
    }

    void ServerSocket::setSockOptInt_(jxx::lang::jint level,
                                      jxx::lang::jint name,
                                      jxx::lang::jint value) const
    {
        int v = static_cast<int>(value);
        if (::setsockopt(state_->socket, level, name, reinterpret_cast<const char*>(&v), sizeof(v)) != 0)
            throwSE_("setsockopt(int) failed");
    }

    jxx::lang::jint ServerSocket::getSockOptInt_(jxx::lang::jint level,
                                                 jxx::lang::jint name) const
    {
        int v = 0;
        socklen_t len = sizeof(v);
        if (::getsockopt(state_->socket, level, name, reinterpret_cast<char*>(&v), &len) != 0)
            throwSE_("getsockopt(int) failed");
        return static_cast<jxx::lang::jint>(v);
    }

    void ServerSocket::setSoTimeout(jxx::lang::jint timeout)
    {
        ensureCreated_();
        soTimeout_ = timeout;
    #if defined(_WIN32)
        DWORD tv = static_cast<DWORD>(timeout);
        ::setsockopt(state_->socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&tv), sizeof(tv));
    #else
        timeval tv{};
        tv.tv_sec = timeout / 1000;
        tv.tv_usec = (timeout % 1000) * 1000;
        ::setsockopt(state_->socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&tv), sizeof(tv));
    #endif
    }

    jxx::lang::jint ServerSocket::getSoTimeout() const noexcept { return soTimeout_; }
    void ServerSocket::setReuseAddress(jxx::lang::jbool on) { ensureCreated_(); setSockOptBool_(SOL_SOCKET, SO_REUSEADDR, on); }
    jxx::lang::jbool ServerSocket::getReuseAddress() const { return state_ && state_->socket != internal::kInvalidSocket ? getSockOptBool_(SOL_SOCKET, SO_REUSEADDR) : false; }
    void ServerSocket::setReceiveBufferSize(jxx::lang::jint size) { ensureCreated_(); setSockOptInt_(SOL_SOCKET, SO_RCVBUF, size); }
    jxx::lang::jint ServerSocket::getReceiveBufferSize() const { return state_ && state_->socket != internal::kInvalidSocket ? getSockOptInt_(SOL_SOCKET, SO_RCVBUF) : 0; }
    void ServerSocket::setPerformancePreferences(jxx::lang::jint, jxx::lang::jint, jxx::lang::jint) {}

    jxx::Ptr<jxx::lang::String> ServerSocket::toString() const
    {
        if (!bound_)
            return jxx::NEW<jxx::lang::String>("ServerSocket[unbound]");
        return jxx::NEW<jxx::lang::String>(
            std::string("ServerSocket[addr=") + (localAddr_ ? localAddr_->getHostAddress()->utf8() : std::string()) +
            ",localport=" + std::to_string(localPort_) + "]");
    }
}
