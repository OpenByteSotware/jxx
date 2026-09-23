#include <gtest/gtest.h>
#include "net/jxx.net.NetworkInterface.h"
namespace {
TEST(NetworkInterfaceExceptionHygieneTest, EnumerationDoesNotLeakStdExceptionsOnSuccess) {
 EXPECT_NO_THROW((void)::jxx::net::NetworkInterface::getNetworkInterfaces());
}
}
