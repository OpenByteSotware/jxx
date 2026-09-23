#include <gtest/gtest.h>
#include "net/jxx.net.NetworkInterface.h"
namespace {
TEST(NetworkInterfaceInterfaceAddressTest, InterfaceAddressListContainsNoNullEntries) {
 const auto interfaces=::jxx::net::NetworkInterface::getNetworkInterfaces(); ASSERT_NE(nullptr,interfaces);
 while(interfaces->hasMoreElements()) {
  const auto list=interfaces->nextElement()->getInterfaceAddresses(); ASSERT_NE(nullptr,list);
  for(::jxx::lang::jint i=0;i<list->size();++i) EXPECT_NE(nullptr,list->get(i));
 }
}
}
