#include "net/jxx.net.MulticastSocket.h"

#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InetAddress.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "net/jxx.net.SocketException.h"

namespace jxx::net {

MulticastSocket::MulticastSocket()
    : Super() {}

MulticastSocket::MulticastSocket(Family family)
    : Super(family) {}

MulticastSocket::MulticastSocket(std::uint16_t localPort, Family family)
    : Super(family) {
    const auto addressText = family == Family::IPv6 ? "::" : "0.0.0.0";
    const auto address = InetAddress::getByName(
        jxx::NEW<jxx::lang::String>(addressText));
    bind(jxx::NEW<InetSocketAddress>(address,
        static_cast<jxx::lang::jint>(localPort)));
}

void MulticastSocket::joinGroup(const std::string& groupIPv4,
    const std::string& interfaceIPv4) {
    joinGroupIPv4(groupIPv4, interfaceIPv4);
}

void MulticastSocket::leaveGroup(const std::string& groupIPv4,
    const std::string& interfaceIPv4) {
    leaveGroupIPv4(groupIPv4, interfaceIPv4);
}

void MulticastSocket::joinGroupV6(const std::string& groupIPv6,
    unsigned interfaceIndex) {
    joinGroupIPv6(groupIPv6, interfaceIndex);
}

void MulticastSocket::leaveGroupV6(const std::string& groupIPv6,
    unsigned interfaceIndex) {
    leaveGroupIPv6(groupIPv6, interfaceIndex);
}

void MulticastSocket::setTimeToLive(int ttl) {
    if (ttl < 0 || ttl > 255) {
        throw jxx::lang::IllegalArgumentException("time to live out of range");
    }
    try {
        setMulticastTTL(ttl);
    } catch (const jxx::net::SocketException&) {
        setMulticastHopsIPv6(ttl);
    }
    lastTtl_ = ttl;
}

int MulticastSocket::getTimeToLive() const noexcept { return lastTtl_; }

void MulticastSocket::setLoopback(bool enabled) {
    bool applied = false;
    try { setMulticastLoopIPv4(enabled); applied = true; } catch (const jxx::net::SocketException&) {}
    try { setMulticastLoopIPv6(enabled); applied = true; } catch (const jxx::net::SocketException&) {}
    if (!applied) {
        throw jxx::net::SocketException("multicast loop option could not be applied");
    }
    lastLoopbackEnabled_ = enabled;
}

void MulticastSocket::setLoopbackMode(bool disable) { setLoopback(!disable); }
bool MulticastSocket::isLoopbackEnabled() const noexcept { return lastLoopbackEnabled_; }

void MulticastSocket::setInterface(const std::string& localInterfaceIPv4) {
    setMulticastInterfaceIPv4(localInterfaceIPv4);
    lastInterfaceIPv4_ = localInterfaceIPv4;
}
std::string MulticastSocket::getInterface() const { return lastInterfaceIPv4_; }

void MulticastSocket::setNetworkInterface(unsigned interfaceIndex) {
    setMulticastInterfaceIPv6(interfaceIndex);
    lastInterfaceIndexIPv6_ = interfaceIndex;
}
unsigned MulticastSocket::getNetworkInterface() const noexcept { return lastInterfaceIndexIPv6_; }

void MulticastSocket::sendToGroup(const std::vector<std::uint8_t>& payload,
    const std::string& groupAddress, std::uint16_t port) {
    auto bytes = jxx::NEW<jxx::lang::ByteArrayType>(static_cast<jxx::lang::jint>(payload.size()));
    for (jxx::lang::jint i = 0; i < bytes->length; ++i) {
        (*bytes)[i] = static_cast<jxx::lang::jbyte>(payload[static_cast<std::size_t>(i)]);
    }
    auto address = InetAddress::getByName(jxx::NEW<jxx::lang::String>(groupAddress));
    send(jxx::NEW<DatagramPacket>(bytes, bytes->length, address, port));
}

} // namespace jxx::net
