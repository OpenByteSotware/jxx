#include <gtest/gtest.h>
#include "util/jxx.util.Date.h"
TEST(DateGmtStringParity, IsStableEnglishAndUtc) {
    EXPECT_EQ("1 Jan 1970 00:00:00 GMT", ::jxx::NEW<::jxx::util::Date>(0)->toGMTString()->utf8());
}
