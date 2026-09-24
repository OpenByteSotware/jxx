
#include <cstdint>
#include <cstring>
#include <string>
#include <stdexcept>
#include <vector>
#include <utility>
#include <system_error>


#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <net/if.h>   // if_nametoindex (IPv6 multicast interface lookup)
#endif

#include "net/jxx.net.DatagramSocket.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "net/jxx.net.InetAddress.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.SocketTimeoutException.h"
#include "net/jxx.net.UnknownHostException.h"
#include "net/jxx.net.SocketException.h"


#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif

using socklen_t = int;
using socket_t = SOCKET;
static inline int last_sock_error() { return WSAGetLastError(); }
static inline bool is_sock_wouldblock(int e) { return e == WSAEWOULDBLOCK || e == WSAETIMEDOUT; }
static inline std::string sock_strerror(int e) {
    char* msg = nullptr;
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, e, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&msg, 0, nullptr);
    std::string s = msg ? msg : "socket error";
    if (msg) LocalFree(msg);
    return s;
}
static inline void wsa_ensure_started() {
    static bool started = false;
    if (!started) {
        WSADATA wsa{};
        int rc = WSAStartup(MAKEWORD(2, 2), &wsa);
        if (rc != 0) throw jxx::net::SocketException("WSAStartup failed: " + std::to_string(rc));
        started = true;
    }
}
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
using socket_t = int;
static inline int last_sock_error() { return errno; }
static inline bool is_sock_wouldblock(int e) { return e == EAGAIN || e == EWOULDBLOCK || e == EINPROGRESS; }
static inline std::string sock_strerror(int e) { return std::string(strerror(e)); }
static inline void closesocket(socket_t s) { ::close(s); }
#endif


#include "lang/jxx.lang.IllegalArgumentException.h"
#include "net/jxx.net.SocketException.h"
#include "net/jxx.net.SocketTimeoutException.h"
#include "net/jxx.net.UnknownHostException.h"

namespace jxx::net {
    // ---------------------- Helpers ----------------------
    inline void fill_sockaddr(const std::string& host,
        std::uint16_t port,
        sockaddr_storage& out_addr,
        socklen_t& out_len,
        int family_hint = AF_UNSPEC)
    {
#if defined(_WIN32)
        wsa_ensure_started();
#endif
        addrinfo hints{}; hints.ai_family = family_hint; hints.ai_socktype = SOCK_DGRAM; hints.ai_protocol = IPPROTO_UDP;
        addrinfo* res = nullptr;

        std::string portstr = std::to_string(port);
        int rc = ::getaddrinfo(host.c_str(), portstr.c_str(), &hints, &res);
        if (rc != 0 || !res) {
#if defined(_WIN32)
            throw jxx::net::UnknownHostException("getaddrinfo failed: " + std::to_string(rc));
#else
            throw jxx::net::UnknownHostException(std::string("getaddrinfo failed: ") + gai_strerror(rc));
#endif
        }
        std::memset(&out_addr, 0, sizeof(out_addr));
        std::memcpy(&out_addr, res->ai_addr, res->ai_addrlen);
        out_len = static_cast<socklen_t>(res->ai_addrlen);
        ::freeaddrinfo(res);
    }

    inline std::string sockaddr_to_ip(const sockaddr* sa, socklen_t salen, std::uint16_t& portOut) {
        char host[NI_MAXHOST]{};
        char serv[NI_MAXSERV]{};
        int rc = ::getnameinfo(sa, salen, host, sizeof(host), serv, sizeof(serv),
            NI_NUMERICHOST | NI_NUMERICSERV);
        if (rc != 0) {
#if defined(_WIN32)
            throw jxx::net::SocketException("getnameinfo failed: " + std::to_string(rc));
#else
            throw jxx::net::SocketException(std::string("getnameinfo failed: ") + gai_strerror(rc));
#endif
        }
        portOut = static_cast<std::uint16_t>(std::stoul(serv));
        return std::string(host);
    }


struct NativeDatagramPacket { std::vector<std::uint8_t> buffer; std::size_t offset{0}; std::size_t length{0}; std::string address; std::uint16_t port{0}; };

    class DatagramSocket::Impl {
    public:
        explicit Impl(Family fam) { open_unbound(fam); }
        Impl() { open_unbound(); }

        // Bind immediately to local port (optionally local address like "0.0.0.0" or "::")
        explicit Impl(std::uint16_t localPort, const std::string& localAddress = std::string()) {
            open_unbound();
            if (!localAddress.empty()) {
                bind(localAddress, localPort);
            }
            else {
                bind(localPort);
            }
        }

        // Non-copyable; movable
        Impl(const Impl&) = delete;
        Impl& operator=(const Impl&) = delete;

        Impl(DatagramSocket&& other) noexcept { move_from(std::move(other)); }
        Impl& operator=(Impl&& other) noexcept {
            if (this != &other) {
                close();
                move_from(std::move(other));
            }
            return *this;
        }

        ~Impl() { close(); }
        // -------- Multicast (IPv4) --------
// Join IPv4 multicast group on a specific local interface (IPv4 address).
// Example interface: "0.0.0.0" for default, or "192.168.1.5"
        void joinGroupIPv4(const std::string& groupAddress, const std::string& localInterfaceIPv4 = "0.0.0.0") {
            ensure_open();
            if (family_ != AF_INET) {
                throw jxx::net::SocketException("socket is not IPv4");
            }
            ip_mreq mreq{};
            if (::inet_pton(AF_INET, groupAddress.c_str(), &mreq.imr_multiaddr) != 1 ||
                !IN_MULTICAST(ntohl(mreq.imr_multiaddr.s_addr))) {
                throw jxx::lang::IllegalArgumentException(
                    "joinGroupIPv4: address is not an IPv4 multicast group");
            }
            if (::inet_pton(AF_INET, localInterfaceIPv4.c_str(), &mreq.imr_interface) != 1) {
                throw jxx::lang::IllegalArgumentException(
                    "joinGroupIPv4: invalid interface address");
            }
            if (setsockopt(sock_, IPPROTO_IP, IP_ADD_MEMBERSHIP,
#if defined(_WIN32)
                reinterpret_cast<const char*>(&mreq),
#else
                & mreq,
#endif
                sizeof(mreq)) != 0) {
                throw jxx::net::SocketException("IP_ADD_MEMBERSHIP failed: " + sock_error_string());
            }
        }

        void leaveGroupIPv4(const std::string& groupAddress, const std::string& localInterfaceIPv4 = "0.0.0.0") {
            ensure_open();
            if (family_ != AF_INET) {
                throw jxx::net::SocketException("socket is not IPv4");
            }
            ip_mreq mreq{};
            if (::inet_pton(AF_INET, groupAddress.c_str(), &mreq.imr_multiaddr) != 1 ||
                !IN_MULTICAST(ntohl(mreq.imr_multiaddr.s_addr))) {
                throw jxx::lang::IllegalArgumentException(
                    "leaveGroupIPv4: address is not an IPv4 multicast group");
            }
            if (::inet_pton(AF_INET, localInterfaceIPv4.c_str(), &mreq.imr_interface) != 1) {
                throw jxx::lang::IllegalArgumentException(
                    "leaveGroupIPv4: invalid interface address");
            }
            if (setsockopt(sock_, IPPROTO_IP, IP_DROP_MEMBERSHIP,
#if defined(_WIN32)
                reinterpret_cast<const char*>(&mreq),
#else
                & mreq,
#endif
                sizeof(mreq)) != 0) {
                throw jxx::net::SocketException("IP_DROP_MEMBERSHIP failed: " + sock_error_string());
            }
        }

        // Set IPv4 multicast TTL (hop count). Default in many stacks is 1.
        void setMulticastTTL(int ttl) {
            ensure_open();
            if (ttl < 0 || ttl > 255) {
                throw jxx::lang::IllegalArgumentException("multicast TTL out of range");
            }
            if (family_ != AF_INET) {
                throw jxx::net::SocketException("socket is not IPv4");
            }
            unsigned char t = static_cast<unsigned char>(ttl);
            if (setsockopt(sock_, IPPROTO_IP, IP_MULTICAST_TTL,
#if defined(_WIN32)
                reinterpret_cast<const char*>(&t),
#else
                & t,
#endif
                sizeof(t)) != 0) {
                throw jxx::net::SocketException("IP_MULTICAST_TTL failed: " + sock_error_string());
            }
        }

        // Enable/disable IPv4 multicast loopback
        void setMulticastLoopIPv4(bool enable) {
            ensure_open();
            if (family_ != AF_INET) {
                throw jxx::net::SocketException("socket is not IPv4");
            }
            unsigned char on = enable ? 1 : 0;
            if (setsockopt(sock_, IPPROTO_IP, IP_MULTICAST_LOOP,
#if defined(_WIN32)
                reinterpret_cast<const char*>(&on),
#else
                & on,
#endif
                sizeof(on)) != 0) {
                throw jxx::net::SocketException("IP_MULTICAST_LOOP failed: " + sock_error_string());
            }
        }

        // Select IPv4 outgoing interface by local IPv4 address
        void setMulticastInterfaceIPv4(const std::string& localInterfaceIPv4) {
            ensure_open();
            if (family_ != AF_INET) {
                throw jxx::net::SocketException("socket is not IPv4");
            }
            in_addr ia{};
            if (::inet_pton(AF_INET, localInterfaceIPv4.c_str(), &ia) != 1) {
                throw jxx::lang::IllegalArgumentException(
                    "setMulticastInterfaceIPv4: invalid interface address");
            }
            if (setsockopt(sock_, IPPROTO_IP, IP_MULTICAST_IF,
#if defined(_WIN32)
                reinterpret_cast<const char*>(&ia),
#else
                & ia,
#endif
                sizeof(ia)) != 0) {
                throw jxx::net::SocketException("IP_MULTICAST_IF failed: " + sock_error_string());
            }
        }

        // -------- Multicast (IPv6) --------
        // Join IPv6 multicast group (e.g., "ff12::1234") on interface index (0 = default)
        void joinGroupIPv6(const std::string& groupAddress, unsigned int ifindex = 0) {
            ensure_open();
            if (family_ != AF_INET6) {
                throw jxx::net::SocketException("socket is not IPv6");
            }
            ipv6_mreq mreq{};
            // Convert textual address to in6_addr
#if defined(_WIN32)
            IN6_ADDR addr6{};
            if (InetPtonA(AF_INET6, groupAddress.c_str(), &addr6) != 1 ||
                addr6.u.Byte[0] != 0xff) {
                throw jxx::lang::IllegalArgumentException(
                    "joinGroupIPv6: address is not an IPv6 multicast group");
            }
            std::memcpy(&mreq.ipv6mr_multiaddr, &addr6, sizeof(IN6_ADDR));
#else
            if (::inet_pton(AF_INET6, groupAddress.c_str(), &mreq.ipv6mr_multiaddr) != 1 ||
                mreq.ipv6mr_multiaddr.s6_addr[0] != 0xff) {
                throw jxx::lang::IllegalArgumentException(
                    "joinGroupIPv6: address is not an IPv6 multicast group");
            }
#endif
            mreq.ipv6mr_interface = ifindex;
            if (setsockopt(sock_, IPPROTO_IPV6, IPV6_JOIN_GROUP,
#if defined(_WIN32)
                reinterpret_cast<const char*>(&mreq),
#else
                & mreq,
#endif
                sizeof(mreq)) != 0) {
                throw jxx::net::SocketException("IPV6_JOIN_GROUP failed: " + sock_error_string());
            }
        }

        void leaveGroupIPv6(const std::string& groupAddress, unsigned int ifindex = 0) {
            ensure_open();
            if (family_ != AF_INET6) {
                throw jxx::net::SocketException("socket is not IPv6");
            }
            ipv6_mreq mreq{};
#if defined(_WIN32)
            IN6_ADDR addr6{};
            if (InetPtonA(AF_INET6, groupAddress.c_str(), &addr6) != 1 ||
                addr6.u.Byte[0] != 0xff) {
                throw jxx::lang::IllegalArgumentException(
                    "leaveGroupIPv6: address is not an IPv6 multicast group");
            }
            std::memcpy(&mreq.ipv6mr_multiaddr, &addr6, sizeof(IN6_ADDR));
#else
            if (::inet_pton(AF_INET6, groupAddress.c_str(), &mreq.ipv6mr_multiaddr) != 1 ||
                mreq.ipv6mr_multiaddr.s6_addr[0] != 0xff) {
                throw jxx::lang::IllegalArgumentException(
                    "leaveGroupIPv6: address is not an IPv6 multicast group");
            }
#endif
            mreq.ipv6mr_interface = ifindex;
            if (setsockopt(sock_, IPPROTO_IPV6, IPV6_LEAVE_GROUP,
#if defined(_WIN32)
                reinterpret_cast<const char*>(&mreq),
#else
                & mreq,
#endif
                sizeof(mreq)) != 0) {
                throw jxx::net::SocketException("IPV6_LEAVE_GROUP failed: " + sock_error_string());
            }
        }

        // IPv6 hop limit (TTL analog)
        void setMulticastHopsIPv6(int hops) {
            ensure_open();
            if (hops < 0 || hops > 255) {
                throw jxx::lang::IllegalArgumentException("multicast hop limit out of range");
            }
            if (family_ != AF_INET6) {
                throw jxx::net::SocketException("socket is not IPv6");
            }
            int h = hops;
            if (setsockopt(sock_, IPPROTO_IPV6, IPV6_MULTICAST_HOPS,
#if defined(_WIN32)
                reinterpret_cast<const char*>(&h),
#else
                & h,
#endif
                sizeof(h)) != 0) {
                throw jxx::net::SocketException("IPV6_MULTICAST_HOPS failed: " + sock_error_string());
            }
        }

        // Enable/disable IPv6 multicast loopback
        void setMulticastLoopIPv6(bool enable) {
            ensure_open();
            if (family_ != AF_INET6) {
                throw jxx::net::SocketException("socket is not IPv6");
            }
            unsigned int on = enable ? 1u : 0u;
            if (setsockopt(sock_, IPPROTO_IPV6, IPV6_MULTICAST_LOOP,
#if defined(_WIN32)
                reinterpret_cast<const char*>(&on),
#else
                & on,
#endif
                sizeof(on)) != 0) {
                throw jxx::net::SocketException("IPV6_MULTICAST_LOOP failed: " + sock_error_string());
            }
        }

        // Select IPv6 outgoing interface by index (0 = default)
        void setMulticastInterfaceIPv6(unsigned int ifindex) {
            ensure_open();
            if (family_ != AF_INET6) {
                throw jxx::net::SocketException("socket is not IPv6");
            }
            unsigned int idx = ifindex;
            if (setsockopt(sock_, IPPROTO_IPV6, IPV6_MULTICAST_IF,
#if defined(_WIN32)
                reinterpret_cast<const char*>(&idx),
#else
                & idx,
#endif
                sizeof(idx)) != 0) {
                throw jxx::net::SocketException("IPV6_MULTICAST_IF failed: " + sock_error_string());
            }
        }

        // -------- Core operations --------
        void bind(std::uint16_t localPort) {
            bind("0.0.0.0", localPort); // INADDR_ANY (IPv4). If you want dual-stack, use "::" on IPv6-enabled OS.
        }

        void bind(const std::string& localAddress, std::uint16_t localPort) {
            ensure_open();
            if (bound_) {
                throw jxx::net::SocketException("DatagramSocket is already bound");
            }
            // Allow re-bind options to be set before bind if desired
            sockaddr_storage addr{}; socklen_t len{};
            fill_sockaddr(localAddress, localPort, addr, len);
            if (::bind(sock_, reinterpret_cast<sockaddr*>(&addr), len) != 0) {
                throw jxx::net::SocketException("bind failed: " + sock_error_string());
            }
            update_local_endpoint();
            bound_ = true;
        }

        // Optional: set a default remote peer (like Java connect(); filters inbound)
        void connect(const std::string& remoteHost, std::uint16_t remotePort) {
            ensure_open();
            sockaddr_storage ra{}; socklen_t rlen{};
            fill_sockaddr(remoteHost, remotePort, ra, rlen);
            if (::connect(sock_, reinterpret_cast<sockaddr*>(&ra), rlen) != 0) {
                throw jxx::net::SocketException("connect failed: " + sock_error_string());
            }
            connected_ = true;
            peerAddress_ = sockaddr_to_ip(reinterpret_cast<sockaddr*>(&ra), rlen, peerPort_);
            if (!bound_) {
                update_local_endpoint();
                bound_ = true;
            }
        }

        void disconnect() {
            ensure_open();
#if defined(_WIN32)
            sockaddr_storage ra{}; socklen_t rlen{};
            std::memset(&ra, 0, sizeof(ra));
            reinterpret_cast<sockaddr_in*>(&ra)->sin_family = AF_UNSPEC;
            if (::connect(sock_, reinterpret_cast<sockaddr*>(&ra), sizeof(sockaddr_in)) != 0) {
                // On Windows, this will likely set error but still disconnect; ignore
            }
#else
            // POSIX: no standard disconnect; connect to AF_UNSPEC to disassociate
            sockaddr_storage ra{}; std::memset(&ra, 0, sizeof(ra));
            reinterpret_cast<sockaddr_in*>(&ra)->sin_family = AF_UNSPEC;
            ::connect(sock_, reinterpret_cast<sockaddr*>(&ra), sizeof(sockaddr_in));
#endif
            connected_ = false;
            bound_ = false;
            peerAddress_.clear(); peerPort_ = 0;
        }

        // Send a packet; if connected and packet.address empty, send to connected peer
        void send(const NativeDatagramPacket& pkt) {
            ensure_open();
            if (pkt.offset > pkt.buffer.size() || pkt.length > pkt.buffer.size() - pkt.offset) {
                throw jxx::lang::IllegalArgumentException("DatagramPacket length exceeds buffer size");
            }
            const std::uint8_t* data = pkt.buffer.data();
            std::size_t len = pkt.length;

            if (connected_ && pkt.address.empty()) {
                auto sent = ::send(sock_, reinterpret_cast<const char*>(data), static_cast<int>(len), 0);
                if (sent < 0 || static_cast<std::size_t>(sent) != len) {
                    throw jxx::net::SocketException("send failed: " + sock_error_string());
                }
                if (!bound_) {
                    update_local_endpoint();
                    bound_ = true;
                }
                return;
            }

            // Else use destination from packet
            sockaddr_storage ra{}; socklen_t rlen{};
            if (pkt.address.empty() || pkt.port == 0) {
                throw jxx::lang::IllegalArgumentException("DatagramPacket missing destination address/port");
            }
            fill_sockaddr(pkt.address, pkt.port, ra, rlen);
            auto sent = ::sendto(sock_,
                reinterpret_cast<const char*>(data),
                static_cast<int>(len),
                0,
                reinterpret_cast<const sockaddr*>(&ra),
                rlen);
            if (sent < 0 || static_cast<std::size_t>(sent) != len) {
                throw jxx::net::SocketException("sendto failed: " + sock_error_string());
            }
            if (!bound_) {
                update_local_endpoint();
                bound_ = true;
            }
        }

        // Receive into packet's buffer; fills pkt.length and sender address/port.
        // The packet's buffer capacity determines max datagram size we can accept.
        void receive(NativeDatagramPacket& pkt) {
            ensure_open();
            if (pkt.buffer.empty()) {
                throw jxx::lang::IllegalArgumentException("DatagramPacket buffer is empty (set capacity first)");
            }
            sockaddr_storage from{}; socklen_t flen = sizeof(from);

            auto recvd = ::recvfrom(sock_,
                reinterpret_cast<char*>(pkt.buffer.data() + pkt.offset),
                static_cast<int>(pkt.buffer.size() - pkt.offset),
                0,
                reinterpret_cast<sockaddr*>(&from),
                &flen);
            if (recvd < 0) {
                const int error = last_sock_error();
                if (is_sock_wouldblock(error)) {
                    throw jxx::net::SocketTimeoutException("receive timed out");
                }
                throw jxx::net::SocketException(
                    "recvfrom failed: " + sock_strerror(error));
            }
            pkt.length = static_cast<std::size_t>(recvd);
            pkt.address = sockaddr_to_ip(reinterpret_cast<sockaddr*>(&from), flen, pkt.port);
            if (!bound_) {
                update_local_endpoint();
                bound_ = true;
            }
        }

        // -------- Options (Java parity) --------
        void setSoTimeout(int millis) {
            ensure_open();
            if (millis < 0) {
                throw jxx::lang::IllegalArgumentException("timeout is negative");
            }
#if defined(_WIN32)
            DWORD tv = static_cast<DWORD>(millis);
            if (setsockopt(sock_, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&tv), sizeof(tv)) != 0) {
                throw jxx::net::SocketException("setsockopt SO_RCVTIMEO failed: " + sock_error_string());
            }
#else
            timeval tv{};
            tv.tv_sec = millis / 1000;
            tv.tv_usec = (millis % 1000) * 1000;
            if (setsockopt(sock_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) != 0) {
                throw jxx::net::SocketException("setsockopt SO_RCVTIMEO failed: " + sock_error_string());
            }
#endif
            soTimeout_ = millis;
        }

        int getSoTimeout() const noexcept { return soTimeout_; }

        void setBroadcast(bool enabled) {
            ensure_open();
            int val = enabled ? 1 : 0;
            if (setsockopt(sock_, SOL_SOCKET, SO_BROADCAST,
#if defined(_WIN32)
                reinterpret_cast<const char*>(&val),
#else
                & val,
#endif
                sizeof(val)) != 0) {
                throw jxx::net::SocketException("setsockopt SO_BROADCAST failed: " + sock_error_string());
            }
            broadcast_ = enabled;
        }

        bool getBroadcast() const noexcept { return broadcast_; }

        void setReuseAddress(bool enabled) {
            ensure_open();
            int val = enabled ? 1 : 0;
            if (setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR,
#if defined(_WIN32)
                reinterpret_cast<const char*>(&val),
#else
                & val,
#endif
                sizeof(val)) != 0) {
                throw jxx::net::SocketException("setsockopt SO_REUSEADDR failed: " + sock_error_string());
            }
            reuseAddress_ = enabled;
        }

        bool getReuseAddress() const noexcept { return reuseAddress_; }

        void setSendBufferSize(int size) {
            ensure_open();
            if (size <= 0) {
                throw jxx::lang::IllegalArgumentException(
                    "send buffer size must be positive");
            }
            if (::setsockopt(sock_, SOL_SOCKET, SO_SNDBUF,
#if defined(_WIN32)
                reinterpret_cast<const char*>(&size),
#else
                &size,
#endif
                sizeof(size)) != 0) {
                throw jxx::net::SocketException(
                    "setsockopt SO_SNDBUF failed: " + sock_error_string());
            }
        }

        int getSendBufferSize() const {
            ensure_open();
            int size = 0;
            socklen_t length = sizeof(size);
            if (::getsockopt(sock_, SOL_SOCKET, SO_SNDBUF,
                reinterpret_cast<char*>(&size), &length) != 0) {
                throw jxx::net::SocketException(
                    "getsockopt SO_SNDBUF failed: " + sock_error_string());
            }
            return size;
        }

        void setReceiveBufferSize(int size) {
            ensure_open();
            if (size <= 0) {
                throw jxx::lang::IllegalArgumentException(
                    "receive buffer size must be positive");
            }
            if (::setsockopt(sock_, SOL_SOCKET, SO_RCVBUF,
#if defined(_WIN32)
                reinterpret_cast<const char*>(&size),
#else
                &size,
#endif
                sizeof(size)) != 0) {
                throw jxx::net::SocketException(
                    "setsockopt SO_RCVBUF failed: " + sock_error_string());
            }
        }

        int getReceiveBufferSize() const {
            ensure_open();
            int size = 0;
            socklen_t length = sizeof(size);
            if (::getsockopt(sock_, SOL_SOCKET, SO_RCVBUF,
                reinterpret_cast<char*>(&size), &length) != 0) {
                throw jxx::net::SocketException(
                    "getsockopt SO_RCVBUF failed: " + sock_error_string());
            }
            return size;
        }

        // -------- Query / lifecycle --------
        std::uint16_t getLocalPort() const { return localPort_; }
        std::string getLocalAddress() const { return localAddress_; }
        std::string getRemoteAddress() const { return peerAddress_; }
        std::uint16_t getRemotePort() const noexcept { return peerPort_; }

        bool isClosed() const noexcept { return sock_ == invalid_socket(); }
        bool isBound() const noexcept { return bound_; }
        bool isConnected() const noexcept { return connected_; }

        void close() noexcept {
            if (!isClosed()) {
                closesocket(sock_);
                sock_ = invalid_socket();
            }
            connected_ = false;
            bound_ = false;
            peerAddress_.clear(); peerPort_ = 0;
            localAddress_.clear(); localPort_ = 0;
            soTimeout_ = 0;
            broadcast_ = false;
            reuseAddress_ = false;
        }

    private:
        // -------- Internals --------
        static socket_t invalid_socket() {
#if defined(_WIN32)
            return INVALID_SOCKET;
#else
            return -1;
#endif
        }

        void open_unbound(Family fam = Family::Any) {
#if defined(_WIN32)
            wsa_ensure_started();
#endif
            if (fam == Family::IPv4) {
                family_ = AF_INET;
            }
            else if (fam == Family::IPv6) {
                family_ = AF_INET6;
            }
            else {
                family_ = AF_INET6; // try IPv6 first
            }

            sock_ = ::socket(family_, SOCK_DGRAM, IPPROTO_UDP);
            if (sock_ == invalid_socket() && fam == Family::Any) {
                // Fallback to IPv4 when Any fails on IPv6
                family_ = AF_INET;
                sock_ = ::socket(family_, SOCK_DGRAM, IPPROTO_UDP);
            }
            if (sock_ == invalid_socket()) {
                throw jxx::net::SocketException("socket() failed: " + sock_error_string());
            }

#ifdef IPV6_V6ONLY
            if (family_ == AF_INET6) {
                int off = 0;
                setsockopt(sock_, IPPROTO_IPV6, IPV6_V6ONLY,
#if defined(_WIN32)
                    reinterpret_cast<const char*>(&off),
#else
                    & off,
#endif
                    sizeof(off));
            }
#endif
            }

        void ensure_open() const {
            if (isClosed()) throw jxx::net::SocketException("DatagramSocket is closed");
        }

        void move_from(Impl&& other) noexcept {
            sock_ = other.sock_; other.sock_ = invalid_socket();
            connected_ = other.connected_; other.connected_ = false;
            bound_ = other.bound_; other.bound_ = false;
            localAddress_ = std::move(other.localAddress_); other.localAddress_.clear();
            peerAddress_ = std::move(other.peerAddress_);  other.peerAddress_.clear();
            localPort_ = other.localPort_;               other.localPort_ = 0;
            peerPort_ = other.peerPort_;                other.peerPort_ = 0;
            soTimeout_ = other.soTimeout_;               other.soTimeout_ = 0;
            broadcast_ = other.broadcast_;               other.broadcast_ = false;
            reuseAddress_ = other.reuseAddress_;          other.reuseAddress_ = false;
        }

        void update_local_endpoint() {
            sockaddr_storage la{}; socklen_t llen = sizeof(la);
            if (::getsockname(sock_, reinterpret_cast<sockaddr*>(&la), &llen) != 0) {
                throw jxx::net::SocketException("getsockname failed: " + sock_error_string());
            }
            localAddress_ = sockaddr_to_ip(reinterpret_cast<sockaddr*>(&la), llen, localPort_);
        }

        std::string sock_error_string() const {
            int e = last_sock_error();
            return sock_strerror(e) + " (code " + std::to_string(e) + ")";
        }

    private:
        socket_t    sock_{ invalid_socket() };
        int family_{ AF_UNSPEC }; // AF_INET or AF_INET6
        bool        connected_{ false };
        bool        bound_{ false };
        std::string localAddress_;
        std::uint16_t localPort_{ 0 };

        std::string peerAddress_;
        std::uint16_t peerPort_{ 0 };
        int soTimeout_{ 0 };
        bool broadcast_{ false };
        bool reuseAddress_{ false };
    public:
        void setTrafficClass(int value) { ensure_open(); if (::setsockopt(sock_, IPPROTO_IP, IP_TOS, reinterpret_cast<const char*>(&value), sizeof(value)) != 0) throw jxx::net::SocketException("setsockopt IP_TOS failed: " + sock_error_string()); }
        int getTrafficClass() const { ensure_open(); int value=0; socklen_t length=sizeof(value); if (::getsockopt(sock_, IPPROTO_IP, IP_TOS, reinterpret_cast<char*>(&value), &length) != 0) throw jxx::net::SocketException("getsockopt IP_TOS failed: " + sock_error_string()); return value; }
    };



static void validatePort_(::jxx::lang::jint port) {
    if (port < 0 || port > 65535) throw jxx::lang::IllegalArgumentException("port out of range");
}
DatagramPacket::DatagramPacket(const jxx::lang::ByteArray& b):DatagramPacket(b,0,b?b->length:0){}
DatagramPacket::DatagramPacket(const jxx::lang::ByteArray& b,::jxx::lang::jint n):DatagramPacket(b,0,n){}
DatagramPacket::DatagramPacket(const jxx::lang::ByteArray& b,::jxx::lang::jint o,::jxx::lang::jint n):buffer_(b),offset_(o),length_(n){validateRange_(o,n);}
DatagramPacket::DatagramPacket(const jxx::lang::ByteArray& b,::jxx::lang::jint n,const jxx::Ptr<InetAddress>& a,::jxx::lang::jint p):DatagramPacket(b,0,n,a,p){}
DatagramPacket::DatagramPacket(const jxx::lang::ByteArray& b,::jxx::lang::jint o,::jxx::lang::jint n,const jxx::Ptr<InetAddress>& a,::jxx::lang::jint p):DatagramPacket(b,o,n){setAddress(a);setPort(p);}
DatagramPacket::DatagramPacket(const jxx::lang::ByteArray& b,::jxx::lang::jint n,const jxx::Ptr<SocketAddress>& a):DatagramPacket(b,0,n,a){}
DatagramPacket::DatagramPacket(const jxx::lang::ByteArray& b,::jxx::lang::jint o,::jxx::lang::jint n,const jxx::Ptr<SocketAddress>& a):DatagramPacket(b,o,n){setSocketAddress(a);}
void DatagramPacket::validateRange_(::jxx::lang::jint o,::jxx::lang::jint n)const{if(!buffer_)throw jxx::lang::NullPointerException();if(o<0||n<0||o>buffer_->length||n>buffer_->length-o)throw jxx::lang::IllegalArgumentException("packet range out of bounds");}
jxx::Ptr<InetAddress> DatagramPacket::getAddress()const{return address_;} ::jxx::lang::jint DatagramPacket::getPort()const noexcept{return port_;} jxx::lang::ByteArray DatagramPacket::getData()const{return buffer_;} ::jxx::lang::jint DatagramPacket::getOffset()const noexcept{return offset_;} ::jxx::lang::jint DatagramPacket::getLength()const noexcept{return length_;}
jxx::Ptr<jxx::lang::String> DatagramPacket::getAddressText()const{return address_?address_->getHostAddress():nullptr;}
jxx::Ptr<SocketAddress> DatagramPacket::getSocketAddress()const{return address_?jxx::NEW<InetSocketAddress>(address_,port_):nullptr;}
void DatagramPacket::setAddress(const jxx::Ptr<InetAddress>&a){address_=a;} void DatagramPacket::setPort(::jxx::lang::jint p){validatePort_(p);port_=p;}
void DatagramPacket::setData(const jxx::lang::ByteArray&b){buffer_=b;offset_=0;length_=b?b->length:0;if(!b)throw jxx::lang::NullPointerException();}
void DatagramPacket::setData(const jxx::lang::ByteArray&b,::jxx::lang::jint o,::jxx::lang::jint n){buffer_=b;validateRange_(o,n);offset_=o;length_=n;} void DatagramPacket::setLength(::jxx::lang::jint n){validateRange_(offset_,n);length_=n;}
void DatagramPacket::setSocketAddress(const jxx::Ptr<SocketAddress>&a){auto i=std::dynamic_pointer_cast<InetSocketAddress>(a);if(!i)throw jxx::lang::IllegalArgumentException("unsupported socket address");if(i->isUnresolved())throw jxx::lang::IllegalArgumentException("unresolved socket address");address_=i->getAddress();port_=i->getPort();}

DatagramSocket::DatagramSocket():impl_(std::make_unique<Impl>()){} DatagramSocket::DatagramSocket(Family f):impl_(std::make_unique<Impl>(f)){}
DatagramSocket::DatagramSocket(::jxx::lang::jint p):DatagramSocket(jxx::NEW<InetSocketAddress>(p)){}
DatagramSocket::DatagramSocket(::jxx::lang::jint p,const jxx::Ptr<InetAddress>&a):DatagramSocket(jxx::NEW<InetSocketAddress>(a,p)){}
DatagramSocket::DatagramSocket(const jxx::Ptr<SocketAddress>&a):impl_(std::make_unique<Impl>()){if(a)bind(a);}
DatagramSocket::DatagramSocket(DatagramSocket&&)noexcept=default; DatagramSocket& DatagramSocket::operator=(DatagramSocket&&)noexcept=default; DatagramSocket::~DatagramSocket()=default;
void DatagramSocket::bind(const jxx::Ptr<SocketAddress>&a){auto i=std::dynamic_pointer_cast<InetSocketAddress>(a);if(!i)throw jxx::lang::IllegalArgumentException("unsupported socket address");impl_->bind(i->getHostString()->utf8(),static_cast<std::uint16_t>(i->getPort()));}
void DatagramSocket::connect(const jxx::Ptr<InetAddress>&a,::jxx::lang::jint p){if(!a)throw jxx::lang::NullPointerException();validatePort_(p);impl_->connect(a->getHostAddress()->utf8(),static_cast<std::uint16_t>(p));}
void DatagramSocket::connect(const jxx::Ptr<SocketAddress>&a){auto i=std::dynamic_pointer_cast<InetSocketAddress>(a);if(!i||i->isUnresolved())throw jxx::lang::IllegalArgumentException("unsupported socket address");connect(i->getAddress(),i->getPort());}
void DatagramSocket::disconnect(){impl_->disconnect();}
static NativeDatagramPacket native_(const jxx::Ptr<DatagramPacket>&p){if(!p)throw jxx::lang::NullPointerException();NativeDatagramPacket n;auto b=p->getData();n.offset=p->getOffset();n.length=p->getLength();n.buffer.resize(b?b->length:0);for(::jxx::lang::jint i=0;b&&i<b->length;++i)n.buffer[i]=static_cast<std::uint8_t>((*b)[i]);auto a=p->getAddressText();n.address=a?a->utf8():std::string();n.port=static_cast<std::uint16_t>(p->getPort());return n;}
void DatagramSocket::send(const jxx::Ptr<DatagramPacket>&p){auto n=native_(p);impl_->send(n);} void DatagramSocket::receive(const jxx::Ptr<DatagramPacket>&p){auto n=native_(p);impl_->receive(n);auto b=p->getData();for(std::size_t i=0;b&&i<n.length&&n.offset+i<static_cast<std::size_t>(b->length);++i)(*b)[static_cast<::jxx::lang::jint>(n.offset+i)]=static_cast<::jxx::lang::jbyte>(n.buffer[n.offset+i]);p->setLength(static_cast<::jxx::lang::jint>(n.length));p->setAddress(InetAddress::getByName(jxx::NEW<jxx::lang::String>(n.address)));p->setPort(n.port);}
void DatagramSocket::close()noexcept{if(impl_)impl_->close();}
jxx::Ptr<InetAddress> DatagramSocket::getInetAddress()const{auto a=impl_->getRemoteAddress();return a.empty()?nullptr:InetAddress::getByName(jxx::NEW<jxx::lang::String>(a));} jxx::Ptr<InetAddress> DatagramSocket::getLocalAddress()const{auto a=impl_->getLocalAddress();return a.empty()?nullptr:InetAddress::getByName(jxx::NEW<jxx::lang::String>(a));}
::jxx::lang::jint DatagramSocket::getPort()const noexcept{return impl_->getRemotePort();} ::jxx::lang::jint DatagramSocket::getLocalPort()const noexcept{return impl_->isBound()?impl_->getLocalPort():-1;}
jxx::Ptr<SocketAddress> DatagramSocket::getRemoteSocketAddress()const{auto a=getInetAddress();return a?jxx::NEW<InetSocketAddress>(a,getPort()):nullptr;} jxx::Ptr<SocketAddress> DatagramSocket::getLocalSocketAddress()const{auto a=getLocalAddress();return a?jxx::NEW<InetSocketAddress>(a,getLocalPort()):nullptr;} jxx::Ptr<jxx::nio::channels::DatagramChannel> DatagramSocket::getChannel()const{return nullptr;}
::jxx::lang::jbool DatagramSocket::isBound()const noexcept{return impl_&&impl_->isBound();} ::jxx::lang::jbool DatagramSocket::isConnected()const noexcept{return impl_&&impl_->isConnected();} ::jxx::lang::jbool DatagramSocket::isClosed()const noexcept{return !impl_||impl_->isClosed();}
#define V(n,t) void DatagramSocket::n(t v){impl_->n(v);} 
V(setSoTimeout,::jxx::lang::jint) V(setSendBufferSize,::jxx::lang::jint) V(setReceiveBufferSize,::jxx::lang::jint) V(setReuseAddress,::jxx::lang::jbool) V(setBroadcast,::jxx::lang::jbool)
#undef V
::jxx::lang::jint DatagramSocket::getSoTimeout()const noexcept{return impl_->getSoTimeout();} ::jxx::lang::jint DatagramSocket::getSendBufferSize()const{return impl_->getSendBufferSize();} ::jxx::lang::jint DatagramSocket::getReceiveBufferSize()const{return impl_->getReceiveBufferSize();} ::jxx::lang::jbool DatagramSocket::getReuseAddress()const noexcept{return impl_->getReuseAddress();} ::jxx::lang::jbool DatagramSocket::getBroadcast()const noexcept{return impl_->getBroadcast();}
void DatagramSocket::setTrafficClass(::jxx::lang::jint t){if(t<0||t>255)throw jxx::lang::IllegalArgumentException("traffic class out of range");impl_->setTrafficClass(t);} ::jxx::lang::jint DatagramSocket::getTrafficClass()const{return impl_->getTrafficClass();}
#define M(n,args,call) void DatagramSocket::n args{impl_->n call;}
M(joinGroupIPv4,(const std::string&a,const std::string&i),(a,i)) M(leaveGroupIPv4,(const std::string&a,const std::string&i),(a,i)) M(setMulticastTTL,(int v),(v)) M(setMulticastLoopIPv4,(bool v),(v)) M(setMulticastInterfaceIPv4,(const std::string&a),(a)) M(joinGroupIPv6,(const std::string&a,unsigned i),(a,i)) M(leaveGroupIPv6,(const std::string&a,unsigned i),(a,i)) M(setMulticastHopsIPv6,(int v),(v)) M(setMulticastLoopIPv6,(bool v),(v)) M(setMulticastInterfaceIPv6,(unsigned i),(i))
#undef M
} // namespace jxx::net
