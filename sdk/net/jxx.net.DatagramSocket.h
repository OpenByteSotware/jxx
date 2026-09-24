#pragma once

#include <memory>
#include <string>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "net/jxx.net.SocketAddress.h"

namespace jxx::lang { class String; }
namespace jxx::nio::channels { class DatagramChannel; }
namespace jxx::net {
class InetAddress;
class InetSocketAddress;

enum class Family { Any, IPv4, IPv6 };

class DatagramPacket final : public jxx::lang::ClassBase<DatagramPacket, jxx::lang::Object> {
public:
    using JxxSuper = jxx::lang::Object;
    using Super = jxx::lang::ClassBase<DatagramPacket, JxxSuper>;

    explicit DatagramPacket(const jxx::lang::ByteArray& buffer);
    DatagramPacket(const jxx::lang::ByteArray& buffer, ::jxx::lang::jint length);
    DatagramPacket(const jxx::lang::ByteArray& buffer, ::jxx::lang::jint offset,
        ::jxx::lang::jint length);
    DatagramPacket(const jxx::lang::ByteArray& buffer, ::jxx::lang::jint length,
        const jxx::Ptr<InetAddress>& address, ::jxx::lang::jint port);
    DatagramPacket(const jxx::lang::ByteArray& buffer, ::jxx::lang::jint offset,
        ::jxx::lang::jint length, const jxx::Ptr<InetAddress>& address,
        ::jxx::lang::jint port);
    DatagramPacket(const jxx::lang::ByteArray& buffer, ::jxx::lang::jint length,
        const jxx::Ptr<SocketAddress>& address);
    DatagramPacket(const jxx::lang::ByteArray& buffer, ::jxx::lang::jint offset,
        ::jxx::lang::jint length, const jxx::Ptr<SocketAddress>& address);
    ~DatagramPacket() override = default;

    jxx::Ptr<InetAddress> getAddress() const;
    jxx::Ptr<jxx::lang::String> getAddressText() const;
    ::jxx::lang::jint getPort() const noexcept;
    jxx::lang::ByteArray getData() const;
    ::jxx::lang::jint getOffset() const noexcept;
    ::jxx::lang::jint getLength() const noexcept;
    jxx::Ptr<SocketAddress> getSocketAddress() const;
    void setAddress(const jxx::Ptr<InetAddress>& address);
    void setPort(::jxx::lang::jint port);
    void setData(const jxx::lang::ByteArray& buffer);
    void setData(const jxx::lang::ByteArray& buffer, ::jxx::lang::jint offset,
        ::jxx::lang::jint length);
    void setLength(::jxx::lang::jint length);
    void setSocketAddress(const jxx::Ptr<SocketAddress>& address);

private:
    void validateRange_(::jxx::lang::jint offset, ::jxx::lang::jint length) const;
    jxx::lang::ByteArray buffer_;
    ::jxx::lang::jint offset_{0};
    ::jxx::lang::jint length_{0};
    jxx::Ptr<InetAddress> address_;
    ::jxx::lang::jint port_{-1};
};

class DatagramSocket : public jxx::lang::ClassBase<DatagramSocket, jxx::lang::Object> {
public:
    using JxxSuper = jxx::lang::Object;
    using Super = jxx::lang::ClassBase<DatagramSocket, JxxSuper>;

    DatagramSocket();
    explicit DatagramSocket(::jxx::lang::jint port);
    DatagramSocket(::jxx::lang::jint port, const jxx::Ptr<InetAddress>& localAddress);
    explicit DatagramSocket(const jxx::Ptr<SocketAddress>& bindAddress);
    explicit DatagramSocket(Family family);
    DatagramSocket(const DatagramSocket&) = delete;
    DatagramSocket& operator=(const DatagramSocket&) = delete;
    DatagramSocket(DatagramSocket&& other) noexcept;
    DatagramSocket& operator=(DatagramSocket&& other) noexcept;
    ~DatagramSocket() override;

    void bind(const jxx::Ptr<SocketAddress>& address);
    void connect(const jxx::Ptr<InetAddress>& address, ::jxx::lang::jint port);
    void connect(const jxx::Ptr<SocketAddress>& address);
    void disconnect();
    void send(const jxx::Ptr<DatagramPacket>& packet);
    void receive(const jxx::Ptr<DatagramPacket>& packet);
    void close() noexcept;

    jxx::Ptr<InetAddress> getInetAddress() const;
    jxx::Ptr<InetAddress> getLocalAddress() const;
    ::jxx::lang::jint getPort() const noexcept;
    ::jxx::lang::jint getLocalPort() const noexcept;
    jxx::Ptr<SocketAddress> getRemoteSocketAddress() const;
    jxx::Ptr<SocketAddress> getLocalSocketAddress() const;
    jxx::Ptr<jxx::nio::channels::DatagramChannel> getChannel() const;
    ::jxx::lang::jbool isBound() const noexcept;
    ::jxx::lang::jbool isConnected() const noexcept;
    ::jxx::lang::jbool isClosed() const noexcept;

    void setSoTimeout(::jxx::lang::jint timeout);
    ::jxx::lang::jint getSoTimeout() const noexcept;
    void setSendBufferSize(::jxx::lang::jint size);
    ::jxx::lang::jint getSendBufferSize() const;
    void setReceiveBufferSize(::jxx::lang::jint size);
    ::jxx::lang::jint getReceiveBufferSize() const;
    void setReuseAddress(::jxx::lang::jbool enabled);
    ::jxx::lang::jbool getReuseAddress() const noexcept;
    void setBroadcast(::jxx::lang::jbool enabled);
    ::jxx::lang::jbool getBroadcast() const noexcept;
    void setTrafficClass(::jxx::lang::jint trafficClass);
    ::jxx::lang::jint getTrafficClass() const;

    // Native multicast extensions used by MulticastSocket.
    void joinGroupIPv4(const std::string&, const std::string& = "0.0.0.0");
    void leaveGroupIPv4(const std::string&, const std::string& = "0.0.0.0");
    void setMulticastTTL(int); void setMulticastLoopIPv4(bool);
    void setMulticastInterfaceIPv4(const std::string&);
    void joinGroupIPv6(const std::string&, unsigned = 0);
    void leaveGroupIPv6(const std::string&, unsigned = 0);
    void setMulticastHopsIPv6(int); void setMulticastLoopIPv6(bool);
    void setMulticastInterfaceIPv6(unsigned);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace jxx::net
