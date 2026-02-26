#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <string>
#include "jxx.h"

using jxx::net::DatagramPacket;
using jxx::net::DatagramSocket;
using jxx::net::Family;

using ::testing::HasSubstr;
using ::testing::AllOf;

class RunnableForSocketReadt : public jxx::lang::Runnable {
 
    bool runThread_{ true };
public:

    void stop() { runThread_ = false; }

    virtual void run() override {
        while (runThread_) {
         }
    }
};

class TestDatagramSocketTest : public testing::Test {
protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.

    TestDatagramSocketTest() {
        // You can do set-up work for each test here.
    }

    ~TestDatagramSocketTest() override {
        // You can do clean-up work that doesn't throw exceptions here.
    } 

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
       }

    void TearDown() override {
     }

    // Class members declared here can be used by all tests in the test suite
    // for Foo.
};

static std::vector<std::uint8_t> Bytes(std::string s) {
    return std::vector<std::uint8_t>(s.begin(), s.end());
}

// Helper macro to assert exception type and inspect e.what()
#define EXPECT_THROW_MSG(stmt, ex_type, matcher)                                      \
    do {                                                                              \
        bool caught_ = false;                                                         \
        try { (void)(stmt); }                                                         \
        catch (const ex_type& e_) {                                                   \
            caught_ = true;                                                           \
            EXPECT_THAT(std::string(e_.what()), (matcher))                            \
                << "Exception message was: [" << e_.what() << "]";                    \
        }                                                                             \
        catch (...) {                                                                 \
            FAIL() << "Expected exception of type " #ex_type ", but different thrown";\
        }                                                                             \
        if (!caught_) {                                                               \
            FAIL() << "Expected exception of type " #ex_type " to be thrown";         \
        }                                                                             \
    } while (0)

// ---------------------------- Positive tests (unchanged core coverage) ---------------

TEST(DatagramSocket_Basic, ConstructAndCloseIPv4) {
    DatagramSocket s(Family::IPv4);
    EXPECT_FALSE(s.isClosed());
    s.close();
    EXPECT_TRUE(s.isClosed());
}

TEST(DatagramSocket_Bind, BindIPv4LoopbackUpdatesLocalEndpoint) {
    DatagramSocket s(Family::IPv4);
    ASSERT_NO_THROW(s.bind("127.0.0.1", 0));
    EXPECT_NE(s.getLocalPort(), 0);
    EXPECT_EQ(s.getLocalAddress(), "127.0.0.1");
}

TEST(DatagramSocket_SendRecv, SendToAndReceiveFromLoopback) {
    DatagramSocket recvSock(Family::IPv4);
    recvSock.bind("127.0.0.1", 0);
    recvSock.setSoTimeout(1500);

    const auto serverPort = recvSock.getLocalPort();
    ASSERT_NE(serverPort, 0);

    DatagramSocket sendSock(Family::IPv4);

    const auto payload = Bytes("hello-udp");
    DatagramPacket out(payload, "127.0.0.1", serverPort);
    ASSERT_NO_THROW(sendSock.send(out));

    DatagramPacket in(1024);
    ASSERT_NO_THROW(recvSock.receive(in));
    ASSERT_EQ(in.length, payload.size());
    EXPECT_EQ(std::string(in.buffer.begin(), in.buffer.begin() + in.length), "hello-udp");
    EXPECT_EQ(in.address, "127.0.0.1");
    EXPECT_NE(in.port, 0u);
}

TEST(DatagramSocket_SendRecv, ConnectedSendWithoutDestinationFields) {
    DatagramSocket receiver(Family::IPv4);
    receiver.bind("127.0.0.1", 0);
    receiver.setSoTimeout(1500);

    const auto port = receiver.getLocalPort();
    ASSERT_NE(port, 0);

    DatagramSocket client(Family::IPv4);
    ASSERT_NO_THROW(client.connect("127.0.0.1", port));

    DatagramPacket pkt;
    pkt.buffer = Bytes("connected-path");
    pkt.length = pkt.buffer.size();

    ASSERT_NO_THROW(client.send(pkt));

    DatagramPacket in(256);
    ASSERT_NO_THROW(receiver.receive(in));
    ASSERT_EQ(in.length, pkt.length);
    EXPECT_EQ(std::string(in.buffer.begin(), in.buffer.begin() + in.length), "connected-path");
    EXPECT_EQ(in.address, "127.0.0.1");
    EXPECT_NE(in.port, 0u);

    EXPECT_NO_THROW(client.disconnect());
}

TEST(DatagramSocket_Timeout, ReceiveTimeoutThrows) {
    DatagramSocket s(Family::IPv4);
    s.bind("127.0.0.1", 0);
    s.setSoTimeout(100);

    DatagramPacket in(64);
    //EXPECT_THROW(in.length = 0, std::runtime_error); // dummy to keep clang-tidy quiet
    // Check both type and message
    EXPECT_THROW_MSG(s.receive(in), std::runtime_error, HasSubstr("recvfrom failed:"));
}

TEST(DatagramSocket_MoveSemantics, MoveConstructorAndAssignment) {
    DatagramSocket a(Family::IPv4);
    a.bind("127.0.0.1", 0);
    const auto aPort = a.getLocalPort();
    const auto aAddr = a.getLocalAddress();

    DatagramSocket b(std::move(a));
    EXPECT_TRUE(a.isClosed());
    EXPECT_FALSE(b.isClosed());
    EXPECT_EQ(b.getLocalPort(), aPort);
    EXPECT_EQ(b.getLocalAddress(), aAddr);

    DatagramSocket c(Family::IPv4);
    c.bind("127.0.0.1", 0);
    const auto cOldPort = c.getLocalPort();

    c = std::move(b);
    EXPECT_TRUE(b.isClosed());
    EXPECT_FALSE(c.isClosed());
    EXPECT_EQ(c.getLocalPort(), aPort);
    EXPECT_NE(c.getLocalPort(), cOldPort);
}

TEST(DatagramSocket_Options, SetBroadcastAndReuseAddress) {
    DatagramSocket s(Family::IPv4);
    s.bind("127.0.0.1", 0);

    EXPECT_NO_THROW(s.setBroadcast(true));
    EXPECT_NO_THROW(s.setBroadcast(false));
    EXPECT_NO_THROW(s.setReuseAddress(true));
    EXPECT_NO_THROW(s.setReuseAddress(false));
}

// IPv6 positive test is disabled by default (environment dependent)
TEST(DatagramSocket_IPv6, DISABLED_BindAndSendRecvOnLoopback) {
    DatagramSocket recvSock(Family::IPv6);
    ASSERT_NO_THROW(recvSock.bind("::1", 0));
    recvSock.setSoTimeout(1500);

    const auto port = recvSock.getLocalPort();
    ASSERT_NE(port, 0);

    DatagramSocket sendSock(Family::IPv6);
    auto payload = Bytes("hello-ipv6");
    DatagramPacket out(payload, "::1", port);

    ASSERT_NO_THROW(sendSock.send(out));

    DatagramPacket in(256);
    ASSERT_NO_THROW(recvSock.receive(in));

    ASSERT_EQ(in.length, payload.size());
    EXPECT_EQ(std::string(in.buffer.begin(), in.buffer.begin() + in.length), "hello-ipv6");
    EXPECT_EQ(in.address, "::1");
    EXPECT_NE(in.port, 0u);
}

// ---------------------------- Negative tests with EXPECT_THAT ------------------------

TEST(DatagramSocket_Negative, UseAfterCloseThrowsLogicError) {
    DatagramSocket s(Family::IPv4);
    s.close();

    EXPECT_TRUE(s.isClosed());

    EXPECT_THROW_MSG(s.bind("127.0.0.1", 0), std::logic_error,
        HasSubstr("DatagramSocket is closed"));
    EXPECT_THROW_MSG(s.connect("127.0.0.1", 9), std::logic_error,
        HasSubstr("DatagramSocket is closed"));

    DatagramPacket pkt(32);
    EXPECT_THROW_MSG(s.receive(pkt), std::logic_error,
        HasSubstr("DatagramSocket is closed"));
    EXPECT_THROW_MSG(s.setBroadcast(true), std::logic_error,
        HasSubstr("DatagramSocket is closed"));
}

TEST(DatagramSocket_Negative, MovedFromObjectThrowsOnUse) {
    DatagramSocket s(Family::IPv4);
    DatagramSocket t(std::move(s)); // s becomes moved-from/closed
    EXPECT_TRUE(s.isClosed());
    EXPECT_NO_THROW(t.bind("127.0.0.1", 0));

    EXPECT_THROW_MSG(s.bind("127.0.0.1", 0), std::logic_error,
        HasSubstr("DatagramSocket is closed"));
}

TEST(DatagramSocket_Negative, BindSamePortTwiceFailsWithoutReuse) {
    DatagramSocket s1(Family::IPv4);
    s1.bind("127.0.0.1", 0);
    const auto port = s1.getLocalPort();
    ASSERT_NE(port, 0);

    DatagramSocket s2(Family::IPv4);
    EXPECT_THROW_MSG(s2.bind("127.0.0.1", port), std::runtime_error,
        HasSubstr("bind failed:"));
}

TEST(DatagramSocket_Negative, BindWithInvalidAddressThrows) {
    DatagramSocket s(Family::IPv4);
    EXPECT_THROW_MSG(s.bind("300.300.300.300", 0), std::runtime_error,
        HasSubstr("getaddrinfo failed:"));
}

TEST(DatagramSocket_Negative, ConnectWithInvalidAddressThrows) {
    DatagramSocket s(Family::IPv4);
    EXPECT_THROW_MSG(s.connect("256.256.256.256", 12345), std::runtime_error,
        HasSubstr("getaddrinfo failed:"));
}

TEST(DatagramSocket_Negative, SendWithoutDestinationWhenNotConnectedThrows) {
    DatagramSocket s(Family::IPv4);
    DatagramPacket pkt;
    pkt.buffer = Bytes("payload");
    pkt.length = pkt.buffer.size();

    EXPECT_THROW_MSG(s.send(pkt), std::invalid_argument,
        HasSubstr("DatagramPacket missing destination address/port"));
}

TEST(DatagramSocket_Negative, DisconnectThenSendWithoutDestinationThrows) {
    DatagramSocket s(Family::IPv4);
    DatagramSocket r(Family::IPv4);
    r.bind("127.0.0.1", 0);

    ASSERT_NO_THROW(s.connect("127.0.0.1", r.getLocalPort()));
    s.disconnect();

    DatagramPacket pkt;
    pkt.buffer = Bytes("payload");
    pkt.length = pkt.buffer.size();

    EXPECT_THROW_MSG(s.send(pkt), std::invalid_argument,
        HasSubstr("DatagramPacket missing destination address/port"));
}

TEST(DatagramSocket_Negative, ReceiveWithEmptyBufferThrows) {
    DatagramSocket s(Family::IPv4);
    s.bind("127.0.0.1", 0);

    DatagramPacket pkt; // buffer is empty
    EXPECT_THROW_MSG(s.receive(pkt), std::invalid_argument,
        HasSubstr("DatagramPacket buffer is empty (set capacity first)"));
}

TEST(DatagramSocket_Negative, SendToInvalidAddressThrowsAtResolution) {
    DatagramSocket s(Family::IPv4);
    DatagramPacket out(Bytes("hi"), "256.256.256.256", 12345);
    EXPECT_THROW_MSG(s.send(out), std::runtime_error,
        HasSubstr("getaddrinfo failed:"));
}

TEST(DatagramSocket_Negative, IPv4MulticastAPIsOnIPv6SocketThrowLogicError) {
    DatagramSocket s(Family::IPv6);

    EXPECT_THROW_MSG(s.joinGroupIPv4("239.1.1.1"), std::logic_error,
        HasSubstr("Socket is not IPv4"));
    EXPECT_THROW_MSG(s.leaveGroupIPv4("239.1.1.1"), std::logic_error,
        HasSubstr("Socket is not IPv4"));
    EXPECT_THROW_MSG(s.setMulticastTTL(2), std::logic_error,
        HasSubstr("setMulticastTTL: socket is not IPv4"));
    EXPECT_THROW_MSG(s.setMulticastLoopIPv4(true), std::logic_error,
        HasSubstr("setMulticastLoopIPv4: socket is not IPv4"));
    EXPECT_THROW_MSG(s.setMulticastInterfaceIPv4("127.0.0.1"), std::logic_error,
        HasSubstr("setMulticastInterfaceIPv4: socket is not IPv4"));
}

TEST(DatagramSocket_Negative, IPv6MulticastAPIsOnIPv4SocketThrowLogicError) {
    DatagramSocket s(Family::IPv4);

    EXPECT_THROW_MSG(s.joinGroupIPv6("ff12::1"), std::logic_error,
        HasSubstr("Socket is not IPv6"));
    EXPECT_THROW_MSG(s.leaveGroupIPv6("ff12::1"), std::logic_error,
        HasSubstr("Socket is not IPv6"));
    EXPECT_THROW_MSG(s.setMulticastHopsIPv6(2), std::logic_error,
        HasSubstr("setMulticastHopsIPv6: socket is not IPv6"));
    EXPECT_THROW_MSG(s.setMulticastLoopIPv6(true), std::logic_error,
        HasSubstr("setMulticastLoopIPv6: socket is not IPv6"));
    EXPECT_THROW_MSG(s.setMulticastInterfaceIPv6(0), std::logic_error,
        HasSubstr("setMulticastInterfaceIPv6: socket is not IPv6"));
}

TEST(DatagramSocket_Negative, IPv4MulticastInvalidParamsThrow) {
    DatagramSocket s(Family::IPv4);

    EXPECT_THROW_MSG(s.joinGroupIPv4("not_an_ip"), std::invalid_argument,
        HasSubstr("joinGroupIPv4: invalid group address"));
    EXPECT_THROW_MSG(s.leaveGroupIPv4("not_an_ip"), std::invalid_argument,
        HasSubstr("leaveGroupIPv4: invalid group address"));
    EXPECT_THROW_MSG(s.setMulticastInterfaceIPv4("not_an_ip"), std::invalid_argument,
        HasSubstr("setMulticastInterfaceIPv4: invalid interface address"));
}

TEST(DatagramSocket_Negative, DISABLED_IPv6MulticastInvalidGroupThrows) {
    DatagramSocket s(Family::IPv6);
    EXPECT_THROW_MSG(s.joinGroupIPv6("not_an_ipv6"), std::invalid_argument,
        HasSubstr("joinGroupIPv6: invalid group address"));
    EXPECT_THROW_MSG(s.leaveGroupIPv6("also_bad"), std::invalid_argument,
        HasSubstr("leaveGroupIPv6: invalid group address"));
}
