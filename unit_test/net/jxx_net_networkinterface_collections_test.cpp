#include <gtest/gtest.h>
#include "net/jxx.net.NetworkInterface.h"
namespace {
TEST(NetworkInterfaceCollectionsTest, CollectionAccessorsAreNeverNull) {
 const auto interfaces=::jxx::net::NetworkInterface::getNetworkInterfaces();
 ASSERT_NE(nullptr,interfaces);
 while(interfaces->hasMoreElements()) {
  const auto item=interfaces->nextElement(); ASSERT_NE(nullptr,item);
  ASSERT_NE(nullptr,item->getInetAddresses());
  ASSERT_NE(nullptr,item->getInterfaceAddresses());
  ASSERT_NE(nullptr,item->getSubInterfaces());
 }
}
}
