#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace jxx::net {

enum class Family { Any, IPv4, IPv6 };

struct DatagramPacket {
    std::vector<std::uint8_t> buffer;
    std::size_t offset{0};
    std::size_t length{0};
    std::string address;
    std::uint16_t port{0};

    DatagramPacket();
    explicit DatagramPacket(std::size_t capacity);
    DatagramPacket(const std::vector<std::uint8_t>& payload,
        const std::string& address, std::uint16_t port);

    void setData(const std::vector<std::uint8_t>& data);
    void setData(const std::vector<std::uint8_t>& data,
        std::size_t offset, std::size_t length);
    void setLength(std::size_t length);
};

class DatagramSocket {
public:
    DatagramSocket();
    explicit DatagramSocket(Family family);
    explicit DatagramSocket(std::uint16_t localPort,
        const std::string& localAddress = std::string());
    DatagramSocket(const DatagramSocket&) = delete;
    DatagramSocket& operator=(const DatagramSocket&) = delete;
    DatagramSocket(DatagramSocket&& other) noexcept;
    DatagramSocket& operator=(DatagramSocket&& other) noexcept;
    virtual ~DatagramSocket();

    void joinGroupIPv4(const std::string& groupAddress,
        const std::string& localInterfaceIPv4 = "0.0.0.0");
    void leaveGroupIPv4(const std::string& groupAddress,
        const std::string& localInterfaceIPv4 = "0.0.0.0");
    void setMulticastTTL(int ttl);
    void setMulticastLoopIPv4(bool enable);
    void setMulticastInterfaceIPv4(const std::string& address);
    void joinGroupIPv6(const std::string& groupAddress, unsigned ifindex = 0);
    void leaveGroupIPv6(const std::string& groupAddress, unsigned ifindex = 0);
    void setMulticastHopsIPv6(int hops);
    void setMulticastLoopIPv6(bool enable);
    void setMulticastInterfaceIPv6(unsigned ifindex);

    void bind(std::uint16_t localPort);
    void bind(const std::string& localAddress, std::uint16_t localPort);
    void connect(const std::string& remoteHost, std::uint16_t remotePort);
    void disconnect();
    void send(const DatagramPacket& packet);
    void receive(DatagramPacket& packet);

    void setSoTimeout(int millis);
    int getSoTimeout() const noexcept;
    void setBroadcast(bool enabled);
    bool getBroadcast() const noexcept;
    void setReuseAddress(bool enabled);
    bool getReuseAddress() const noexcept;
    void setSendBufferSize(int size);
    int getSendBufferSize() const;
    void setReceiveBufferSize(int size);
    int getReceiveBufferSize() const;

    std::uint16_t getLocalPort() const noexcept;
    std::string getLocalAddress() const;
    std::string getRemoteAddress() const;
    std::uint16_t getRemotePort() const noexcept;
    bool isClosed() const noexcept;
    bool isBound() const noexcept;
    bool isConnected() const noexcept;
    void close() noexcept;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace jxx::net
