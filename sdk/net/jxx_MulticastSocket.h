#pragma once
#include "jxx_DatagramSocket.h"
#ifndef _WIN32
#include <net/if.h> // if_nametoindex helper used in examples
#endif
#include <string>
#include <stdexcept>
#include <utility>

namespace jxx::net {
    

    class MulticastSocket : public DatagramSocket {
    public:
        // ---- Constructors (mirror Java) ----
        // Unbound (choose family automatically; see notes below)
        MulticastSocket()
            : DatagramSocket() {}

        // Unbound with explicit address family preference
        explicit MulticastSocket(Family fam)
            : DatagramSocket(fam) {}

        // Bind immediately to local port on ANY address (0.0.0.0 or ::)
        explicit MulticastSocket(std::uint16_t localPort, Family fam = Family::Any)
            : DatagramSocket(fam) {
            // Bind to ANY address for the selected family
            if (fam == Family::IPv6) {
                bind("::", localPort);
            }
            else {
                bind("0.0.0.0", localPort);
            }
        }

        // ---- Java-like API ----

        // joinGroup (IPv4): group like "239.255.0.1" and optional interface IPv4 address.
        void joinGroup(const std::string& groupIPv4, const std::string& ifaceIPv4 = "0.0.0.0") {
            joinGroupIPv4(groupIPv4, ifaceIPv4);
        }

        // leaveGroup (IPv4)
        void leaveGroup(const std::string& groupIPv4, const std::string& ifaceIPv4 = "0.0.0.0") {
            leaveGroupIPv4(groupIPv4, ifaceIPv4);
        }

        // joinGroup (IPv6): group like "ff12::1234" and interface index (0 = default)
        void joinGroupV6(const std::string& groupIPv6, unsigned ifindex = 0) {
            joinGroupIPv6(groupIPv6, ifindex);
        }

        // leaveGroup (IPv6)
        void leaveGroupV6(const std::string& groupIPv6, unsigned ifindex = 0) {
            leaveGroupIPv6(groupIPv6, ifindex);
        }

        // setTimeToLive (Java: int TTL). For IPv6 we map to hop limit.
        // We try IPv4 first; if not valid for current socket, try IPv6.
        void setTimeToLive(int ttl) {
            lastTtl_ = ttl;
            // Try IPv4 setter
            try {
                setMulticastTTL(ttl);
                return;
            }
            catch (...) {
                // Fall through and try IPv6
            }
            setMulticastHopsIPv6(ttl);
        }

        int getTimeToLive() const noexcept { return lastTtl_; }

        // Java has setLoopbackMode(boolean disable). Here we provide both:
        // - setLoopback(true/false) -> intuitive enable/disable
        // - setLoopbackMode(disable) -> Java parity (true means disable)
        void setLoopback(bool enable) {
            lastLoopbackEnabled_ = enable;
            bool ok = false;
            // Try IPv4
            try { setMulticastLoopIPv4(enable); ok = true; }
            catch (...) {}
            // Try IPv6
            try { setMulticastLoopIPv6(enable); ok = true; }
            catch (...) {}
            if (!ok) throw std::logic_error("setLoopback: neither IPv4 nor IPv6 multicast loop option applied");
        }

        // Java-parity: true means disable loopback
        void setLoopbackMode(bool disable) { setLoopback(!disable); }

        bool isLoopbackEnabled() const noexcept { return lastLoopbackEnabled_; }

        // Java's setInterface/getInterface for IPv4
        void setInterface(const std::string& localInterfaceIPv4) {
            setMulticastInterfaceIPv4(localInterfaceIPv4);
            lastIfaceV4_ = localInterfaceIPv4;
        }
        std::string getInterface() const { return lastIfaceV4_; }

        // Java's setNetworkInterface/getNetworkInterface, IPv6 by index
        void setNetworkInterface(unsigned ifindex) {
            setMulticastInterfaceIPv6(ifindex);
            lastIfindexV6_ = ifindex;
        }
        unsigned getNetworkInterface() const noexcept { return lastIfindexV6_; }

        // Convenience send to a multicast group (IPv4 or IPv6)
        void sendToGroup(const std::vector<std::uint8_t>& payload,
            const std::string& groupAddr,
            std::uint16_t port)
        {
            DatagramPacket pkt(payload, groupAddr, port);
            send(pkt);
        }

    private:
        // Cached options for getters (OS query not done portably)
        int         lastTtl_{ 1 };             // default TTL/hop limit
        bool        lastLoopbackEnabled_{ true };
        std::string lastIfaceV4_{};
        unsigned    lastIfindexV6_{ 0 };
    };

} // namespace jxx::net