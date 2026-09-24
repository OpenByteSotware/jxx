#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "net/jxx.net.DatagramSocket.h"

namespace jxx::net {

class MulticastSocket : public jxx::lang::ClassBase<MulticastSocket, DatagramSocket> {
public:
    using JxxSuper = DatagramSocket;
    using Super = jxx::lang::ClassBase<MulticastSocket, JxxSuper>;

    MulticastSocket();
    explicit MulticastSocket(Family family);
    explicit MulticastSocket(std::uint16_t localPort, Family family = Family::Any);
    ~MulticastSocket() override = default;

    void joinGroup(const std::string& groupIPv4,
        const std::string& interfaceIPv4 = "0.0.0.0");
    void leaveGroup(const std::string& groupIPv4,
        const std::string& interfaceIPv4 = "0.0.0.0");
    void joinGroupV6(const std::string& groupIPv6, unsigned interfaceIndex = 0);
    void leaveGroupV6(const std::string& groupIPv6, unsigned interfaceIndex = 0);

    void setTimeToLive(int ttl);
    int getTimeToLive() const noexcept;
    void setLoopback(bool enabled);
    void setLoopbackMode(bool disable);
    bool isLoopbackEnabled() const noexcept;

    void setInterface(const std::string& localInterfaceIPv4);
    std::string getInterface() const;
    void setNetworkInterface(unsigned interfaceIndex);
    unsigned getNetworkInterface() const noexcept;

    void sendToGroup(const std::vector<std::uint8_t>& payload,
        const std::string& groupAddress, std::uint16_t port);

private:
    int lastTtl_{1};
    bool lastLoopbackEnabled_{true};
    std::string lastInterfaceIPv4_;
    unsigned lastInterfaceIndexIPv6_{0};
};

} // namespace jxx::net
