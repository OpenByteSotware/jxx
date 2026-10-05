#include <gtest/gtest.h>
#include "net/jxx.net.NetworkInterface.h"
#include "util/jxx.util.Enumeration.h"
#include "net/jxx.net.InetAddress.h"
TEST(JxxNetworkInterfaceEnumerationTest, EnumeratesInterfacesAndAddresses)
{
	const auto all = ::jxx::net::NetworkInterface::getNetworkInterfaces(); 
	ASSERT_NE(nullptr, all); ::jxx::lang::jint count = 0; while (all->hasMoreElements()) {
		const auto n = all->nextElement(); 
		ASSERT_NE(nullptr, n); ASSERT_NE(nullptr, n->getName()); 
		const auto addresses = n->getInetAddresses(); 
		ASSERT_NE(nullptr, addresses); 
		while (addresses->hasMoreElements()) {
			const auto addressElement = addresses->nextElement();
			ASSERT_NE(nullptr, addressElement);

			::jxx::net::InetAddress* address = addressElement.get();
			ASSERT_NE(nullptr, address);
			ASSERT_NE(nullptr, address->getHostAddress());
		} ++count;
	} EXPECT_GT(count, 0);
}
