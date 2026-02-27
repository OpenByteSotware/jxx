#include <gtest/gtest.h>
#include <unordered_set>
#include <vector>
#include <cstring>
#include "jxx.h"

// Helper to detect if IPv6 is likely available on the host for testing.
static bool ipv6_available() {
#if defined(_WIN32)
    // Minimal heuristic: on Windows, assume IPv6 is available on modern systems
    return true;
#else
    // Try to create an IPv6 socket (not a perfect test, but good heuristic)
    int s = ::socket(AF_INET6, SOCK_STREAM, 0);
    if (s >= 0) {
        ::close(s);
        return true;
    }
    return false;
#endif
}

namespace {

    using jxx::net::InetSocketAddress;

    TEST(InetSocketAddress, DefaultCtorIsUnresolved) {
        InetSocketAddress a;
        EXPECT_TRUE(a.isUnresolved());
        EXPECT_EQ(a.port(), 0);
        EXPECT_EQ(a.family(), AF_UNSPEC);
        EXPECT_EQ(a.sockaddrPtr(), nullptr);
        EXPECT_EQ(a.sockaddrLen(), 0);
        EXPECT_FALSE(a.toString().empty()); // " :0"
    }

    TEST(InetSocketAddress, CreateUnresolvedKeepsHostAndPort) {
        auto a = InetSocketAddress::createUnresolved("example.com", 443);
        EXPECT_TRUE(a.isUnresolved());
        EXPECT_EQ(a.hostString(), "example.com");
        EXPECT_EQ(a.port(), 443);
        EXPECT_EQ(a.family(), AF_UNSPEC);
        EXPECT_EQ(a.sockaddrPtr(), nullptr);
        EXPECT_EQ(a.sockaddrLen(), 0);

        // toString unresolved format: "host:port"
        EXPECT_EQ(a.toString(), "example.com:443");
    }

    TEST(InetSocketAddress, FromNumericIPv4ResolvesWithoutDNS) {
        auto a = InetSocketAddress::fromNumericIP("127.0.0.1", 8080);
        EXPECT_FALSE(a.isUnresolved());
        EXPECT_EQ(a.port(), 8080);
        EXPECT_EQ(a.family(), AF_INET);
        ASSERT_NE(a.sockaddrPtr(), nullptr);
        EXPECT_GT(a.sockaddrLen(), 0);
        EXPECT_EQ(a.ipString(), "127.0.0.1");
        EXPECT_EQ(a.toString(), "127.0.0.1:8080");
    }

    TEST(InetSocketAddress, FromNumericIPv6ResolvesWithoutDNS) {
        if (!ipv6_available()) GTEST_SKIP() << "IPv6 not available on this host";
        auto a = InetSocketAddress::fromNumericIP("::1", 8080);
        EXPECT_FALSE(a.isUnresolved());
        EXPECT_EQ(a.port(), 8080);
        EXPECT_EQ(a.family(), AF_INET6);
        ASSERT_NE(a.sockaddrPtr(), nullptr);
        EXPECT_GT(a.sockaddrLen(), 0);
        EXPECT_EQ(a.ipString(), "::1");

        // IPv6 toString uses brackets
        EXPECT_EQ(a.toString(), "[::1]:8080");
    }

    TEST(InetSocketAddress, ConstructWithHostBestEffortResolve) {
        // "localhost" should resolve on virtually all systems
        InetSocketAddress a("localhost", 9090 /*port*/, false /*deferResolve*/);
        // Allow either resolved or unresolved depending on system configuration,
        // but try to resolve explicitly after construction and check it works.
        (void)a.resolve();

        // After resolve(), most systems should succeed
        // We'll accept unresolved if system is extremely restricted, but assert fields coherently.
        EXPECT_EQ(a.port(), 9090);
        if (!a.isUnresolved()) {
            EXPECT_TRUE(a.family() == AF_INET || a.family() == AF_INET6);
            EXPECT_NE(a.sockaddrPtr(), nullptr);
            EXPECT_GT(a.sockaddrLen(), 0);
            // toString prints numeric + port; IPv6 is bracketed.
            auto s = a.toString();
            EXPECT_NE(s.find(":9090"), std::string::npos);
        }
    }

    TEST(InetSocketAddress, ResolveFailureKeepsUnresolved) {
        InetSocketAddress a("nonexistent-hostname.invalid-tld-xyz", 1234);
        // If constructor’s best-effort didn't resolve, calling resolve should also fail
        bool ok = a.resolve();
        EXPECT_FALSE(ok);
        EXPECT_TRUE(a.isUnresolved());
        EXPECT_EQ(a.hostString(), "nonexistent-hostname.invalid-tld-xyz");
        EXPECT_EQ(a.port(), 1234);
        EXPECT_EQ(a.family(), AF_UNSPEC);
        EXPECT_EQ(a.sockaddrPtr(), nullptr);
        EXPECT_EQ(a.sockaddrLen(), 0);
        EXPECT_EQ(a.toString(), "nonexistent-hostname.invalid-tld-xyz:1234");
    }

    TEST(InetSocketAddress, ParseHostPortIPv4) {
        std::string host;
        uint16_t port = 0;
        ASSERT_TRUE(InetSocketAddress::parseHostPort("example.com:443", host, port));
        EXPECT_EQ(host, "example.com");
        EXPECT_EQ(port, 443);

        ASSERT_TRUE(InetSocketAddress::parseHostPort("127.0.0.1:80", host, port));
        EXPECT_EQ(host, "127.0.0.1");
        EXPECT_EQ(port, 80);

        // invalid
        EXPECT_FALSE(InetSocketAddress::parseHostPort("noport", host, port));
        EXPECT_FALSE(InetSocketAddress::parseHostPort("too:many:colons", host, port));
        EXPECT_FALSE(InetSocketAddress::parseHostPort("host:70000", host, port)); // out of range
    }

    TEST(InetSocketAddress, ParseHostPortIPv6Bracketed) {
        std::string host;
        uint16_t port = 0;

        ASSERT_TRUE(InetSocketAddress::parseHostPort("[::1]:8080", host, port));
        EXPECT_EQ(host, "::1");
        EXPECT_EQ(port, 8080);

        ASSERT_TRUE(InetSocketAddress::parseHostPort("[2001:db8::1]:443", host, port));
        EXPECT_EQ(host, "2001:db8::1");
        EXPECT_EQ(port, 443);

        // invalid bracket usage
        EXPECT_FALSE(InetSocketAddress::parseHostPort("[::1", host, port));
        //EXPECT_FALSE(InetSocketAddress::parseHostPort("::1]:80", host, port));
        EXPECT_FALSE(InetSocketAddress::parseHostPort("[::1]:", host, port));
    }

    TEST(InetSocketAddress, EqualityAndHashUnresolved) {
        auto a = InetSocketAddress::createUnresolved("example.com", 443);
        auto b = InetSocketAddress::createUnresolved("example.com", 443);
        auto c = InetSocketAddress::createUnresolved("example.com", 80);
        auto d = InetSocketAddress::createUnresolved("EXAMPLE.com", 443); // case-sensitive comparison

        EXPECT_TRUE(a == b);
        EXPECT_FALSE(a == c);
        EXPECT_FALSE(a == d);

        std::unordered_set<InetSocketAddress, InetSocketAddress::Hasher> set;
        set.insert(a);
        EXPECT_EQ(set.count(b), 1U);
        EXPECT_EQ(set.count(c), 0U);
        EXPECT_EQ(set.count(d), 0U);
    }

    TEST(InetSocketAddress, EqualityAndHashResolved) {
        auto a = InetSocketAddress::fromNumericIP("127.0.0.1", 12345);
        auto b = InetSocketAddress::fromNumericIP("127.0.0.1", 12345);
        auto c = InetSocketAddress::fromNumericIP("127.0.0.1", 54321);
        auto d = InetSocketAddress::fromNumericIP("127.0.0.2", 12345);

        ASSERT_FALSE(a.isUnresolved());
        EXPECT_TRUE(a == b);
        EXPECT_FALSE(a == c);
        EXPECT_FALSE(a == d);

        std::unordered_set<InetSocketAddress, InetSocketAddress::Hasher> set;
        set.insert(a);
        EXPECT_EQ(set.count(b), 1U);
        EXPECT_EQ(set.count(c), 0U);
        EXPECT_EQ(set.count(d), 0U);
    }

    TEST(InetSocketAddress, SockaddrRoundTripFromSockaddr) {
        auto a = InetSocketAddress::fromNumericIP("127.0.0.1", 5555);
        ASSERT_FALSE(a.isUnresolved());
        ASSERT_NE(a.sockaddrPtr(), nullptr);

        // Create a new object from the raw sockaddr
        auto* sa = a.sockaddrPtr();
        auto  sl = a.sockaddrLen();
        auto b = InetSocketAddress::fromSockaddr(sa, sl);

        EXPECT_FALSE(b.isUnresolved());
        EXPECT_EQ(a.family(), b.family());
        EXPECT_EQ(a.port(), b.port());
        EXPECT_EQ(a.ipString(), b.ipString());
        EXPECT_EQ(a, b);
    }

    TEST(InetSocketAddress, ToStringFormatsCorrectly) {
        // unresolved
        {
            auto a = InetSocketAddress::createUnresolved("host.name", 77);
            EXPECT_EQ(a.toString(), "host.name:77");
        }
        // IPv4 resolved
        {
            auto a = InetSocketAddress::fromNumericIP("192.0.2.1", 8080);
            EXPECT_EQ(a.toString(), "192.0.2.1:8080");
        }
        // IPv6 bracketed
        if (ipv6_available()) {
            auto a = InetSocketAddress::fromNumericIP("2001:db8::1", 9090);
            EXPECT_EQ(a.toString(), "[2001:db8::1]:9090");
        }
    }

} // namespace