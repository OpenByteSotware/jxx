#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"
#include "net/jxx.net.DatagramSocket.h"
#include "net/jxx.net.InetAddress.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.SocketException.h"
#include "net/jxx.net.SocketTimeoutException.h"
#include "net/jxx.net.UnknownHostException.h"

using jxx::net::DatagramPacket;
using jxx::net::DatagramSocket;
using jxx::net::Family;
using jxx::net::InetAddress;
using jxx::net::InetSocketAddress;
using ::testing::HasSubstr;

namespace {

jxx::lang::ByteArray Bytes(const std::string& value) {
    auto result = ::jxx::NEW<::jxx::lang::ByteArrayType>(
        static_cast<::jxx::lang::jint>(value.size()));
    for (::jxx::lang::jint index = 0; index < result->length; ++index) {
        (*result)[index] = static_cast<::jxx::lang::jbyte>(
            static_cast<unsigned char>(value[static_cast<std::size_t>(index)]));
    }
    return result;
}

std::string Text(const jxx::lang::ByteArray& value,
                 ::jxx::lang::jint offset,
                 ::jxx::lang::jint length) {
    std::string result;
    result.reserve(static_cast<std::size_t>(length));
    for (::jxx::lang::jint index = 0; index < length; ++index) {
        result.push_back(static_cast<char>(
            static_cast<unsigned char>((*value)[offset + index])));
    }
    return result;
}

jxx::Ptr<InetAddress> Address(const char* value) {
    return InetAddress::getByName(::jxx::NEW<::jxx::lang::String>(value));
}

jxx::Ptr<InetSocketAddress> Endpoint(const char* value,
                                    ::jxx::lang::jint port) {
    return ::jxx::NEW<InetSocketAddress>(Address(value), port);
}

} // namespace

#define EXPECT_THROW_MSG(stmt, ex_type, matcher)                                      \
    do {                                                                              \
        bool caught_ = false;                                                         \
        try { (void)(stmt); }                                                         \
        catch (const ex_type& e_) {                                                   \
            caught_ = true;                                                           \
            EXPECT_THAT(std::string(e_.what()), (matcher))                            \
                << "Exception message was: [" << e_.what() << "]";                   \
        }                                                                             \
        catch (...) {                                                                 \
            FAIL() << "Expected exception of type " #ex_type                         \
                   << ", but a different type was thrown";                           \
        }                                                                             \
        if (!caught_) {                                                               \
            FAIL() << "Expected exception of type " #ex_type " to be thrown";        \
        }                                                                             \
    } while (false)

TEST(DatagramSocket_Basic, ConstructAndCloseIPv4) {
    DatagramSocket socket(Family::IPv4);
    EXPECT_FALSE(socket.isClosed());
    socket.close();
    EXPECT_TRUE(socket.isClosed());
}

TEST(DatagramSocket_Bind, BindIPv4LoopbackUpdatesLocalEndpoint) {
    DatagramSocket socket(Family::IPv4);
    ASSERT_NO_THROW(socket.bind(Endpoint("127.0.0.1", 0)));
    EXPECT_NE(socket.getLocalPort(), 0);
    ASSERT_NE(nullptr, socket.getLocalAddress());
    EXPECT_EQ("127.0.0.1", socket.getLocalAddress()->getHostAddress()->utf8());
    EXPECT_NE(nullptr, socket.getLocalSocketAddress());
}

TEST(DatagramSocket_SendRecv, SendToAndReceiveFromLoopback) {
    DatagramSocket receiver(Family::IPv4);
    receiver.bind(Endpoint("127.0.0.1", 0));
    receiver.setSoTimeout(1500);

    const auto serverPort = receiver.getLocalPort();
    ASSERT_NE(serverPort, 0);

    DatagramSocket sender(Family::IPv4);
    const auto payload = Bytes("hello-udp");
    auto outgoing = ::jxx::NEW<DatagramPacket>(
        payload, payload->length, Address("127.0.0.1"), serverPort);
    ASSERT_NO_THROW(sender.send(outgoing));

    auto incoming = ::jxx::NEW<DatagramPacket>(
        ::jxx::NEW<::jxx::lang::ByteArrayType>(1024));
    ASSERT_NO_THROW(receiver.receive(incoming));
    ASSERT_EQ(incoming->getLength(), payload->length);
    EXPECT_EQ("hello-udp", Text(incoming->getData(), incoming->getOffset(),
                                incoming->getLength()));
    ASSERT_NE(nullptr, incoming->getAddress());
    EXPECT_EQ("127.0.0.1", incoming->getAddress()->getHostAddress()->utf8());
    EXPECT_NE(incoming->getPort(), 0);
}

TEST(DatagramSocket_SendRecv, ConnectedSendWithoutPacketDestination) {
    DatagramSocket receiver(Family::IPv4);
    receiver.bind(Endpoint("127.0.0.1", 0));
    receiver.setSoTimeout(1500);

    DatagramSocket client(Family::IPv4);
    ASSERT_NO_THROW(client.connect(Address("127.0.0.1"), receiver.getLocalPort()));

    const auto payload = Bytes("connected-path");
    auto outgoing = ::jxx::NEW<DatagramPacket>(payload, payload->length);
    ASSERT_NO_THROW(client.send(outgoing));

    auto incoming = ::jxx::NEW<DatagramPacket>(
        ::jxx::NEW<::jxx::lang::ByteArrayType>(256));
    ASSERT_NO_THROW(receiver.receive(incoming));
    ASSERT_EQ(incoming->getLength(), outgoing->getLength());
    EXPECT_EQ("connected-path", Text(incoming->getData(), incoming->getOffset(),
                                     incoming->getLength()));
    ASSERT_NE(nullptr, incoming->getAddress());
    EXPECT_EQ("127.0.0.1", incoming->getAddress()->getHostAddress()->utf8());
    EXPECT_NE(incoming->getPort(), 0);
    EXPECT_NO_THROW(client.disconnect());
}

TEST(DatagramSocket_Timeout, ReceiveTimeoutThrowsSocketTimeoutException) {
    DatagramSocket socket(Family::IPv4);
    socket.bind(Endpoint("127.0.0.1", 0));
    socket.setSoTimeout(100);
    auto packet = ::jxx::NEW<DatagramPacket>(
        ::jxx::NEW<::jxx::lang::ByteArrayType>(64));
    EXPECT_THROW_MSG(socket.receive(packet), ::jxx::net::SocketTimeoutException,
                     HasSubstr("timed out"));
}

TEST(DatagramSocket_MoveSemantics, MoveConstructorAndAssignment) {
    DatagramSocket first(Family::IPv4);
    first.bind(Endpoint("127.0.0.1", 0));
    const auto firstPort = first.getLocalPort();
    const auto firstAddress = first.getLocalAddress()->getHostAddress()->utf8();

    DatagramSocket second(std::move(first));
    EXPECT_TRUE(first.isClosed());
    EXPECT_FALSE(second.isClosed());
    EXPECT_EQ(firstPort, second.getLocalPort());
    EXPECT_EQ(firstAddress, second.getLocalAddress()->getHostAddress()->utf8());

    DatagramSocket third(Family::IPv4);
    third.bind(Endpoint("127.0.0.1", 0));
    const auto previousPort = third.getLocalPort();
    third = std::move(second);
    EXPECT_TRUE(second.isClosed());
    EXPECT_FALSE(third.isClosed());
    EXPECT_EQ(firstPort, third.getLocalPort());
    EXPECT_NE(previousPort, third.getLocalPort());
}

TEST(DatagramSocket_Options, SetBroadcastAndReuseAddress) {
    DatagramSocket socket(Family::IPv4);
    socket.bind(Endpoint("127.0.0.1", 0));
    EXPECT_NO_THROW(socket.setBroadcast(true));
    EXPECT_TRUE(socket.getBroadcast());
    EXPECT_NO_THROW(socket.setBroadcast(false));
    EXPECT_FALSE(socket.getBroadcast());
    EXPECT_NO_THROW(socket.setReuseAddress(true));
    EXPECT_TRUE(socket.getReuseAddress());
    EXPECT_NO_THROW(socket.setReuseAddress(false));
    EXPECT_FALSE(socket.getReuseAddress());
}

TEST(DatagramSocket_IPv6, DISABLED_BindAndSendRecvOnLoopback) {
    DatagramSocket receiver(Family::IPv6);
    ASSERT_NO_THROW(receiver.bind(Endpoint("::1", 0)));
    receiver.setSoTimeout(1500);

    DatagramSocket sender(Family::IPv6);
    const auto payload = Bytes("hello-ipv6");
    auto outgoing = ::jxx::NEW<DatagramPacket>(
        payload, payload->length, Address("::1"), receiver.getLocalPort());
    ASSERT_NO_THROW(sender.send(outgoing));

    auto incoming = ::jxx::NEW<DatagramPacket>(
        ::jxx::NEW<::jxx::lang::ByteArrayType>(256));
    ASSERT_NO_THROW(receiver.receive(incoming));
    ASSERT_EQ(incoming->getLength(), payload->length);
    EXPECT_EQ("hello-ipv6", Text(incoming->getData(), incoming->getOffset(),
                                 incoming->getLength()));
}

TEST(DatagramSocket_Negative, UseAfterCloseThrowsSocketException) {
    DatagramSocket socket(Family::IPv4);
    socket.close();
    EXPECT_THROW_MSG(socket.bind(Endpoint("127.0.0.1", 0)),
                     ::jxx::net::SocketException, HasSubstr("closed"));
    EXPECT_THROW_MSG(socket.connect(Address("127.0.0.1"), 9),
                     ::jxx::net::SocketException, HasSubstr("closed"));
    auto packet = ::jxx::NEW<DatagramPacket>(
        ::jxx::NEW<::jxx::lang::ByteArrayType>(32));
    EXPECT_THROW_MSG(socket.receive(packet), ::jxx::net::SocketException,
                     HasSubstr("closed"));
    EXPECT_THROW_MSG(socket.setBroadcast(true), ::jxx::net::SocketException,
                     HasSubstr("closed"));
}

TEST(DatagramSocket_Negative, MovedFromObjectReportsClosed) {
    DatagramSocket source(Family::IPv4);
    DatagramSocket target(std::move(source));
    EXPECT_TRUE(source.isClosed());
    EXPECT_NO_THROW(target.bind(Endpoint("127.0.0.1", 0)));
}

TEST(DatagramSocket_Negative, BindSamePortTwiceFailsWithoutReuse) {
    DatagramSocket first(Family::IPv4);
    first.bind(Endpoint("127.0.0.1", 0));
    DatagramSocket second(Family::IPv4);
    EXPECT_THROW_MSG(second.bind(Endpoint("127.0.0.1", first.getLocalPort())),
                     ::jxx::net::SocketException, HasSubstr("bind failed"));
}

TEST(DatagramSocket_Negative, BindWithInvalidAddressThrowsUnknownHost) {
    DatagramSocket socket(Family::IPv4);
    EXPECT_THROW(
        socket.bind(::jxx::NEW<InetSocketAddress>(
            ::jxx::NEW<::jxx::lang::String>("300.300.300.300"), 0)),
        ::jxx::lang::IllegalArgumentException);
}

TEST(DatagramSocket_Negative, ConnectWithInvalidAddressThrowsUnknownHost) {
    DatagramSocket socket(Family::IPv4);
    EXPECT_THROW(Address("256.256.256.256"), ::jxx::net::UnknownHostException);
}

TEST(DatagramSocket_Negative, SendWithoutDestinationWhenNotConnectedThrows) {
    DatagramSocket socket(Family::IPv4);
    const auto payload = Bytes("payload");
    auto packet = ::jxx::NEW<DatagramPacket>(payload, payload->length);
    EXPECT_THROW_MSG(socket.send(packet), ::jxx::lang::IllegalArgumentException,
                     HasSubstr("missing destination"));
}

TEST(DatagramSocket_Negative, DisconnectThenSendWithoutDestinationThrows) {
    DatagramSocket sender(Family::IPv4);
    DatagramSocket receiver(Family::IPv4);
    receiver.bind(Endpoint("127.0.0.1", 0));
    sender.connect(Address("127.0.0.1"), receiver.getLocalPort());
    sender.disconnect();

    const auto payload = Bytes("payload");
    auto packet = ::jxx::NEW<DatagramPacket>(payload, payload->length);
    EXPECT_THROW_MSG(sender.send(packet), ::jxx::lang::IllegalArgumentException,
                     HasSubstr("missing destination"));
}

TEST(DatagramSocket_Negative, ReceiveWithEmptyBufferThrows) {
    DatagramSocket socket(Family::IPv4);
    socket.bind(Endpoint("127.0.0.1", 0));
    auto packet = ::jxx::NEW<DatagramPacket>(
        ::jxx::NEW<::jxx::lang::ByteArrayType>(0));
    EXPECT_THROW_MSG(socket.receive(packet), ::jxx::lang::IllegalArgumentException,
                     HasSubstr("buffer is empty"));
}

TEST(DatagramSocket_Negative, MulticastFamilyMismatchUsesSocketException) {
    DatagramSocket ipv6(Family::IPv6);
    EXPECT_THROW(ipv6.joinGroupIPv4("239.1.1.1"), ::jxx::net::SocketException);
    EXPECT_THROW(ipv6.setMulticastTTL(2), ::jxx::net::SocketException);

    DatagramSocket ipv4(Family::IPv4);
    EXPECT_THROW(ipv4.joinGroupIPv6("ff12::1"), ::jxx::net::SocketException);
    EXPECT_THROW(ipv4.setMulticastHopsIPv6(2), ::jxx::net::SocketException);
}

TEST(DatagramSocket_Negative, InvalidMulticastArgumentsUseIllegalArgumentException) {
    DatagramSocket ipv4(Family::IPv4);
    EXPECT_THROW(ipv4.joinGroupIPv4("not_an_ip"),
                 ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(ipv4.leaveGroupIPv4("not_an_ip"),
                 ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(ipv4.setMulticastInterfaceIPv4("not_an_ip"),
                 ::jxx::lang::IllegalArgumentException);
}
