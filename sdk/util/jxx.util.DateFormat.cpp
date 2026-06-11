#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include "util/jxx.util.Locale.h"
#include "util/jxx.util.TimeZone.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.StringPool.h"

#include "util/jxx.util.DateFormat.h"

namespace jxx::util {
namespace {
    std::tm toUtcTm(jxx::lang::jlong epochMillis, jxx::lang::jint totalOffsetMillis) {
        std::time_t seconds = static_cast<std::time_t>((epochMillis + totalOffsetMillis) / 1000LL);
        std::tm out{};
    #if defined(_WIN32)
        gmtime_s(&out, &seconds);
    #else
        gmtime_r(&seconds, &out);
    #endif
        return out;
    }

    std::string pad2(int value) {
        std::ostringstream os;
        os << std::setfill('0') << std::setw(2) << value;
        return os.str();
    }
    std::string pad4(int value) {
        std::ostringstream os;
        os << std::setfill('0') << std::setw(4) << value;
        return os.str();
    }
}

DateFormat::DateFormat(const jxx::Ptr<jxx::lang::String>& pattern, const jxx::Ptr<Locale>& locale)
    : pattern_(pattern ? pattern : StringPool::intern("yyyy-MM-dd HH:mm:ss")),
      locale_(locale ? locale : Locale::getDefault()) {}

jxx::Ptr<DateFormat> DateFormat::ofPattern(const jxx::Ptr<jxx::lang::String>& pattern,
                                           const jxx::Ptr<Locale>& locale) {
    return jxx::NEW<DateFormat>(pattern, locale);
}

jxx::Ptr<jxx::lang::String> DateFormat::format(jxx::lang::jlong epochMillis,
                                               const jxx::Ptr<TimeZone>& timeZone) const {
    jxx::lang::jint totalOffsetMillis = timeZone ? timeZone->getOffset(epochMillis) : 0;
    std::tm tm = toUtcTm(epochMillis, totalOffsetMillis);
    std::string pattern = pattern_ ? pattern_->utf8() : std::string("yyyy-MM-dd HH:mm:ss");
    std::string out;

    for (std::size_t i = 0; i < pattern.size();) {
        if (pattern.compare(i, 4, "yyyy") == 0) { out += pad4(tm.tm_year + 1900); i += 4; continue; }
        if (pattern.compare(i, 2, "MM") == 0)   { out += pad2(tm.tm_mon + 1); i += 2; continue; }
        if (pattern.compare(i, 2, "dd") == 0)   { out += pad2(tm.tm_mday); i += 2; continue; }
        if (pattern.compare(i, 2, "HH") == 0)   { out += pad2(tm.tm_hour); i += 2; continue; }
        if (pattern.compare(i, 2, "mm") == 0)   { out += pad2(tm.tm_min); i += 2; continue; }
        if (pattern.compare(i, 2, "ss") == 0)   { out += pad2(tm.tm_sec); i += 2; continue; }
        if (pattern.compare(i, 1, "z") == 0 && timeZone) {
            auto abbr = timeZone->getAbbreviation(epochMillis);
            out += abbr ? abbr->utf8() : timeZone->getID()->utf8();
            i += 1;
            continue;
        }
        out.push_back(pattern[i]);
        ++i;
    }

    return jxx::lang::String::valueOf(out.c_str());
}

} // namespace jxx::util
