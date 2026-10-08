#include "util/jxx.util.Date.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace jxx::util {

Date::Date() {
    using namespace std::chrono;
    time_ = static_cast<::jxx::lang::jlong>(
        duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()).count());
}

Date::Date(::jxx::lang::jlong epochMillis)
    : time_(epochMillis) {
}

::jxx::lang::jlong Date::getTime() const {
    return time_;
}

void Date::setTime(::jxx::lang::jlong epochMillis) {
    time_ = epochMillis;
}

::jxx::lang::jbool Date::after(const ::jxx::Ptr<Date>& when) const {
    if (when == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    return time_ > when->time_;
}

::jxx::lang::jbool Date::before(const ::jxx::Ptr<Date>& when) const {
    if (when == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    return time_ < when->time_;
}

::jxx::lang::jint Date::compareTo(
    const ::jxx::Ptr<Date>& anotherDate) const {
    if (anotherDate == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    if (time_ < anotherDate->time_) return -1;
    if (time_ > anotherDate->time_) return 1;
    return 0;
}

::jxx::lang::jbool Date::equals(
    const ::jxx::Ptr<::jxx::lang::Object>& object) const {
    auto other = ::jxx::CAST<Date>(object);
    return other != nullptr && time_ == other->time_;
}

::jxx::lang::jint Date::hashCode() const {
    const auto value = static_cast<std::uint64_t>(time_);
    return static_cast<::jxx::lang::jint>(value ^ (value >> 32U));
}

::jxx::Ptr<::jxx::lang::String> Date::toString() const {
    const std::time_t seconds = static_cast<std::time_t>(time_ / 1000);
    std::tm local{};
#ifdef _WIN32
    localtime_s(&local, &seconds);
#else
    localtime_r(&seconds, &local);
#endif
    std::ostringstream stream;
    stream << std::put_time(&local, "%a %b %d %H:%M:%S %Z %Y");
    return ::jxx::NEW<::jxx::lang::String>(stream.str());
}

} // namespace jxx::util
