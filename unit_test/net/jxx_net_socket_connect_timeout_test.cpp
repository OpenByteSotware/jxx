#include <gtest/gtest.h>
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.ServerSocket.h"
#include "net/jxx.net.Socket.h"
namespace { TEST(SocketConnectTimeoutTest,RejectsNegativeTimeout){auto s=::jxx::NEW<::jxx::net::Socket>();auto e=::jxx::NEW<::jxx::net::InetSocketAddress>(::jxx::NEW<::jxx::lang::String>("127.0.0.1"),1);EXPECT_THROW(s->connect(e,-1),::jxx::lang::IllegalArgumentException);} TEST(SocketConnectTimeoutTest,ConnectsToLoopback){auto server=::jxx::NEW<::jxx::net::ServerSocket>(0);auto s=::jxx::NEW<::jxx::net::Socket>();auto e=::jxx::NEW<::jxx::net::InetSocketAddress>(::jxx::NEW<::jxx::lang::String>("127.0.0.1"),server->getLocalPort());s->connect(e,2000);EXPECT_TRUE(s->isConnected());auto peer=server->accept();peer->close();s->close();server->close();} }
