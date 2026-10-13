#include <gtest/gtest.h>

#include "jxx.initialize.h"

TEST(JxxCompleteClassInitialization, RootInitializationCompletes)
{
    EXPECT_NO_THROW(::jxx::initialize());
}

TEST(JxxCompleteClassInitialization, RootInitializationIsIdempotent)
{
    EXPECT_NO_THROW(::jxx::initialize());
    EXPECT_NO_THROW(::jxx::initialize());
}
