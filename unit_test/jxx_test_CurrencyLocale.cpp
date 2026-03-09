#include <gtest/gtest.h>
#include <locale>
#include "jxx.h"

using namespace jxx::lang;

static bool hasLocale(const char* name){ try { std::locale loc(name); (void)loc; return true; } catch(...) {return false;} }

TEST(CurrencyLocale, EN_US) {
    if (!hasLocale("en_US.UTF-8") && !hasLocale("en_US")) GTEST_SKIP() << "en_US locale not available";
    Formatter f; f.setLocale(hasLocale("en_US.UTF-8")?"en_US.UTF-8":"en_US");
    //auto s = f.format("%m", { 1234.56 }).toStdString();
    // Typical US formatting contains a $ and a decimal point
    //EXPECT_NE(std::string::npos, s.find("$"));
}

TEST(CurrencyLocale, DE_DE) {
    if (!hasLocale("de_DE.UTF-8") && !hasLocale("de_DE")) GTEST_SKIP() << "de_DE locale not available";
    Formatter f; f.setLocale(hasLocale("de_DE.UTF-8")?"de_DE.UTF-8":"de_DE");
   //auto s = f.format("%m", { 1234.56 }).toStdString();
    // For German we expect either trailing euro or a non-breaking space before the symbol
    //bool ok = (s.find("€")!=std::string::npos);
    //EXPECT_TRUE(ok) << s;
}
