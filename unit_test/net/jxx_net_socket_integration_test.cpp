#include <gtest/gtest.h>
#include <array>
#include <atomic>
#include <thread>
#include "io/jxx.io.InputStream.h"
#include "io/jxx.io.OutputStream.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.ServerSocket.h"
#include "net/jxx.net.Socket.h"
namespace {
TEST(SocketIntegrationTest, LoopbackClientServerRoundTripAndEof) {
 const auto server=::jxx::NEW<::jxx::net::ServerSocket>(0); ASSERT_GT(server->getLocalPort(),0); std::atomic<bool> accepted{false};
 std::thread peer([&]{const auto socket=server->accept(); accepted=true; const auto in=socket->getInputStream(); const auto out=socket->getOutputStream(); auto data=std::make_shared<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(5); ASSERT_EQ(5,in->read(data,0,5)); out->write(data,0,5); out->flush(); socket->shutdownOutput(); socket->close();});
 const auto client=::jxx::NEW<::jxx::net::Socket>(::jxx::NEW<::jxx::lang::String>("127.0.0.1"),server->getLocalPort()); const auto out=client->getOutputStream(); const auto in=client->getInputStream(); auto sent=std::make_shared<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(5); const std::array<int,5> values{1,2,3,4,5}; for(int i=0;i<5;++i)(*sent)[i]=static_cast<::jxx::lang::jbyte>(values[i]); out->write(sent,0,5); out->flush(); auto received=std::make_shared<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(5); ASSERT_EQ(5,in->read(received,0,5)); for(int i=0;i<5;++i)EXPECT_EQ((*sent)[i],(*received)[i]); EXPECT_EQ(-1,in->read()); client->close(); peer.join(); server->close(); EXPECT_TRUE(accepted.load());
}
}
