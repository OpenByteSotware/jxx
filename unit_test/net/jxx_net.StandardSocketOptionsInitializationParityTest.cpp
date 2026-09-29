#include <gtest/gtest.h>
#include "net/jxx.net.StandardSocketOptions.h"
namespace {
TEST(StandardSocketOptionsInitializationParityTest, StaticOptionsInitializeWithoutEagerClassLookup) {
    ASSERT_NE(::jxx::net::StandardSocketOptions::SO_KEEPALIVE_, nullptr);
    EXPECT_EQ(::jxx::net::StandardSocketOptions::SO_KEEPALIVE_->name()->utf8(), "SO_KEEPALIVE");
    EXPECT_NE(::jxx::net::StandardSocketOptions::SO_KEEPALIVE_->type(), nullptr);
}
}
