#include <gtest/gtest.h>

#include "util/jxx.util.SimpleTimeZone.h"
#include "util/jxx.util.GregorianCalendar.h"
#include "util/jxx.util.DecimalFormat.h"
#include "util/logging/jxx.util.logging.SimpleFormatter.h"
#include "util/regex/jxx.util.regex.Matcher.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "text/jxx.text.NumberFormat.h"
#include "text/jxx.text.DecimalFormat.h"


TEST(JxxDerivedForwardingHierarchyTest, PublicDerivedTypesHaveDistinctMetadata) {
    EXPECT_NE(::jxx::util::SimpleTimeZone::Class(), ::jxx::util::TimeZone::Class());
    EXPECT_NE(::jxx::util::GregorianCalendar::Class(), ::jxx::util::Calendar::Class());
    EXPECT_NE(::jxx::text::DecimalFormat::Class(), ::jxx::text::NumberFormat::Class());
    EXPECT_NE(::jxx::util::logging::SimpleFormatter::Class(), ::jxx::util::logging::Formatter::Class());
    EXPECT_NE(::jxx::util::regex::Matcher::Class(), nullptr);
}
