#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "util/logging/jxx.util.logging.Level.h"
#include "util/logging/jxx.util.logging.LogRecord.h"

TEST(JxxLoggingClassInitialization, PublicTypesHaveClassMetadata)
{
    ::jxx::initialize();

    EXPECT_NE(::jxx::util::logging::Level::Class(), nullptr);
    EXPECT_NE(::jxx::util::logging::LogRecord::Class(), nullptr);
}
