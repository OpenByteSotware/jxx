#include <gtest/gtest.h>
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InetAddress.h"
TEST(JxxInetAddressReachabilityTest, ValidationAndLoopback)
{
	const auto a = ::jxx::net::InetAddress::getByName(::jxx::NEW<::jxx::lang::String>("127.0.0.1")); 
	EXPECT_THROW(a->isReachable(-1), ::jxx::lang::IllegalArgumentException); 
	EXPECT_THROW(a->isReachable(nullptr, -1, 100), 
		::jxx::lang::IllegalArgumentException);
	EXPECT_TRUE(a->isReachable(250));
}
