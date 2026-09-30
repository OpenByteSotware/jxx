#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "net/jxx.net.SocketAddress.h"

namespace jxx::lang { class String; }
namespace jxx::net {
class InetAddress;
class InetSocketAddress;

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

} // namespace jxx::net
