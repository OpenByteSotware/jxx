#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "text/jxx.text.ClassInitialization.h"
#include "text/jxx.text.DateFormat.h"
#include "text/jxx.text.DecimalFormat.h"
#include "text/jxx.text.Format.h"
#include "text/jxx.text.MessageFormat.h"
#include "text/jxx.text.NumberFormat.h"

TEST(JxxTextClassInitialization, PublicTypesHaveClassMetadata)
{
    ::jxx::initialize();

    EXPECT_NE(::jxx::text::DateFormat::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::text::DecimalFormat::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::text::Format::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::text::MessageFormat::JxxClassInfoMarker::Class(), nullptr);
    EXPECT_NE(::jxx::text::NumberFormat::JxxClassInfoMarker::Class(), nullptr);
}

TEST(JxxTextClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto formatBefore =
        ::jxx::text::Format::JxxClassInfoMarker::Class();
    const auto numberFormatBefore =
        ::jxx::text::NumberFormat::JxxClassInfoMarker::Class();

    EXPECT_NO_THROW(::jxx::text::initializeClasses());
    EXPECT_NO_THROW(::jxx::text::initializeClasses());

    EXPECT_EQ(
        formatBefore,
        ::jxx::text::Format::JxxClassInfoMarker::Class());
    EXPECT_EQ(
        numberFormatBefore,
        ::jxx::text::NumberFormat::JxxClassInfoMarker::Class());
}
