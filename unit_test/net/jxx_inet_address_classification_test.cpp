#include <gtest/gtest.h>
#include "lang/jxx.lang.buildin_array.h"
#include "net/jxx.net.InetAddress.h"
namespace { ::jxx::Ptr<::jxx::net::InetAddress> a(std::initializer_list<unsigned int> v){ auto b=::jxx::NEW<::jxx::lang::ByteArrayType>(static_cast<::jxx::lang::jint>(v.size())); ::jxx::lang::jint i=0; for(auto x:v)(*b)[i++]=static_cast<::jxx::lang::jbyte>(x); return ::jxx::net::InetAddress::getByAddress(b); } }
TEST(JxxInetAddressClassificationTest, Ipv4){ EXPECT_TRUE(a({0,0,0,0})->isAnyLocalAddress()); EXPECT_TRUE(a({127,1,2,3})->isLoopbackAddress()); EXPECT_TRUE(a({169,254,1,2})->isLinkLocalAddress()); EXPECT_TRUE(a({192,168,1,2})->isSiteLocalAddress()); EXPECT_TRUE(a({224,0,0,1})->isMCLinkLocal()); EXPECT_TRUE(a({238,1,2,3})->isMCGlobal()); EXPECT_FALSE(a({239,1,2,3})->isMCGlobal()); }
TEST(JxxInetAddressClassificationTest, Ipv6){ EXPECT_TRUE(a({0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1})->isLoopbackAddress()); EXPECT_TRUE(a({0xFE,0x80,0,0,0,0,0,0,0,0,0,0,0,0,0,1})->isLinkLocalAddress()); EXPECT_TRUE(a({0xFF,0x0E,0,0,0,0,0,0,0,0,0,0,0,0,0,1})->isMCGlobal()); }
