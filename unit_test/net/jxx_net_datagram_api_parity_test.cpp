#include <gtest/gtest.h>
#include "lang/jxx.lang.buildin_array.h"
#include "net/jxx.net.DatagramSocket.h"
#include "net/jxx.net.InetAddress.h"
#include "net/jxx.net.InetSocketAddress.h"
TEST(DatagramPacketApiParityTest, ArrayRangeAndSocketAddressRoundTrip)
{
	auto b = ::jxx::NEW<::jxx::lang::ByteArrayType>(4); 
	auto a = ::jxx::net::InetAddress::getLoopbackAddress(); 
	auto p = ::jxx::NEW<::jxx::net::DatagramPacket>(b, 1, 2, a, 1234); 
	EXPECT_EQ(1, p->getOffset());
	EXPECT_EQ(2, p->getLength());
	EXPECT_EQ(1234, p->getPort());
	ASSERT_NE(nullptr, p->getSocketAddress());
}
TEST(DatagramSocketApiParityTest, JavaStyleBindAndEndpointAccess)
{
	auto s = ::jxx::NEW<::jxx::net::DatagramSocket>();
	s->bind(::jxx::NEW<::jxx::net::InetSocketAddress>(0)); 
	EXPECT_TRUE(s->isBound()); EXPECT_NE(-1, s->getLocalPort()); 
	EXPECT_NE(nullptr, s->getLocalSocketAddress());
	EXPECT_EQ(nullptr, s->getChannel());
}
