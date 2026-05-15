#include "jxx.util.Date.h"

#include <ctime>

namespace jxx::util {

Date::Date() {
    std::time_t now = std::time(nullptr);
    time_ = (jxx::lang::jlong)now * 1000;
}

Date::Date(jxx::lang::jlong epochMillis) : time_(epochMillis) {}

jxx::lang::jlong Date::getTime() const { return time_; }
void Date::setTime(jxx::lang::jlong epochMillis) { time_ = epochMillis; }

} // namespace jxx::util
