#include "jxx.util.Date.h"
#include <ctime>
#include <sstream>
#include <iomanip>
namespace jxx::util {
Date::Date() {
    std::time_t now = std::time(nullptr);
    time_ = (jlong)now * 1000;
}
Date::Date(jlong epochMillis) : time_(epochMillis) {}
jlong Date::getTime() const { return time_; }
void Date::setTime(jlong epochMillis) { time_ = epochMillis; }
jxx::Ptr<jxx::lang::String> Date::toString() const {
    std::time_t sec = (std::time_t)(time_ / 1000);
    std::tm tm{};
#if defined(_WIN32)
    localtime_s(&tm, &sec);
#else
    localtime_r(&sec, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%c");
    return std::make_shared<jxx::lang::String>(oss.str().c_str());
}
}