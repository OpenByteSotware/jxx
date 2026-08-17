#include <memory>
#include <string>
#include <type_traits>

#include <gtest/gtest.h>

#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

#include "net/jxx.net.InetAddress.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.SocketAddress.h"

namespace {

    using jxx::lang::Object;
    using jxx::lang::String;
    using jxx::lang::jbool;
    using jxx::lang::jint;

    using jxx::net::InetAddress;
    using jxx::net::InetSocketAddress;
    using jxx::net::SocketAddress;

    static jxx::Ptr<String> S(const char* value) {
        return jxx::NEW<String>(value);
    }

    static jxx::Ptr<String> S(const std::string& value) {
        return jxx::NEW<String>(value);
    }

    static std::string textOf(
        const jxx::Ptr<String>& value) {

        return value == nullptr
            ? std::string()
            : value->utf8();
    }

    static jxx::Ptr<Object> asObject(
        const jxx::Ptr<InetSocketAddress>& address) {

        return jxx::CAST<Object>(address);
    }

    /*
     * Type hierarchy
     */

    TEST(InetSocketAddressTest, ExtendsSocketAddress) {
        static_assert(
            std::is_base_of_v<
            SocketAddress,
            InetSocketAddress>,
            "InetSocketAddress must extend SocketAddress");

        static_assert(
            std::is_base_of_v<
            Object,
            InetSocketAddress>,
            "InetSocketAddress must transitively extend Object");

        SUCCEED();
    }

    /*
     * Port-only constructor
     */

    TEST(InetSocketAddressTest, PortOnlyConstructorStoresPort) {
        auto socketAddress =
            jxx::NEW<InetSocketAddress>(
                static_cast<jint>(8080));

        ASSERT_NE(socketAddress, nullptr);
        EXPECT_EQ(socketAddress->getPort(), 8080);
        EXPECT_FALSE(socketAddress->isUnresolved());
    }

    TEST(InetSocketAddressTest, PortOnlyConstructorCreatesWildcardAddress) {
        auto socketAddress =
            jxx::NEW<InetSocketAddress>(
                static_cast<jint>(8080));

        auto address = socketAddress->getAddress();

        ASSERT_NE(address, nullptr);
        EXPECT_TRUE(address->isAnyLocalAddress());
    }

    TEST(InetSocketAddressTest, PortOnlyConstructorRejectsNegativePort) {
        EXPECT_THROW(
            jxx::NEW<InetSocketAddress>(
                static_cast<jint>(-1)),
            jxx::lang::IllegalArgumentException);
    }

    TEST(InetSocketAddressTest, PortOnlyConstructorRejectsPortAbove65535) {
        EXPECT_THROW(
            jxx::NEW<InetSocketAddress>(
                static_cast<jint>(65536)),
            jxx::lang::IllegalArgumentException);
    }

    TEST(InetSocketAddressTest, PortOnlyConstructorAcceptsZero) {
        auto socketAddress =
            jxx::NEW<InetSocketAddress>(
                static_cast<jint>(0));

        ASSERT_NE(socketAddress, nullptr);
        EXPECT_EQ(socketAddress->getPort(), 0);
    }

    TEST(InetSocketAddressTest, PortOnlyConstructorAccepts65535) {
        auto socketAddress =
            jxx::NEW<InetSocketAddress>(
                static_cast<jint>(65535));

        ASSERT_NE(socketAddress, nullptr);
        EXPECT_EQ(socketAddress->getPort(), 65535);
    }

    /*
     * InetAddress constructor
     */

    TEST(InetSocketAddressTest, InetAddressConstructorStoresAddressAndPort) {
        auto loopback =
            InetAddress::getLoopbackAddress();

        ASSERT_NE(loopback, nullptr);

        auto socketAddress =
            jxx::NEW<InetSocketAddress>(
                loopback,
                static_cast<jint>(4321));

        ASSERT_NE(socketAddress, nullptr);
        EXPECT_EQ(socketAddress->getPort(), 4321);
        EXPECT_FALSE(socketAddress->isUnresolved());

        auto storedAddress =
            socketAddress->getAddress();

        ASSERT_NE(storedAddress, nullptr);
        EXPECT_TRUE(storedAddress->equals(loopback));
    }

    TEST(InetSocketAddressTest, InetAddressConstructorWithNullUsesWildcardAddress) {
        jxx::Ptr<InetAddress> nullAddress = nullptr;

        auto socketAddress =
            jxx::NEW<InetSocketAddress>(
                nullAddress,
                static_cast<jint>(1234));

        ASSERT_NE(socketAddress, nullptr);
        EXPECT_FALSE(socketAddress->isUnresolved());

        auto storedAddress =
            socketAddress->getAddress();

        ASSERT_NE(storedAddress, nullptr);
        EXPECT_TRUE(storedAddress->isAnyLocalAddress());
    }

    TEST(InetSocketAddressTest, InetAddressConstructorRejectsNegativePort) {
        auto loopback =
            InetAddress::getLoopbackAddress();

        EXPECT_THROW(
            jxx::NEW<InetSocketAddress>(
                loopback,
                static_cast<jint>(-1)),
            jxx::lang::IllegalArgumentException);
    }

    TEST(InetSocketAddressTest, InetAddressConstructorRejectsPortAbove65535) {
        auto loopback =
            InetAddress::getLoopbackAddress();

        EXPECT_THROW(
            jxx::NEW<InetSocketAddress>(
                loopback,
                static_cast<jint>(65536)),
            jxx::lang::IllegalArgumentException);
    }

    /*
     * Hostname constructor
     */

    TEST(InetSocketAddressTest, NumericHostnameConstructorStoresPort) {
        auto socketAddress =
            jxx::NEW<InetSocketAddress>(
                S("127.0.0.1"),
                static_cast<jint>(9090));

        ASSERT_NE(socketAddress, nullptr);
        EXPECT_EQ(socketAddress->getPort(), 9090);
    }

    TEST(InetSocketAddressTest, NumericHostnameResolvesAddress) {
        auto socketAddress =
            jxx::NEW<InetSocketAddress>(
                S("127.0.0.1"),
                static_cast<jint>(9090));

        ASSERT_NE(socketAddress, nullptr);
        EXPECT_FALSE(socketAddress->isUnresolved());

        auto address =
            socketAddress->getAddress();

        ASSERT_NE(address, nullptr);

        EXPECT_TRUE(
            address->isLoopbackAddress());
    }

    TEST(InetSocketAddressTest, NumericHostnamePreservesHostString) {
        auto socketAddress =
            jxx::NEW<InetSocketAddress>(
                S("127.0.0.1"),
                static_cast<jint>(9090));

        auto hostString =
            socketAddress->getHostString();

        ASSERT_NE(hostString, nullptr);
        EXPECT_EQ(hostString->utf8(), "127.0.0.1");
    }

    TEST(InetSocketAddressTest, HostnameConstructorRejectsNullHostname) {
        jxx::Ptr<String> nullHost = nullptr;

        EXPECT_THROW(
            jxx::NEW<InetSocketAddress>(
                nullHost,
                static_cast<jint>(8080)),
            jxx::lang::IllegalArgumentException);
    }

    TEST(InetSocketAddressTest, HostnameConstructorRejectsInvalidPort) {
        EXPECT_THROW(
            jxx::NEW<InetSocketAddress>(
                S("127.0.0.1"),
                static_cast<jint>(-1)),
            jxx::lang::IllegalArgumentException);

        EXPECT_THROW(
            jxx::NEW<InetSocketAddress>(
                S("127.0.0.1"),
                static_cast<jint>(65536)),
            jxx::lang::IllegalArgumentException);
    }

    /*
     * createUnresolved
     */

    TEST(InetSocketAddressTest, CreateUnresolvedStoresHostnameAndPort) {
        auto socketAddress =
            InetSocketAddress::createUnresolved(
                S("unresolved.example.invalid"),
                static_cast<jint>(8443));

        ASSERT_NE(socketAddress, nullptr);

        EXPECT_EQ(socketAddress->getPort(), 8443);
        EXPECT_TRUE(socketAddress->isUnresolved());
        EXPECT_EQ(socketAddress->getAddress(), nullptr);

        EXPECT_EQ(
            textOf(socketAddress->getHostName()),
            "unresolved.example.invalid");

        EXPECT_EQ(
            textOf(socketAddress->getHostString()),
            "unresolved.example.invalid");
    }

    TEST(InetSocketAddressTest, CreateUnresolvedDoesNotPerformDnsLookup) {
        auto socketAddress =
            InetSocketAddress::createUnresolved(
                S("this-host-does-not-need-to-exist.invalid"),
                static_cast<jint>(80));

        ASSERT_NE(socketAddress, nullptr);
        EXPECT_TRUE(socketAddress->isUnresolved());
        EXPECT_EQ(socketAddress->getAddress(), nullptr);
    }

    TEST(InetSocketAddressTest, CreateUnresolvedRejectsNullHost) {
        jxx::Ptr<String> nullHost = nullptr;

        EXPECT_THROW(
            InetSocketAddress::createUnresolved(
                nullHost,
                static_cast<jint>(80)),
            jxx::lang::IllegalArgumentException);
    }

    TEST(InetSocketAddressTest, CreateUnresolvedRejectsNegativePort) {
        EXPECT_THROW(
            InetSocketAddress::createUnresolved(
                S("example.invalid"),
                static_cast<jint>(-1)),
            jxx::lang::IllegalArgumentException);
    }

    TEST(InetSocketAddressTest, CreateUnresolvedRejectsPortAbove65535) {
        EXPECT_THROW(
            InetSocketAddress::createUnresolved(
                S("example.invalid"),
                static_cast<jint>(65536)),
            jxx::lang::IllegalArgumentException);
    }

    /*
     * Hostname and host-string behavior
     */

    TEST(InetSocketAddressTest, UnresolvedGetHostNameReturnsOriginalHost) {
        auto socketAddress =
            InetSocketAddress::createUnresolved(
                S("device.example.invalid"),
                static_cast<jint>(1234));

        auto hostname =
            socketAddress->getHostName();

        ASSERT_NE(hostname, nullptr);
        EXPECT_EQ(
            hostname->utf8(),
            "device.example.invalid");
    }

    TEST(InetSocketAddressTest, UnresolvedGetHostStringReturnsOriginalHost) {
        auto socketAddress =
            InetSocketAddress::createUnresolved(
                S("device.example.invalid"),
                static_cast<jint>(1234));

        auto hostString =
            socketAddress->getHostString();

        ASSERT_NE(hostString, nullptr);
        EXPECT_EQ(
            hostString->utf8(),
            "device.example.invalid");
    }

    TEST(InetSocketAddressTest, ResolvedGetHostStringReturnsUsefulText) {
        auto loopback =
            InetAddress::getLoopbackAddress();

        ASSERT_NE(loopback, nullptr);

        auto socketAddress =
            jxx::NEW<InetSocketAddress>(
                loopback,
                static_cast<jint>(1234));

        auto hostString =
            socketAddress->getHostString();

        ASSERT_NE(hostString, nullptr);
        EXPECT_FALSE(hostString->isEmpty());
    }

    /*
     * Equality
     */

    TEST(InetSocketAddressTest, SameResolvedAddressAndPortAreEqual) {
        auto loopback =
            InetAddress::getLoopbackAddress();

        ASSERT_NE(loopback, nullptr);

        auto left =
            jxx::NEW<InetSocketAddress>(
                loopback,
                static_cast<jint>(8080));

        auto right =
            jxx::NEW<InetSocketAddress>(
                loopback,
                static_cast<jint>(8080));

        EXPECT_TRUE(
            left->equals(asObject(right)));

        EXPECT_TRUE(
            right->equals(asObject(left)));
    }

    TEST(InetSocketAddressTest, DifferentPortsAreNotEqual) {
        auto loopback =
            InetAddress::getLoopbackAddress();

        auto left =
            jxx::NEW<InetSocketAddress>(
                loopback,
                static_cast<jint>(8080));

        auto right =
            jxx::NEW<InetSocketAddress>(
                loopback,
                static_cast<jint>(8081));

        EXPECT_FALSE(
            left->equals(asObject(right)));
    }

    TEST(InetSocketAddressTest, SameUnresolvedHostAndPortAreEqualIgnoringCase) {
        auto left =
            InetSocketAddress::createUnresolved(
                S("DEVICE.EXAMPLE.INVALID"),
                static_cast<jint>(8080));

        auto right =
            InetSocketAddress::createUnresolved(
                S("device.example.invalid"),
                static_cast<jint>(8080));

        EXPECT_TRUE(
            left->equals(asObject(right)));

        EXPECT_TRUE(
            right->equals(asObject(left)));
    }

    TEST(InetSocketAddressTest, DifferentUnresolvedHostsAreNotEqual) {
        auto left =
            InetSocketAddress::createUnresolved(
                S("device-one.example.invalid"),
                static_cast<jint>(8080));

        auto right =
            InetSocketAddress::createUnresolved(
                S("device-two.example.invalid"),
                static_cast<jint>(8080));

        EXPECT_FALSE(
            left->equals(asObject(right)));
    }

    TEST(InetSocketAddressTest, UnresolvedAddressesWithDifferentPortsAreNotEqual) {
        auto left =
            InetSocketAddress::createUnresolved(
                S("device.example.invalid"),
                static_cast<jint>(8080));

        auto right =
            InetSocketAddress::createUnresolved(
                S("device.example.invalid"),
                static_cast<jint>(8081));

        EXPECT_FALSE(
            left->equals(asObject(right)));
    }

    TEST(InetSocketAddressTest, ResolvedAndUnresolvedAddressesAreNotEqual) {
        auto resolved =
            jxx::NEW<InetSocketAddress>(
                S("127.0.0.1"),
                static_cast<jint>(8080));

        auto unresolved =
            InetSocketAddress::createUnresolved(
                S("127.0.0.1"),
                static_cast<jint>(8080));

        EXPECT_FALSE(
            resolved->equals(asObject(unresolved)));

        EXPECT_FALSE(
            unresolved->equals(asObject(resolved)));
    }

    TEST(InetSocketAddressTest, DoesNotEqualNull) {
        auto socketAddress =
            InetSocketAddress::createUnresolved(
                S("example.invalid"),
                static_cast<jint>(8080));

        EXPECT_FALSE(
            socketAddress->equals(nullptr));
    }

    TEST(InetSocketAddressTest, DoesNotEqualDifferentObjectType) {
        auto socketAddress =
            InetSocketAddress::createUnresolved(
                S("example.invalid"),
                static_cast<jint>(8080));

        jxx::Ptr<Object> differentObject =
            jxx::CAST<Object>(S("not-an-address"));

        EXPECT_FALSE(
            socketAddress->equals(differentObject));
    }

    /*
     * hashCode
     */

    TEST(InetSocketAddressTest, EqualResolvedAddressesHaveSameHashCode) {
        auto loopback =
            InetAddress::getLoopbackAddress();

        auto left =
            jxx::NEW<InetSocketAddress>(
                loopback,
                static_cast<jint>(8080));

        auto right =
            jxx::NEW<InetSocketAddress>(
                loopback,
                static_cast<jint>(8080));

        ASSERT_TRUE(
            left->equals(asObject(right)));

        EXPECT_EQ(
            left->hashCode(),
            right->hashCode());
    }

    TEST(InetSocketAddressTest, EqualUnresolvedAddressesHaveSameHashCode) {
        auto left =
            InetSocketAddress::createUnresolved(
                S("DEVICE.EXAMPLE.INVALID"),
                static_cast<jint>(8080));

        auto right =
            InetSocketAddress::createUnresolved(
                S("device.example.invalid"),
                static_cast<jint>(8080));

        ASSERT_TRUE(
            left->equals(asObject(right)));

        EXPECT_EQ(
            left->hashCode(),
            right->hashCode());
    }

    TEST(InetSocketAddressTest, HashCodeIsStable) {
        auto socketAddress =
            InetSocketAddress::createUnresolved(
                S("device.example.invalid"),
                static_cast<jint>(8080));

        const jint first =
            socketAddress->hashCode();

        const jint second =
            socketAddress->hashCode();

        EXPECT_EQ(first, second);
    }

    /*
     * toString
     */

    TEST(InetSocketAddressTest, UnresolvedToStringContainsHostnameAndPort) {
        auto socketAddress =
            InetSocketAddress::createUnresolved(
                S("device.example.invalid"),
                static_cast<jint>(8443));

        auto result =
            socketAddress->toString();

        ASSERT_NE(result, nullptr);

        const std::string text =
            result->utf8();

        EXPECT_NE(
            text.find("device.example.invalid"),
            std::string::npos);

        EXPECT_NE(
            text.find("8443"),
            std::string::npos);
    }

    TEST(InetSocketAddressTest, ResolvedToStringContainsPort) {
        auto loopback =
            InetAddress::getLoopbackAddress();

        auto socketAddress =
            jxx::NEW<InetSocketAddress>(
                loopback,
                static_cast<jint>(4321));

        auto result =
            socketAddress->toString();

        ASSERT_NE(result, nullptr);

        EXPECT_NE(
            result->utf8().find("4321"),
            std::string::npos);
    }

} // namespace