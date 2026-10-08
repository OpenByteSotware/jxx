#include <gtest/gtest.h>

#include "text/jxx.text.Format.h"
#include "text/jxx.text.NumberFormat.h"
#include "text/jxx.text.DateFormat.h"
#include "text/jxx.text.DecimalFormat.h"
#include "text/jxx.text.MessageFormat.h"

TEST(JxxTextFormatHierarchyTest, PublicFormattingTypesUseTextHierarchy) {
    EXPECT_TRUE(::jxx::text::Format::Class()->isAssignableFrom(
        ::jxx::text::NumberFormat::Class()));
    EXPECT_TRUE(::jxx::text::Format::Class()->isAssignableFrom(
        ::jxx::text::DateFormat::Class()));
    EXPECT_TRUE(::jxx::text::Format::Class()->isAssignableFrom(
        ::jxx::text::MessageFormat::Class()));
    EXPECT_TRUE(::jxx::text::NumberFormat::Class()->isAssignableFrom(
        ::jxx::text::DecimalFormat::Class()));
}

TEST(JxxTextFormatHierarchyTest, LegacyUtilNamesForwardWithoutDuplicateMetadata) {
    EXPECT_EQ(::jxx::text::NumberFormat::Class(),
              ::jxx::text::NumberFormat::Class());
    EXPECT_EQ(::jxx::text::DateFormat::Class(),
              ::jxx::text::DateFormat::Class());
}
