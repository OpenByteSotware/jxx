#include "jxx.util.Date.h"

#include <ctime>

namespace jxx::util {

Date::Date() {
    std::time_t now = std::time(nullptr);
    time_ = (jlong)now * 1000;
}

Date::Date(jlong epochMillis) : time_(epochMillis) {}

jlong Date::getTime() const { return time_; }

void Date::setTime(jlong epochMillis) { time_ = epochMillis; }

} // namespace jxx::util
