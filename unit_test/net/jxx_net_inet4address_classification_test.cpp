#include <gtest/gtest.h>
#include "lang/jxx.lang.buildin_array.h"
#include "net/jxx.net.InetAddress.h"
namespace {
::jxx::Ptr<::jxx::net::InetAddress> address4(unsigned a,unsigned b,unsigned c,unsigned d){auto x=::jxx::NEW<::jxx::lang::ByteArrayType>(4);(*x)[0]=a;(*x)[1]=b;(*x)[2]=c;(*x)[3]=d;return ::jxx::net::InetAddress::getByAddress(x);}
TEST(Inet4AddressClassificationTest, RecognizesCommonAddressClasses){EXPECT_TRUE(address4(127,0,0,1)->isLoopbackAddress());EXPECT_TRUE(address4(169,254,1,2)->isLinkLocalAddress());EXPECT_TRUE(address4(10,1,2,3)->isSiteLocalAddress());EXPECT_TRUE(address4(224,0,0,1)->isMulticastAddress());}
}
