#include "net/jxx.net.Socket.h"

#if defined(_WIN32)
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <netinet/tcp.h>
    #include <sys/socket.h>
    #include <unistd.h>
#endif

#include <cstring>
#include <stdexcept>

#include "net/internal/jxx.net.internal.NativeSocketState.h"
#include "net/internal/jxx.net.internal.NetPlatform.h"
#include "net/jxx.net.Inet4Address.h"
#include "net/jxx.net.Inet6Address.h"
#include "net/jxx.net.InetAddress.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.SocketImpl.h"
#include "net/jxx.net.SocketImplFactory.h"
#include "net/jxx.net.SocketException.h"
#include "net/jxx.net.UnknownHostException.h"

namespace
{
    jxx::Ptr<jxx::net::SocketImplFactory> g_factory;

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
            return std::make_shared<jxx::net::Inet6Address>(nullptr, std::make_shared<jxx::lang::String>(std::string(buf)), bytes, static_cast<jxx::lang::jint>(sa->sin6_scope_id), nullptr);
        }
        auto* sa = reinterpret_cast<const sockaddr_in*>(&ss);
        std::vector<jxx::lang::jbyte> bytes(4);
        std::memcpy(bytes.data(), &sa->sin_addr, 4);
        char buf[INET_ADDRSTRLEN] = {0};
        ::inet_ntop(AF_INET, &sa->sin_addr, buf, sizeof(buf));
        return std::make_shared<jxx::net::Inet4Address>(nullptr, std::make_shared<jxx::lang::String>(std::string(buf)), bytes);
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
    Socket::Socket()
        : state_(std::make_shared<internal::NativeSocketState>())
    {
    }

    Socket::Socket(jxx::Ptr<Proxy> proxy)
        : Socket()
    {
        proxy_ = std::move(proxy);
    }

    Socket::Socket(jxx::Ptr<jxx::lang::String> host,
                   jxx::lang::jint port)
        : Socket(InetAddress::getByName(std::move(host)), port)
    {
    }

    Socket::Socket(jxx::Ptr<InetAddress> address,
                   jxx::lang::jint port)
        : Socket()
    {
        connect(std::make_shared<InetSocketAddress>(std::move(address), port));
    }

    Socket::Socket(jxx::Ptr<jxx::lang::String> host,
                   jxx::lang::jint port,
                   jxx::Ptr<InetAddress> localAddr,
                   jxx::lang::jint localPort)
        : Socket(InetAddress::getByName(std::move(host)), port, std::move(localAddr), localPort)
    {
    }

    Socket::Socket(jxx::Ptr<InetAddress> address,
                   jxx::lang::jint port,
                   jxx::Ptr<InetAddress> localAddr,
                   jxx::lang::jint localPort)
        : Socket()
    {
        bind(std::make_shared<InetSocketAddress>(std::move(localAddr), localPort));
        connect(std::make_shared<InetSocketAddress>(std::move(address), port));
    }

    Socket::Socket(internal::NativeSocket handle,
                   jxx::Ptr<InetAddress> remoteAddr,
                   jxx::lang::jint remotePort,
                   jxx::Ptr<InetAddress> localAddr,
                   jxx::lang::jint localPort)
        : state_(std::make_shared<internal::NativeSocketState>()),
          remoteAddr_(std::move(remoteAddr)),
          remotePort_(remotePort),
          localAddr_(std::move(localAddr)),
          localPort_(localPort),
          connected_(true),
          bound_(true)
    {
        state_->socket = handle;
    }

    Socket::~Socket()
    {
        try { close(); } catch (...) {}
    }

    void Socket::setSocketImplFactory(jxx::Ptr<SocketImplFactory> fac)
    {
        g_factory = std::move(fac);
    }

    void Socket::ensureCreated_(jxx::lang::jbool /*stream*/)
    {
        if (!state_)
            state_ = std::make_shared<internal::NativeSocketState>();
        if (state_->socket != internal::kInvalidSocket)
            return;
        internal::ensureNetworkInitialized();

        int family = AF_INET;
        if (remoteAddr_ && isIpv6_(remoteAddr_))
            family = AF_INET6;
        else if (localAddr_ && isIpv6_(localAddr_))
            family = AF_INET6;

        state_->socket = ::socket(family, SOCK_STREAM, IPPROTO_TCP);
        if (state_->socket == internal::kInvalidSocket)
            throwSE_("socket creation failed");
    }

    void Socket::connect(jxx::Ptr<SocketAddress> endpoint)
    {
        connect(std::move(endpoint), 0);
    }

    void Socket::connect(jxx::Ptr<SocketAddress> endpoint,
                         jxx::lang::jint /*timeout*/)
    {
        auto isa = std::dynamic_pointer_cast<InetSocketAddress>(endpoint);
        if (!isa)
            throw std::invalid_argument("unsupported socket address");
        auto addr = isa->getAddress();
        if (!addr && isa->isUnresolved())
            addr = InetAddress::getByName(isa->getHostString());
        if (!addr)
            throw UnknownHostException("unable to resolve host");

        remoteAddr_ = addr;
        remotePort_ = isa->getPort();
        ensureCreated_(true);

        socklen_t len = 0;
        auto ss = toSockaddr_(addr, isa->getPort(), len);
        if (::connect(state_->socket, reinterpret_cast<sockaddr*>(&ss), len) != 0)
            throwSE_("connect failed");

        sockaddr_storage local{};
        socklen_t llen = sizeof(local);
        if (::getsockname(state_->socket, reinterpret_cast<sockaddr*>(&local), &llen) == 0)
        {
            localAddr_ = fromSockaddr_(local);
            localPort_ = portFromSockaddr_(local);
        }

        connected_ = true;
        bound_ = true;
    }

    void Socket::bind(jxx::Ptr<SocketAddress> bindpoint)
    {
        auto isa = std::dynamic_pointer_cast<InetSocketAddress>(bindpoint);
        if (!isa)
            throw std::invalid_argument("unsupported socket address");
        localAddr_ = isa->getAddress();
        localPort_ = isa->getPort();
        if (!localAddr_)
            localAddr_ = InetAddress::getByAddress(std::make_shared<jxx::JxxArray<jxx::lang::jbyte, 1U>>(4));
        ensureCreated_(true);

        socklen_t len = 0;
        auto ss = toSockaddr_(localAddr_, localPort_, len);
        if (::bind(state_->socket, reinterpret_cast<sockaddr*>(&ss), len) != 0)
            throwSE_("bind failed");
        bound_ = true;
    }

    jxx::Ptr<InetAddress> Socket::getInetAddress() const { return remoteAddr_; }
    jxx::Ptr<InetAddress> Socket::getLocalAddress() const { return localAddr_; }
    jxx::lang::jint Socket::getPort() const noexcept { return remotePort_; }
    jxx::lang::jint Socket::getLocalPort() const noexcept { return localPort_; }

    jxx::Ptr<SocketAddress> Socket::getRemoteSocketAddress() const
    {
        return connected_ ? std::make_shared<InetSocketAddress>(remoteAddr_, remotePort_) : nullptr;
    }

    jxx::Ptr<SocketAddress> Socket::getLocalSocketAddress() const
    {
        return bound_ ? std::make_shared<InetSocketAddress>(localAddr_, localPort_) : nullptr;
    }

    jxx::Ptr<jxx::nio::channels::SocketChannel> Socket::getChannel() const { return nullptr; }

    jxx::Ptr<jxx::io::InputStream> Socket::getInputStream()
    {
        ensureCreated_(true);
        return std::make_shared<internal::NativeSocketInputStream>(state_);
    }

    jxx::Ptr<jxx::io::OutputStream> Socket::getOutputStream()
    {
        ensureCreated_(true);
        return std::make_shared<internal::NativeSocketOutputStream>(state_);
    }

    void Socket::setSockOptBool_(jxx::lang::jint level,
                                 jxx::lang::jint name,
                                 jxx::lang::jbool value) const
    {
        int v = value ? 1 : 0;
        if (::setsockopt(state_->socket, level, name, reinterpret_cast<const char*>(&v), sizeof(v)) != 0)
            throwSE_("setsockopt(bool) failed");
    }

    jxx::lang::jbool Socket::getSockOptBool_(jxx::lang::jint level,
                                             jxx::lang::jint name) const
    {
        int v = 0;
        socklen_t len = sizeof(v);
        if (::getsockopt(state_->socket, level, name, reinterpret_cast<char*>(&v), &len) != 0)
            throwSE_("getsockopt(bool) failed");
        return v != 0;
    }

    void Socket::setSockOptInt_(jxx::lang::jint level,
                                jxx::lang::jint name,
                                jxx::lang::jint value) const
    {
        int v = static_cast<int>(value);
        if (::setsockopt(state_->socket, level, name, reinterpret_cast<const char*>(&v), sizeof(v)) != 0)
            throwSE_("setsockopt(int) failed");
    }

    jxx::lang::jint Socket::getSockOptInt_(jxx::lang::jint level,
                                           jxx::lang::jint name) const
    {
        int v = 0;
        socklen_t len = sizeof(v);
        if (::getsockopt(state_->socket, level, name, reinterpret_cast<char*>(&v), &len) != 0)
            throwSE_("getsockopt(int) failed");
        return static_cast<jxx::lang::jint>(v);
    }

    void Socket::setTcpNoDelay(jxx::lang::jbool on) { ensureCreated_(true); setSockOptBool_(IPPROTO_TCP, TCP_NODELAY, on); }
    jxx::lang::jbool Socket::getTcpNoDelay() const { return state_ && state_->socket != internal::kInvalidSocket ? getSockOptBool_(IPPROTO_TCP, TCP_NODELAY) : false; }

    void Socket::setSoLinger(jxx::lang::jbool on, jxx::lang::jint linger)
    {
        ensureCreated_(true);
        linger l{};
        l.l_onoff = on ? 1 : 0;
        l.l_linger = static_cast<u_short>(linger);
        if (::setsockopt(state_->socket, SOL_SOCKET, SO_LINGER, reinterpret_cast<const char*>(&l), sizeof(l)) != 0)
            throwSE_("setsockopt(linger) failed");
    }

    jxx::lang::jint Socket::getSoLinger() const
    {
        if (!state_ || state_->socket == internal::kInvalidSocket)
            return -1;
        linger l{};
        socklen_t len = sizeof(l);
        if (::getsockopt(state_->socket, SOL_SOCKET, SO_LINGER, reinterpret_cast<char*>(&l), &len) != 0)
            throwSE_("getsockopt(linger) failed");
        return l.l_onoff ? static_cast<jxx::lang::jint>(l.l_linger) : -1;
    }

    void Socket::sendUrgentData(jxx::lang::jint data)
    {
        ensureCreated_(true);
        unsigned char c = static_cast<unsigned char>(data & 0xFF);
        if (::send(state_->socket, reinterpret_cast<const char*>(&c), 1, MSG_OOB) != 1)
            throwSE_("send urgent data failed");
    }

    void Socket::setOOBInline(jxx::lang::jbool on) { ensureCreated_(true); setSockOptBool_(SOL_SOCKET, SO_OOBINLINE, on); }
    jxx::lang::jbool Socket::getOOBInline() const { return state_ && state_->socket != internal::kInvalidSocket ? getSockOptBool_(SOL_SOCKET, SO_OOBINLINE) : false; }

    void Socket::setSoTimeout(jxx::lang::jint timeout)
    {
        ensureCreated_(true);
        soTimeout_ = timeout;
    #if defined(_WIN32)
        DWORD tv = static_cast<DWORD>(timeout);
        ::setsockopt(state_->socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&tv), sizeof(tv));
        ::setsockopt(state_->socket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&tv), sizeof(tv));
    #else
        timeval tv{};
        tv.tv_sec = timeout / 1000;
        tv.tv_usec = (timeout % 1000) * 1000;
        ::setsockopt(state_->socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&tv), sizeof(tv));
        ::setsockopt(state_->socket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&tv), sizeof(tv));
    #endif
    }

    jxx::lang::jint Socket::getSoTimeout() const noexcept { return soTimeout_; }
    void Socket::setSendBufferSize(jxx::lang::jint size) { ensureCreated_(true); setSockOptInt_(SOL_SOCKET, SO_SNDBUF, size); }
    jxx::lang::jint Socket::getSendBufferSize() const { return state_ && state_->socket != internal::kInvalidSocket ? getSockOptInt_(SOL_SOCKET, SO_SNDBUF) : 0; }
    void Socket::setReceiveBufferSize(jxx::lang::jint size) { ensureCreated_(true); setSockOptInt_(SOL_SOCKET, SO_RCVBUF, size); }
    jxx::lang::jint Socket::getReceiveBufferSize() const { return state_ && state_->socket != internal::kInvalidSocket ? getSockOptInt_(SOL_SOCKET, SO_RCVBUF) : 0; }
    void Socket::setKeepAlive(jxx::lang::jbool on) { ensureCreated_(true); setSockOptBool_(SOL_SOCKET, SO_KEEPALIVE, on); }
    jxx::lang::jbool Socket::getKeepAlive() const { return state_ && state_->socket != internal::kInvalidSocket ? getSockOptBool_(SOL_SOCKET, SO_KEEPALIVE) : false; }
    void Socket::setTrafficClass(jxx::lang::jint tc) { ensureCreated_(true); setSockOptInt_(IPPROTO_IP, IP_TOS, tc); }
    jxx::lang::jint Socket::getTrafficClass() const { return state_ && state_->socket != internal::kInvalidSocket ? getSockOptInt_(IPPROTO_IP, IP_TOS) : 0; }
    void Socket::setReuseAddress(jxx::lang::jbool on) { ensureCreated_(true); setSockOptBool_(SOL_SOCKET, SO_REUSEADDR, on); }
    jxx::lang::jbool Socket::getReuseAddress() const { return state_ && state_->socket != internal::kInvalidSocket ? getSockOptBool_(SOL_SOCKET, SO_REUSEADDR) : false; }

    void Socket::shutdownInput()
    {
        if (!state_ || state_->socket == internal::kInvalidSocket)
            return;
    #if defined(_WIN32)
        ::shutdown(state_->socket, SD_RECEIVE);
    #else
        ::shutdown(state_->socket, SHUT_RD);
    #endif
        state_->inputShutdown = true;
    }

    void Socket::shutdownOutput()
    {
        if (!state_ || state_->socket == internal::kInvalidSocket)
            return;
    #if defined(_WIN32)
        ::shutdown(state_->socket, SD_SEND);
    #else
        ::shutdown(state_->socket, SHUT_WR);
    #endif
        state_->outputShutdown = true;
    }

    void Socket::close()
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

    jxx::lang::jbool Socket::isConnected() const noexcept { return connected_; }
    jxx::lang::jbool Socket::isBound() const noexcept { return bound_; }
    jxx::lang::jbool Socket::isClosed() const noexcept { return !state_ || state_->closed; }
    jxx::lang::jbool Socket::isInputShutdown() const noexcept { return state_ && state_->inputShutdown; }
    jxx::lang::jbool Socket::isOutputShutdown() const noexcept { return state_ && state_->outputShutdown; }
    void Socket::setPerformancePreferences(jxx::lang::jint, jxx::lang::jint, jxx::lang::jint) {}

    jxx::Ptr<jxx::lang::String> Socket::toString() const
    {
        if (!connected_)
            return std::make_shared<jxx::lang::String>("Socket[unconnected]");
        return std::make_shared<jxx::lang::String>(
            std::string("Socket[addr=") + (remoteAddr_ ? remoteAddr_->getHostAddress()->utf8() : std::string()) +
            ",port=" + std::to_string(remotePort_) +
            ",localport=" + std::to_string(localPort_) + "]");
    }
}
