#include <gtest/gtest.h>
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.Object.h"
#include "net/jxx.net.Socket.h"

TEST(JxxSocketTimeoutParityTest, NegativeTimeoutThrowsIllegalArgumentException) {
    const auto socket = ::jxx::NEW<::jxx::net::Socket>();
    EXPECT_THROW(socket->setSoTimeout(static_cast<::jxx::lang::jint>(-1)),
                 ::jxx::lang::IllegalArgumentException);
}

TEST(JxxSocketTimeoutParityTest, ZeroTimeoutIsAccepted) {
    const auto socket = ::jxx::NEW<::jxx::net::Socket>();
    EXPECT_NO_THROW(socket->setSoTimeout(static_cast<::jxx::lang::jint>(0)));
    EXPECT_EQ(static_cast<::jxx::lang::jint>(0), socket->getSoTimeout());
}
