#include <gtest/gtest.h>

#include <ctime>
#include <iomanip>
#include <locale>
#include <sstream>

namespace jxx::com::sun::net::httpserver::internal {

TEST(ResponseDateConformanceTests, Rfc1123DateUsesGmtSuffix)
{
    std::tm value{};
    value.tm_year = 126;
    value.tm_mon = 8;
    value.tm_mday = 28;
    value.tm_hour = 18;
    value.tm_min = 10;
    value.tm_sec = 0;
    std::ostringstream output;
    output.imbue(std::locale::classic());
    output << std::put_time(&value, "%a, %d %b %Y %H:%M:%S GMT");
    EXPECT_NE(output.str().find("GMT"), std::string::npos);
}

} // namespace jxx::com::sun::net::httpserver::internal
