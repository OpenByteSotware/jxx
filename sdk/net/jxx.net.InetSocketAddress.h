#pragma once

#include <string>
#include <string_view>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <utility>
#include <functional>
#include <type_traits>

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

namespace jxx::net{

#if defined(_WIN32)
    // RAII initializer for WinSock (no-op on POSIX)
    struct WinsockInit {
        WinsockInit() {
            WSADATA wsaData{};
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                // If this throws during static init, you can adjust to lazy init
                throw std::runtime_error("WSAStartup failed");
            }
        }
        ~WinsockInit() { WSACleanup(); }
    };
#endif

    class InetSocketAddress {
    public:
        // --- ctors ---

        // Default: unresolved, port=0
        InetSocketAddress() noexcept
            : port_(0), unresolved_(true), family_(AF_UNSPEC), sslen_(0) {
            std::memset(&ss_, 0, sizeof(ss_));
        }

        // Construct from host and port.
        // If deferResolve==false, we try to resolve immediately (like Java's InetSocketAddress(host, port)).
        // If resolution fails, we remain unresolved (isUnresolved()==true) and keep host string.
        explicit InetSocketAddress(std::string host, uint16_t port, bool deferResolve = false)
            : host_(std::move(host)), port_(port), unresolved_(true), family_(AF_UNSPEC), sslen_(0) {
            std::memset(&ss_, 0, sizeof(ss_));
            if (!deferResolve) {
                (void)resolve(); // best effort; remain unresolved if it fails
            }
        }

        // Create an unresolved address like Java's createUnresolved(host, port)
        static InetSocketAddress createUnresolved(std::string host, uint16_t port) {
            return InetSocketAddress(std::move(host), port, /*deferResolve*/true);
        }

        // Construct from a numeric ip string (e.g., "127.0.0.1" or "::1") and port (resolved).
        // If parsing fails, remains unresolved but preserves host string.
        static InetSocketAddress fromNumericIP(std::string ip, uint16_t port) {
            InetSocketAddress isa(std::move(ip), port, /*deferResolve*/true);
            isa.resolveNumericOnly(); // only attempt inet_pton parse, no DNS
            return isa;
        }

        // Construct from sockaddr (resolved)
        static InetSocketAddress fromSockaddr(const sockaddr* sa, socklen_t len) {
            InetSocketAddress isa;
            isa.setSockaddr(sa, len);
            isa.unresolved_ = false;
            isa.family_ = sa->sa_family;
            isa.host_ = isa.ipString(); // store numeric as hostString (similar to Java getHostString for numeric)
            return isa;
        }

        // Parse "host:port" and "[ipv6]:port"
        static bool parseHostPort(std::string_view s, std::string& host, uint16_t& port) {
            if (s.empty()) return false;
            if (s.front() == '[') {
                // [IPv6]:port
                auto rb = s.find(']');
                if (rb == std::string_view::npos) return false;
                host = std::string(s.substr(1, rb - 1));
                if (rb + 1 >= s.size() || s[rb + 1] != ':') return false;
                auto port_str = s.substr(rb + 2);
                return parsePort(port_str, port);
            }
            else {
                // host:port (host must not contain ':', unless it's numeric IPv6 -> must be bracketed)
                auto col = s.rfind(':');
                if (col == std::string_view::npos) return false;
                host = std::string(s.substr(0, col));
                auto port_str = s.substr(col + 1);
                return parsePort(port_str, port);
            }
        }

        // --- operations ---

        // Best-effort resolution (DNS). Returns true if resolved to an address.
        // Leaves object unresolved on failure, preserving host string & port.
        bool resolve(int family = AF_UNSPEC) {
#if defined(_WIN32)
            static WinsockInit _winsock_once;
            (void)_winsock_once;
#endif
            if (host_.empty()) return false;

            addrinfo hints{};
            hints.ai_family = family;      // AF_UNSPEC, AF_INET, AF_INET6
            hints.ai_socktype = SOCK_STREAM; // typical default (not strictly required)
            hints.ai_flags = AI_ADDRCONFIG; // avoid unusable families

            addrinfo* res = nullptr;
            const std::string port_str = std::to_string(port_);
            int rc = ::getaddrinfo(host_.c_str(), port_str.c_str(), &hints, &res);
            if (rc != 0 || !res) {
                unresolved_ = true;
                return false;
            }

            // pick first result
            setSockaddr(res->ai_addr, static_cast<socklen_t>(res->ai_addrlen));
            family_ = res->ai_family;
            unresolved_ = false;

            ::freeaddrinfo(res);
            return true;
        }

        // Whether the address could not be resolved (like Java's isUnresolved()).
        bool isUnresolved() const noexcept { return unresolved_; }

        // Port accessor
        uint16_t port() const noexcept { return port_; }

        // Returns the original host string (if unresolved) or the last known host string.
        // Mirrors Java's "getHostString()" (no reverse lookup).
        const std::string& hostString() const noexcept { return host_; }

        // Returns numeric IP string if resolved, otherwise empty string.
        std::string ipString() const {
            if (unresolved_ || sslen_ == 0) return {};
            char hostbuf[INET6_ADDRSTRLEN] = { 0 };
            if (ss_.ss_family == AF_INET) {
                const sockaddr_in* sin = reinterpret_cast<const sockaddr_in*>(&ss_);
                if (::inet_ntop(AF_INET, &sin->sin_addr, hostbuf, sizeof(hostbuf)))
                    return std::string(hostbuf);
            }
            else if (ss_.ss_family == AF_INET6) {
                const sockaddr_in6* sin6 = reinterpret_cast<const sockaddr_in6*>(&ss_);
                if (::inet_ntop(AF_INET6, &sin6->sin6_addr, hostbuf, sizeof(hostbuf)))
                    return std::string(hostbuf);
            }
            return {};
        }

        // Similar to Java's toString():
        // - unresolved: "host:port"
        // - resolved (IPv6): "[ip]:port"
        // - resolved (IPv4): "ip:port"
        std::string toString() const {
            if (unresolved_) {
                return host_ + ":" + std::to_string(port_);
            }
            const std::string ip = ipString();
            if (ss_.ss_family == AF_INET6) {
                return "[" + (ip.empty() ? std::string("?") : ip) + "]:" + std::to_string(port_);
            }
            return (ip.empty() ? std::string("?") : ip) + ":" + std::to_string(port_);
        }

        // Access to sockaddr for connect()/bind() once resolved
        const sockaddr* sockaddrPtr() const noexcept {
            return unresolved_ ? nullptr : reinterpret_cast<const sockaddr*>(&ss_);
        }
        socklen_t sockaddrLen() const noexcept { return unresolved_ ? 0 : sslen_; }
        int family() const noexcept { return family_; }

        // Equality / hash similar to Java:
        // - If both unresolved: equal iff hostString and port match (case-sensitive like Java docs imply).
        // - If both resolved: equal iff family, port, and raw address bytes match.
        // - If one resolved and other not: not equal.
        friend bool operator==(const InetSocketAddress& a, const InetSocketAddress& b) noexcept {
            if (a.unresolved_ || b.unresolved_) {
                return a.unresolved_ == b.unresolved_
                    && a.port_ == b.port_
                    && a.host_ == b.host_;
            }
            if (a.family_ != b.family_ || a.port_ != b.port_ || a.sslen_ != b.sslen_) return false;
            return std::memcmp(&a.ss_, &b.ss_, a.sslen_) == 0;
        }
        friend bool operator!=(const InetSocketAddress& a, const InetSocketAddress& b) noexcept {
            return !(a == b);
        }

        struct Hasher {
            std::size_t operator()(const InetSocketAddress& a) const noexcept {
                std::size_t h = std::hash<uint16_t>{}(a.port_);
                if (a.unresolved_) {
                    // Combine with host string
                    hashCombine(h, std::hash<std::string>{}(a.host_));
                }
                else {
                    // Combine family + raw address bytes
                    hashCombine(h, std::hash<int>{}(a.family_));
                    // simple byte-wise hash over sockaddr_storage (only first sslen_ bytes)
                    const auto* p = reinterpret_cast<const unsigned char*>(&a.ss_);
                    std::size_t local = 0xcbf29ce484222325ULL; // FNV-1a basis
                    for (socklen_t i = 0; i < a.sslen_; ++i) {
                        local ^= p[i];
                        local *= 1099511628211ULL;
                    }
                    hashCombine(h, local);
                }
                return h;
            }
        private:
            static void hashCombine(std::size_t& seed, std::size_t v) noexcept {
                // boost-like hash combine
                seed ^= v + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
            }
        };

    private:
        // Set internal sockaddr from external one (assumes valid len)
        void setSockaddr(const sockaddr* sa, socklen_t len) {
            sslen_ = (len <= sizeof(ss_)) ? len : static_cast<socklen_t>(sizeof(ss_));
            std::memset(&ss_, 0, sizeof(ss_));
            std::memcpy(&ss_, sa, sslen_);
            // Normalize port_ from sockaddr
            if (sa->sa_family == AF_INET) {
                port_ = ntohs(reinterpret_cast<const sockaddr_in*>(sa)->sin_port);
            }
            else if (sa->sa_family == AF_INET6) {
                port_ = ntohs(reinterpret_cast<const sockaddr_in6*>(sa)->sin6_port);
            }
        }

        // Only parse numeric literal; no DNS
        void resolveNumericOnly() {
            // Try IPv4
            sockaddr_in sin{};
            sin.sin_family = AF_INET;
            if (::inet_pton(AF_INET, host_.c_str(), &sin.sin_addr) == 1) {
                sin.sin_port = htons(port_);
                setSockaddr(reinterpret_cast<sockaddr*>(&sin), sizeof(sin));
                family_ = AF_INET;
                unresolved_ = false;
                return;
            }
            // Try IPv6
            sockaddr_in6 sin6{};
            sin6.sin6_family = AF_INET6;
            if (::inet_pton(AF_INET6, host_.c_str(), &sin6.sin6_addr) == 1) {
                sin6.sin6_port = htons(port_);
                setSockaddr(reinterpret_cast<sockaddr*>(&sin6), sizeof(sin6));
                family_ = AF_INET6;
                unresolved_ = false;
                return;
            }
            // Remains unresolved
            unresolved_ = true;
            family_ = AF_UNSPEC;
            sslen_ = 0;
            std::memset(&ss_, 0, sizeof(ss_));
        }

        static bool parsePort(std::string_view sv, uint16_t& out) {
            if (sv.empty()) return false;
            unsigned long long tmp = 0;
            for (char c : sv) {
                if (c < '0' || c > '9') return false;
                tmp = tmp * 10 + static_cast<unsigned>(c - '0');
                if (tmp > 65535ULL) return false;
            }
            out = static_cast<uint16_t>(tmp);
            return true;
        }

    private:
        std::string host_;        // original host string (or last known)
        uint16_t    port_;        // port in host byte order
        bool        unresolved_;  // mirrors Java's "unresolved" concept
        int         family_;      // AF_UNSPEC/AF_INET/AF_INET6
        sockaddr_storage ss_;
        socklen_t   sslen_;
    };

} // namespace net