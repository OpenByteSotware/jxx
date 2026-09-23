#include <gtest/gtest.h>
#include "net/jxx.net.NetworkInterface.h"
namespace {
TEST(NetworkInterfaceLookupTest, EnumeratedInterfacesRoundTripByNameAndIndex) {
 const auto interfaces=::jxx::net::NetworkInterface::getNetworkInterfaces(); ASSERT_NE(nullptr,interfaces);
 while(interfaces->hasMoreElements()) {
  const auto item=interfaces->nextElement(); ASSERT_NE(nullptr,item);
  if(item->getName()!=nullptr) EXPECT_NE(nullptr,::jxx::net::NetworkInterface::getByName(item->getName()));
  if(item->getIndex()>0) EXPECT_NE(nullptr,::jxx::net::NetworkInterface::getByIndex(item->getIndex()));
 }
}
}
