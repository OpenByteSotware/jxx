#include <gtest/gtest.h>
#include "ext/net/jxx.ext.net.SocketFactory.h"
#include "ext/net/ssl/jxx.ext.net.ssl.HandshakeCompletedEvent.h"
#include "ext/net/ssl/jxx.ext.net.ssl.HandshakeCompletedListener.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocket.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocketFactory.h"
TEST(ImportSurfaceParityTest,InheritedFactoryCreateSocketIsVisible){const auto c=::jxx::ext::net::ssl::SSLContext::getInstance(::jxx::NEW<::jxx::lang::String>("TLS"));const auto f=c->getSocketFactory();const auto s=f->createSocket();ASSERT_NE(s,nullptr);}
TEST(ImportSurfaceParityTest,HandshakeSessionIsNullBeforeHandshake){const auto c=::jxx::ext::net::ssl::SSLContext::getInstance(::jxx::NEW<::jxx::lang::String>("TLS"));const auto s=::jxx::CAST<::jxx::ext::net::ssl::SSLSocket>(c->getSocketFactory()->createSocket());ASSERT_NE(s,nullptr);EXPECT_EQ(s->getHandshakeSession(),nullptr);}
