#pragma once
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
        if (rc != 0) throw std::runtime_error("WSAStartup failed: " + std::to_string(rc));
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

namespace jxx::net {
    enum class Family { Any, IPv4, IPv6 };
    // ---------------------- DatagramPacket ----------------------
    struct DatagramPacket {
        // Buffer to send or receive
        std::vector<std::uint8_t> buffer;

        // Valid length in buffer (for send, bytes to send; for receive, bytes received)
        std::size_t length{ 0 };

        // Remote endpoint (for send; for receive, filled with sender)
        std::string address;     // hostname or numeric IP
        std::uint16_t port{ 0 };

        DatagramPacket() = default;

        // Create a packet with a given buffer capacity for receiving
        explicit DatagramPacket(std::size_t capacity)
            : buffer(capacity), length(0) {}

        // Create a packet for sending (copies payload)
        DatagramPacket(const std::vector<std::uint8_t>& payload,
            std::string addr,
            std::uint16_t prt)
            : buffer(payload), length(payload.size()), address(std::move(addr)), port(prt) {}
    };

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
            throw std::runtime_error("getaddrinfo failed: " + std::to_string(rc));
#else
            throw std::runtime_error(std::string("getaddrinfo failed: ") + gai_strerror(rc));
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
            throw std::runtime_error("getnameinfo failed: " + std::to_string(rc));
#else
            throw std::runtime_error(std::string("getnameinfo failed: ") + gai_strerror(rc));
#endif
        }
        portOut = static_cast<std::uint16_t>(std::stoul(serv));
        return std::string(host);
    }

    // ---------------------- DatagramSocket ----------------------
    class DatagramSocket {
    public:
        explicit DatagramSocket(Family fam) { open_unbound(fam); }
        DatagramSocket() { open_unbound(); }

        // Bind immediately to local port (optionally local address like "0.0.0.0" or "::")
        explicit DatagramSocket(std::uint16_t localPort, const std::string& localAddress = std::string()) {
            open_unbound();
            if (!localAddress.empty()) {
                bind(localAddress, localPort);
            }
            else {
                bind(localPort);
            }
        }

        // Non-copyable; movable
        DatagramSocket(const DatagramSocket&) = delete;
        DatagramSocket& operator=(const DatagramSocket&) = delete;

        DatagramSocket(DatagramSocket&& other) noexcept { move_from(std::move(other)); }
        DatagramSocket& operator=(DatagramSocket&& other) noexcept {
            if (this != &other) {
                close();
                move_from(std::move(other));
            }
            return *this;
        }

        ~DatagramSocket() { close(); }
        // -------- Multicast (IPv4) --------
// Join IPv4 multicast group on a specific local interface (IPv4 address).
// Example interface: "0.0.0.0" for default, or "192.168.1.5"
        void joinGroupIPv4(const std::string& groupAddress, const std::string& localInterfaceIPv4 = "0.0.0.0") {
            ensure_open();
            if (family_ != AF_INET) {
                throw std::logic_error("Socket is not IPv4; construct with Family::IPv4 for IPv4 multicast");
            }
            ip_mreq mreq{};
            mreq.imr_multiaddr.s_addr = ::inet_addr(groupAddress.c_str());
            mreq.imr_interface.s_addr = ::inet_addr(localInterfaceIPv4.c_str());
            if (mreq.imr_multiaddr.s_addr == INADDR_NONE) {
                throw std::invalid_argument("joinGroupIPv4: invalid group address");
            }
            if (setsockopt(sock_, IPPROTO_IP, IP_ADD_MEMBERSHIP,
#if defined(_WIN32)
                reinterpret_cast<const char*>(&mreq),
#else
                & mreq,
#endif
                sizeof(mreq)) != 0) {
                throw std::runtime_error("IP_ADD_MEMBERSHIP failed: " + sock_error_string());
            }
        }

        void leaveGroupIPv4(const std::string& groupAddress, const std::string& localInterfaceIPv4 = "0.0.0.0") {
            ensure_open();
            if (family_ != AF_INET) {
                throw std::logic_error("Socket is not IPv4; construct with Family::IPv4 for IPv4 multicast");
            }
            ip_mreq mreq{};
            mreq.imr_multiaddr.s_addr = ::inet_addr(groupAddress.c_str());
            mreq.imr_interface.s_addr = ::inet_addr(localInterfaceIPv4.c_str());
            if (mreq.imr_multiaddr.s_addr == INADDR_NONE) {
                throw std::invalid_argument("leaveGroupIPv4: invalid group address");
            }
            if (setsockopt(sock_, IPPROTO_IP, IP_DROP_MEMBERSHIP,
#if defined(_WIN32)
                reinterpret_cast<const char*>(&mreq),
#else
                & mreq,
#endif
                sizeof(mreq)) != 0) {
                throw std::runtime_error("IP_DROP_MEMBERSHIP failed: " + sock_error_string());
            }
        }

        // Set IPv4 multicast TTL (hop count). Default in many stacks is 1.
        void setMulticastTTL(int ttl) {
            ensure_open();
            if (family_ != AF_INET) {
                throw std::logic_error("setMulticastTTL: socket is not IPv4");
            }
            unsigned char t = static_cast<unsigned char>(ttl);
            if (setsockopt(sock_, IPPROTO_IP, IP_MULTICAST_TTL,
#if defined(_WIN32)
                reinterpret_cast<const char*>(&t),
#else
                & t,
#endif
                sizeof(t)) != 0) {
                throw std::runtime_error("IP_MULTICAST_TTL failed: " + sock_error_string());
            }
        }

        // Enable/disable IPv4 multicast loopback
        void setMulticastLoopIPv4(bool enable) {
            ensure_open();
            if (family_ != AF_INET) {
                throw std::logic_error("setMulticastLoopIPv4: socket is not IPv4");
            }
            unsigned char on = enable ? 1 : 0;
            if (setsockopt(sock_, IPPROTO_IP, IP_MULTICAST_LOOP,
#if defined(_WIN32)
                reinterpret_cast<const char*>(&on),
#else
                & on,
#endif
                sizeof(on)) != 0) {
                throw std::runtime_error("IP_MULTICAST_LOOP failed: " + sock_error_string());
            }
        }

        // Select IPv4 outgoing interface by local IPv4 address
        void setMulticastInterfaceIPv4(const std::string& localInterfaceIPv4) {
            ensure_open();
            if (family_ != AF_INET) {
                throw std::logic_error("setMulticastInterfaceIPv4: socket is not IPv4");
            }
            in_addr ia{}; ia.s_addr = ::inet_addr(localInterfaceIPv4.c_str());
            if (ia.s_addr == INADDR_NONE) {
                throw std::invalid_argument("setMulticastInterfaceIPv4: invalid interface address");
            }
            if (setsockopt(sock_, IPPROTO_IP, IP_MULTICAST_IF,
#if defined(_WIN32)
                reinterpret_cast<const char*>(&ia),
#else
                & ia,
#endif
                sizeof(ia)) != 0) {
                throw std::runtime_error("IP_MULTICAST_IF failed: " + sock_error_string());
            }
        }

        // -------- Multicast (IPv6) --------
        // Join IPv6 multicast group (e.g., "ff12::1234") on interface index (0 = default)
        void joinGroupIPv6(const std::string& groupAddress, unsigned int ifindex = 0) {
            ensure_open();
            if (family_ != AF_INET6) {
                throw std::logic_error("Socket is not IPv6; construct with Family::IPv6 for IPv6 multicast");
            }
            ipv6_mreq mreq{};
            // Convert textual address to in6_addr
#if defined(_WIN32)
            IN6_ADDR addr6{};
            if (InetPtonA(AF_INET6, groupAddress.c_str(), &addr6) != 1) {
                throw std::invalid_argument("joinGroupIPv6: invalid group address");
            }
            std::memcpy(&mreq.ipv6mr_multiaddr, &addr6, sizeof(IN6_ADDR));
#else
            if (::inet_pton(AF_INET6, groupAddress.c_str(), &mreq.ipv6mr_multiaddr) != 1) {
                throw std::invalid_argument("joinGroupIPv6: invalid group address");
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
                throw std::runtime_error("IPV6_JOIN_GROUP failed: " + sock_error_string());
            }
        }

        void leaveGroupIPv6(const std::string& groupAddress, unsigned int ifindex = 0) {
            ensure_open();
            if (family_ != AF_INET6) {
                throw std::logic_error("Socket is not IPv6; construct with Family::IPv6 for IPv6 multicast");
            }
            ipv6_mreq mreq{};
#if defined(_WIN32)
            IN6_ADDR addr6{};
            if (InetPtonA(AF_INET6, groupAddress.c_str(), &addr6) != 1) {
                throw std::invalid_argument("leaveGroupIPv6: invalid group address");
            }
            std::memcpy(&mreq.ipv6mr_multiaddr, &addr6, sizeof(IN6_ADDR));
#else
            if (::inet_pton(AF_INET6, groupAddress.c_str(), &mreq.ipv6mr_multiaddr) != 1) {
                throw std::invalid_argument("leaveGroupIPv6: invalid group address");
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
                throw std::runtime_error("IPV6_LEAVE_GROUP failed: " + sock_error_string());
            }
        }

        // IPv6 hop limit (TTL analog)
        void setMulticastHopsIPv6(int hops) {
            ensure_open();
            if (family_ != AF_INET6) {
                throw std::logic_error("setMulticastHopsIPv6: socket is not IPv6");
            }
            int h = hops;
            if (setsockopt(sock_, IPPROTO_IPV6, IPV6_MULTICAST_HOPS,
#if defined(_WIN32)
                reinterpret_cast<const char*>(&h),
#else
                & h,
#endif
                sizeof(h)) != 0) {
                throw std::runtime_error("IPV6_MULTICAST_HOPS failed: " + sock_error_string());
            }
        }

        // Enable/disable IPv6 multicast loopback
        void setMulticastLoopIPv6(bool enable) {
            ensure_open();
            if (family_ != AF_INET6) {
                throw std::logic_error("setMulticastLoopIPv6: socket is not IPv6");
            }
            unsigned int on = enable ? 1u : 0u;
            if (setsockopt(sock_, IPPROTO_IPV6, IPV6_MULTICAST_LOOP,
#if defined(_WIN32)
                reinterpret_cast<const char*>(&on),
#else
                & on,
#endif
                sizeof(on)) != 0) {
                throw std::runtime_error("IPV6_MULTICAST_LOOP failed: " + sock_error_string());
            }
        }

        // Select IPv6 outgoing interface by index (0 = default)
        void setMulticastInterfaceIPv6(unsigned int ifindex) {
            ensure_open();
            if (family_ != AF_INET6) {
                throw std::logic_error("setMulticastInterfaceIPv6: socket is not IPv6");
            }
            unsigned int idx = ifindex;
            if (setsockopt(sock_, IPPROTO_IPV6, IPV6_MULTICAST_IF,
#if defined(_WIN32)
                reinterpret_cast<const char*>(&idx),
#else
                & idx,
#endif
                sizeof(idx)) != 0) {
                throw std::runtime_error("IPV6_MULTICAST_IF failed: " + sock_error_string());
            }
        }

        // -------- Core operations --------
        void bind(std::uint16_t localPort) {
            bind("0.0.0.0", localPort); // INADDR_ANY (IPv4). If you want dual-stack, use "::" on IPv6-enabled OS.
        }

        void bind(const std::string& localAddress, std::uint16_t localPort) {
            ensure_open();
            // Allow re-bind options to be set before bind if desired
            sockaddr_storage addr{}; socklen_t len{};
            fill_sockaddr(localAddress, localPort, addr, len);
            if (::bind(sock_, reinterpret_cast<sockaddr*>(&addr), len) != 0) {
                throw std::runtime_error("bind failed: " + sock_error_string());
            }
            update_local_endpoint();
        }

        // Optional: set a default remote peer (like Java connect(); filters inbound)
        void connect(const std::string& remoteHost, std::uint16_t remotePort) {
            ensure_open();
            sockaddr_storage ra{}; socklen_t rlen{};
            fill_sockaddr(remoteHost, remotePort, ra, rlen);
            if (::connect(sock_, reinterpret_cast<sockaddr*>(&ra), rlen) != 0) {
                throw std::runtime_error("connect failed: " + sock_error_string());
            }
            connected_ = true;
            peerAddress_ = sockaddr_to_ip(reinterpret_cast<sockaddr*>(&ra), rlen, peerPort_);
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
            peerAddress_.clear(); peerPort_ = 0;
        }

        // Send a packet; if connected and packet.address empty, send to connected peer
        void send(const DatagramPacket& pkt) {
            ensure_open();
            const std::uint8_t* data = pkt.buffer.data();
            std::size_t len = pkt.length;

            if (connected_ && pkt.address.empty()) {
                auto sent = ::send(sock_, reinterpret_cast<const char*>(data), static_cast<int>(len), 0);
                if (sent < 0 || static_cast<std::size_t>(sent) != len) {
                    throw std::runtime_error("send failed: " + sock_error_string());
                }
                return;
            }

            // Else use destination from packet
            sockaddr_storage ra{}; socklen_t rlen{};
            if (pkt.address.empty() || pkt.port == 0) {
                throw std::invalid_argument("DatagramPacket missing destination address/port");
            }
            fill_sockaddr(pkt.address, pkt.port, ra, rlen);
            auto sent = ::sendto(sock_,
                reinterpret_cast<const char*>(data),
                static_cast<int>(len),
                0,
                reinterpret_cast<const sockaddr*>(&ra),
                rlen);
            if (sent < 0 || static_cast<std::size_t>(sent) != len) {
                throw std::runtime_error("sendto failed: " + sock_error_string());
            }
        }

        // Receive into packet's buffer; fills pkt.length and sender address/port.
        // The packet's buffer capacity determines max datagram size we can accept.
        void receive(DatagramPacket& pkt) {
            ensure_open();
            if (pkt.buffer.empty()) {
                throw std::invalid_argument("DatagramPacket buffer is empty (set capacity first)");
            }
            sockaddr_storage from{}; socklen_t flen = sizeof(from);

            auto recvd = ::recvfrom(sock_,
                reinterpret_cast<char*>(pkt.buffer.data()),
                static_cast<int>(pkt.buffer.size()),
                0,
                reinterpret_cast<sockaddr*>(&from),
                &flen);
            if (recvd < 0) {
                int e = last_sock_error();
                // Timeout manifests as EAGAIN/WOULDBLOCK or WSAETIMEDOUT depending on platform
                throw std::runtime_error("recvfrom failed: " + sock_strerror(e));
            }
            pkt.length = static_cast<std::size_t>(recvd);
            pkt.address = sockaddr_to_ip(reinterpret_cast<sockaddr*>(&from), flen, pkt.port);
        }

        // -------- Options (Java parity) --------
        void setSoTimeout(int millis) {
            ensure_open();
#if defined(_WIN32)
            DWORD tv = static_cast<DWORD>(millis);
            if (setsockopt(sock_, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&tv), sizeof(tv)) != 0) {
                throw std::runtime_error("setsockopt SO_RCVTIMEO failed: " + sock_error_string());
            }
#else
            timeval tv{};
            tv.tv_sec = millis / 1000;
            tv.tv_usec = (millis % 1000) * 1000;
            if (setsockopt(sock_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) != 0) {
                throw std::runtime_error("setsockopt SO_RCVTIMEO failed: " + sock_error_string());
            }
#endif
        }

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
                throw std::runtime_error("setsockopt SO_BROADCAST failed: " + sock_error_string());
            }
        }

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
                throw std::runtime_error("setsockopt SO_REUSEADDR failed: " + sock_error_string());
            }
        }

        // -------- Query / lifecycle --------
        std::uint16_t getLocalPort() const { return localPort_; }
        std::string   getLocalAddress() const { return localAddress_; }

        bool isClosed() const noexcept { return sock_ == invalid_socket(); }

        void close() noexcept {
            if (!isClosed()) {
                closesocket(sock_);
                sock_ = invalid_socket();
            }
            connected_ = false;
            peerAddress_.clear(); peerPort_ = 0;
            localAddress_.clear(); localPort_ = 0;
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
                throw std::runtime_error("socket() failed: " + sock_error_string());
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
            if (isClosed()) throw std::logic_error("DatagramSocket is closed");
        }

        void move_from(DatagramSocket&& other) noexcept {
            sock_ = other.sock_; other.sock_ = invalid_socket();
            connected_ = other.connected_; other.connected_ = false;
            localAddress_ = std::move(other.localAddress_); other.localAddress_.clear();
            peerAddress_ = std::move(other.peerAddress_);  other.peerAddress_.clear();
            localPort_ = other.localPort_;               other.localPort_ = 0;
            peerPort_ = other.peerPort_;                other.peerPort_ = 0;
        }

        void update_local_endpoint() {
            sockaddr_storage la{}; socklen_t llen = sizeof(la);
            if (::getsockname(sock_, reinterpret_cast<sockaddr*>(&la), &llen) != 0) {
                throw std::runtime_error("getsockname failed: " + sock_error_string());
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
        std::string localAddress_;
        std::uint16_t localPort_{ 0 };

        std::string peerAddress_;
        std::uint16_t peerPort_{ 0 };
    };

} // namespace jxx::net
