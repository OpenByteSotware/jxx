#include <gtest/gtest.h>
#include "lang/jxx.lang.buildin_array.h"
#include "net/jxx.net.InetAddress.h"
namespace {
TEST(Inet6AddressClassificationTest, RecognizesLoopbackLinkLocalAndMulticast){auto loop=::jxx::NEW<::jxx::lang::ByteArrayType>(16);(*loop)[15]=1;EXPECT_TRUE(::jxx::net::InetAddress::getByAddress(loop)->isLoopbackAddress());auto link=::jxx::NEW<::jxx::lang::ByteArrayType>(16);(*link)[0]=static_cast<::jxx::lang::jbyte>(0xfe);(*link)[1]=static_cast<::jxx::lang::jbyte>(0x80);EXPECT_TRUE(::jxx::net::InetAddress::getByAddress(link)->isLinkLocalAddress());auto multi=::jxx::NEW<::jxx::lang::ByteArrayType>(16);(*multi)[0]=static_cast<::jxx::lang::jbyte>(0xff);(*multi)[1]=2;EXPECT_TRUE(::jxx::net::InetAddress::getByAddress(multi)->isMulticastAddress());}
}
