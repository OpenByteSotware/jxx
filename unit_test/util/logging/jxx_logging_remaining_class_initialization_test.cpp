#include <gtest/gtest.h>

#include "util/logging/jxx.util.logging.ClassInitialization.h"
#include "util/logging/jxx.util.logging.ErrorManager.h"
#include "util/logging/jxx.util.logging.Filter.h"
#include "util/logging/jxx.util.logging.Formatter.h"
#include "util/logging/jxx.util.logging.Handler.h"
#include "util/logging/jxx.util.logging.Logger.h"
#include "util/logging/jxx.util.logging.SimpleFormatter.h"

TEST(JxxLoggingRemainingClassInitialization, OmittedTypesHaveClassMetadata)
{
    ::jxx::util::logging::initializeClasses();

    EXPECT_NE(::jxx::util::logging::ErrorManager::Class(), nullptr);
    EXPECT_NE(::jxx::util::logging::Filter::Class(), nullptr);
    EXPECT_NE(::jxx::util::logging::Formatter::Class(), nullptr);
    EXPECT_NE(::jxx::util::logging::Handler::Class(), nullptr);
    EXPECT_NE(::jxx::util::logging::Logger::Class(), nullptr);
    EXPECT_NE(::jxx::util::logging::SimpleFormatter::Class(), nullptr);
}

TEST(JxxLoggingRemainingClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto before = ::jxx::util::logging::SimpleFormatter::Class();

    EXPECT_NO_THROW(::jxx::util::logging::initializeClasses());
    EXPECT_NO_THROW(::jxx::util::logging::initializeClasses());

    EXPECT_EQ(before, ::jxx::util::logging::SimpleFormatter::Class());
}
