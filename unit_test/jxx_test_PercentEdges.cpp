#include <gtest/gtest.h>
#include "jxx.lang.h"

using namespace jxx::lang;

TEST(PercentEdges, ScalingAndAlternateFlag) {
    Formatter f; f.setLocale("en_US.UTF-8");
    // Default scales by 100
    EXPECT_NE(std::string::npos, f.format("%p", { 0.0 }).toStdString().find("0%"));
    EXPECT_NE(std::string::npos, f.format("%p", { 1.0 }).toStdString().find("100"));
    // Alternate flag: no scaling
    EXPECT_NE(std::string::npos, f.format("%#p", { 12.5 }).toStdString().find("12.5%"));
}

TEST(PercentEdges, PrecisionAndWidthPadding) {
    Formatter f; f.setLocale("en_US.UTF-8");
    auto s1 = f.format("%.0p", { 0.126 }).toStdString();   // rounds after scaling (12.6 -> 13)
    EXPECT_NE(std::string::npos, s1.find("13%"));

    auto s2 = f.format("%08.2p", { 0.3 }).toStdString();   // width+zero pad
    // Expect something like 00030.00%
    EXPECT_TRUE(s2.find("%")!=std::string::npos);
    EXPECT_GE((int)s2.size(), 9);
}

TEST(PercentEdges, NegativeAndLargeValues) {
    Formatter f; f.setLocale("en_US.UTF-8");
    auto neg = f.format("%.1p", { -0.125 }).toStdString();
    EXPECT_NE(std::string::npos, neg.find("-12.5%"));

    auto big = f.format("%,.1p", { 12345.67 }).toStdString();
    // Around 1,234,567.0%
    EXPECT_TRUE(big.find(",")!=std::string::npos);
    EXPECT_NE(std::string::npos, big.find("%"));
}
