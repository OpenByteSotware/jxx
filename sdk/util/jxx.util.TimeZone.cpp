#include "jxx.util.TimeZone.h"
#include <ctime>
#include <string>
namespace jxx::util {
static jint parse_gmt_offset_millis(const std::string& s) {
    if (s.size() < 4) return 0;
    std::size_t pos = 3;
    if (pos >= s.size()) return 0;
    if (s[pos] != '+' && s[pos] != '-') return 0;
    int sign = (s[pos] == '-') ? -1 : 1;
    ++pos;
    if (pos + 1 >= s.size()) return 0;
    int hh = std::stoi(s.substr(pos, 2)); pos += 2;
    if (pos < s.size() && s[pos] == ':') ++pos;
    int mm = 0;
    if (pos + 1 < s.size()) mm = std::stoi(s.substr(pos, 2));
    return (jint)(sign * ((hh * 60 + mm) * 60 * 1000));
}
static int system_offset_seconds(std::time_t epochSeconds) {
    std::tm g{}, l{};
#if defined(_WIN32)
    gmtime_s(&g, &epochSeconds);
    localtime_s(&l, &epochSeconds);
#else
    gmtime_r(&epochSeconds, &g);
    localtime_r(&epochSeconds, &l);
#endif
    std::time_t local_as_epoch = std::mktime(&l);
    std::time_t g_as_local = std::mktime(&g);
    return (int)std::difftime(local_as_epoch, g_as_local);
}
class SystemTimeZone final : public TimeZone {
public:
    SystemTimeZone() : TimeZone(std::make_shared<jxx::lang::String>("SYSTEM"), 0) {}
    jint getOffset(jlong epochMillis) const override {
        std::time_t sec = (std::time_t)(epochMillis / 1000);
        return (jint)(system_offset_seconds(sec) * 1000);
    }
};
jxx::Ptr<TimeZone> TimeZone::getDefault() {
    static jxx::Ptr<TimeZone> tz = std::make_shared<SystemTimeZone>();
    return tz;
}
jxx::Ptr<TimeZone> TimeZone::getTimeZone(jxx::Ptr<jxx::lang::String> id) {
    if (!id) return getDefault();
    std::string s = id->utf8();
    if (s == "UTC" || s == "GMT") return std::make_shared<TimeZone>(id, 0);
    if (s.rfind("GMT",0)==0 || s.rfind("UTC",0)==0) {
        jint off = parse_gmt_offset_millis(s);
        return std::make_shared<TimeZone>(id, off);
    }
    return getDefault();
}
}