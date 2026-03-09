#include <gtest/gtest.h>
#include <locale>
#include "jxx.lang.String.h"
#include "jxx.lang.FormatArg.h"
#include "jxx.lang.Formatter.h"

using namespace jxx::lang;

static bool localeAvailable(const char* name) {
    try { std::locale loc(name); (void)loc; return true; } catch (...) { return false; }
}
/*
TEST(StringFormatLocale, BasicNumberAndPercentLocale) {
    if (!localeAvailable("de_DE.UTF-8") && !localeAvailable("de_DE")) {
        GTEST_SKIP() << "de_DE locale not installed on this system";
    }
    String loc = localeAvailable("de_DE.UTF-8") ? String("de_DE.UTF-8") : String("de_DE");
    // Format a number and percent with German locale
    String out = String::formatLocale(loc, String("zahl=%,.2f prozent=%.1p"), { 12345.678, 0.1234 });
    std::u16string s = out.toStdString().toStdString();
    // Expect decimal comma and thousands separators (either '.' or non-breaking space depending on system)
    // Accept either "12.345,68" or "12\xC2\xA0345,68" (NBSP) as tolerant check
    bool okNumber = (s.find("12.345,68") != std::string::npos) || (s.find("12\xC2\xA0345,68") != std::string::npos);
    EXPECT_TRUE(okNumber) << s;
    // Percent should use comma decimal, and show 12,3% (scaled by 100)
    bool okPct = (s.find("12,3%") != std::string::npos) || (s.find("12,3 %") != std::string::npos);
    EXPECT_TRUE(okPct) << s;
}

TEST(StringFormatLocale, CurrencyLocale) {
    // Try FR locale first, fall back to en_US if not present
    const char* fr = localeAvailable("fr_FR.UTF-8") ? "fr_FR.UTF-8" : (localeAvailable("fr_FR")?"fr_FR":nullptr);
    const char* us = localeAvailable("en_US.UTF-8") ? "en_US.UTF-8" : (localeAvailable("en_US")?"en_US":nullptr);
    const char* use = fr ? fr : us;
    if (!use) {
        GTEST_SKIP() << "Neither fr_FR nor en_US locale is installed";
    }
    String out = String::formatLocale(String(use), String("money=%m"), { 1234.56 });
    std::string s = out.toStdString();
    // We just verify that money= appears and there is a currency symbol or grouping/decimal present
    EXPECT_NE(std::string::npos, s.find("money="));
    // Common signs to tolerate in various locales
    bool hasSym = (s.find("$")!=std::string::npos) || (s.find("€")!=std::string::npos);
    EXPECT_TRUE(hasSym || s.find(",")!=std::string::npos || s.find(" ")!=std::string::npos) << s;
}
*/