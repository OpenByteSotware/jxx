#include <gtest/gtest.h>
#include "lang/jxx.lang.Throwable.h"
#include "lang/jxx.lang.String.h"

TEST(ThrowableJava8Parity, LocalizedMessageDefaultsToMessage) {
    auto message = ::jxx::NEW<::jxx::lang::String>("message");
    auto throwable = ::jxx::NEW<::jxx::lang::Throwable>(message);
    ASSERT_NE(nullptr, throwable->getLocalizedMessage());
    EXPECT_TRUE(throwable->getLocalizedMessage()->equals(message));
}
