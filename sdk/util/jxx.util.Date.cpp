#include "util/jxx.util.Date.h"

#include <array>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "util/jxx.util.TimeZone.h"

namespace jxx::util {
namespace {

std::int64_t floorDivide(std::int64_t value, std::int64_t divisor) {
    std::int64_t quotient = value / divisor;
    const std::int64_t remainder = value % divisor;
    if (remainder != 0 && ((remainder < 0) != (divisor < 0))) --quotient;
    return quotient;
}

std::int64_t daysFromCivil(int year, unsigned month, unsigned day) {
    year -= month <= 2U ? 1 : 0;
    const int era = (year >= 0 ? year : year - 399) / 400;
    const unsigned yearOfEra = static_cast<unsigned>(year - era * 400);
    const unsigned dayOfYear =
        (153U * (month > 2U ? month - 3U : month + 9U) + 2U) / 5U +
        day - 1U;
    const unsigned dayOfEra = yearOfEra * 365U + yearOfEra / 4U -
        yearOfEra / 100U + dayOfYear;
    return static_cast<std::int64_t>(era) * 146097LL +
        static_cast<std::int64_t>(dayOfEra) - 719468LL;
}

std::tm localFields(::jxx::lang::jlong millis) {
    const auto zone = TimeZone::getDefault();
    const auto localMillisValue = millis + zone->getOffset(millis);
    const std::time_t seconds = static_cast<std::time_t>(localMillisValue / 1000LL);
    std::tm fields{};
#if defined(_WIN32)
    gmtime_s(&fields, &seconds);
#else
    gmtime_r(&seconds, &fields);
#endif
    return fields;
}

std::tm utcFields(::jxx::lang::jlong millis) {
    const std::time_t seconds = static_cast<std::time_t>(millis / 1000LL);
    std::tm fields{};
#if defined(_WIN32)
    gmtime_s(&fields, &seconds);
#else
    gmtime_r(&seconds, &fields);
#endif
    return fields;
}

std::time_t toUtc(std::tm* fields) {
#if defined(_WIN32)
    return _mkgmtime(fields);
#else
    return timegm(fields);
#endif
}

::jxx::lang::jlong localMillis(std::tm fields) {
    fields.tm_isdst = 0;
    const auto wallMillis =
        static_cast<::jxx::lang::jlong>(toUtc(&fields)) * 1000LL;
    const auto zone = TimeZone::getDefault();
    auto epochMillis = wallMillis - zone->getRawOffset();
    for (int iteration = 0; iteration < 4; ++iteration) {
        const auto adjusted = wallMillis - zone->getOffset(epochMillis);
        if (adjusted == epochMillis) break;
        epochMillis = adjusted;
    }
    return epochMillis;
}

::jxx::Ptr<::jxx::lang::String> formatted(
    const std::tm& fields, const char* pattern) {
    std::ostringstream stream;
    stream << std::put_time(&fields, pattern);
    return ::jxx::NEW<::jxx::lang::String>(stream.str());
}

bool parseWith(const std::string& input, const char* pattern,
               std::tm& fields) {
    fields = {};
    fields.tm_mday = 1;
    std::istringstream stream(input);
    stream >> std::get_time(&fields, pattern);
    if (stream.fail()) return false;
    stream >> std::ws;
    return stream.eof();
}

} // namespace

Date::Date() {
    using namespace std::chrono;
    time_ = static_cast<::jxx::lang::jlong>(
        duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()).count());
}

Date::Date(::jxx::lang::jlong epochMillis) : time_(epochMillis) {}

Date::Date(::jxx::lang::jint year, ::jxx::lang::jint month,
           ::jxx::lang::jint date)
    : Date(year, month, date, 0, 0, 0) {}

Date::Date(::jxx::lang::jint year, ::jxx::lang::jint month,
           ::jxx::lang::jint date, ::jxx::lang::jint hours,
           ::jxx::lang::jint minutes)
    : Date(year, month, date, hours, minutes, 0) {}

Date::Date(::jxx::lang::jint year, ::jxx::lang::jint month,
           ::jxx::lang::jint date, ::jxx::lang::jint hours,
           ::jxx::lang::jint minutes, ::jxx::lang::jint seconds) {
    std::tm fields{};
    fields.tm_year = year;
    fields.tm_mon = month;
    fields.tm_mday = date;
    fields.tm_hour = hours;
    fields.tm_min = minutes;
    fields.tm_sec = seconds;
    time_ = localMillis(fields);
}

Date::Date(const ::jxx::Ptr<::jxx::lang::String>& value)
    : time_(parse(value)) {}

::jxx::lang::jlong Date::UTC(
    ::jxx::lang::jint year, ::jxx::lang::jint month,
    ::jxx::lang::jint date, ::jxx::lang::jint hours,
    ::jxx::lang::jint minutes, ::jxx::lang::jint seconds) {
    const std::int64_t absoluteMonth =
        static_cast<std::int64_t>(year) * 12LL + month;
    const std::int64_t normalizedYearOffset =
        floorDivide(absoluteMonth, 12LL);
    const int normalizedMonth = static_cast<int>(
        absoluteMonth - normalizedYearOffset * 12LL);
    const int normalizedYear =
        static_cast<int>(normalizedYearOffset + 1900LL);

    const std::int64_t dayMillis =
        (daysFromCivil(normalizedYear,
                       static_cast<unsigned>(normalizedMonth + 1), 1U) +
         static_cast<std::int64_t>(date) - 1LL) * 86400000LL;
    const std::int64_t timeMillis =
        static_cast<std::int64_t>(hours) * 3600000LL +
        static_cast<std::int64_t>(minutes) * 60000LL +
        static_cast<std::int64_t>(seconds) * 1000LL;
    return static_cast<::jxx::lang::jlong>(dayMillis + timeMillis);
}

::jxx::lang::jlong Date::parse(
    const ::jxx::Ptr<::jxx::lang::String>& value) {
    if (value == nullptr) throw ::jxx::lang::NullPointerException();
    const auto input = value->utf8();
    const std::array<const char*, 8> patterns = {
        "%a, %d %b %Y %H:%M:%S GMT",
        "%a %b %d %H:%M:%S %Z %Y",
        "%d %b %Y %H:%M:%S GMT",
        "%b %d, %Y %H:%M:%S",
        "%m/%d/%Y %H:%M:%S",
        "%m/%d/%Y", "%Y-%m-%d %H:%M:%S", "%Y-%m-%d"};
    std::tm fields{};
    for (const auto* pattern : patterns) {
        if (!parseWith(input, pattern, fields)) continue;
        const std::string format(pattern);
        const bool utc = format.find("GMT") != std::string::npos ||
                         input.find("GMT") != std::string::npos ||
                         input.find("UTC") != std::string::npos;
        if (utc) {
            const auto seconds = toUtc(&fields);
            return static_cast<::jxx::lang::jlong>(seconds) * 1000LL;
        }
        return localMillis(fields);
    }
    throw ::jxx::lang::IllegalArgumentException();
}

::jxx::lang::jlong Date::getTime() const { return time_; }
void Date::setTime(::jxx::lang::jlong value) { time_ = value; }

::jxx::lang::jint Date::getYear() const { return localFields(time_).tm_year; }
void Date::setYear(::jxx::lang::jint value) {
    auto fields = localFields(time_); fields.tm_year = value; time_ = localMillis(fields);
}
::jxx::lang::jint Date::getMonth() const { return localFields(time_).tm_mon; }
void Date::setMonth(::jxx::lang::jint value) {
    auto fields = localFields(time_); fields.tm_mon = value; time_ = localMillis(fields);
}
::jxx::lang::jint Date::getDate() const { return localFields(time_).tm_mday; }
void Date::setDate(::jxx::lang::jint value) {
    auto fields = localFields(time_); fields.tm_mday = value; time_ = localMillis(fields);
}
::jxx::lang::jint Date::getDay() const { return localFields(time_).tm_wday; }
::jxx::lang::jint Date::getHours() const { return localFields(time_).tm_hour; }
void Date::setHours(::jxx::lang::jint value) {
    auto fields = localFields(time_); fields.tm_hour = value; time_ = localMillis(fields);
}
::jxx::lang::jint Date::getMinutes() const { return localFields(time_).tm_min; }
void Date::setMinutes(::jxx::lang::jint value) {
    auto fields = localFields(time_); fields.tm_min = value; time_ = localMillis(fields);
}
::jxx::lang::jint Date::getSeconds() const { return localFields(time_).tm_sec; }
void Date::setSeconds(::jxx::lang::jint value) {
    auto fields = localFields(time_); fields.tm_sec = value; time_ = localMillis(fields);
}

::jxx::Ptr<::jxx::lang::String> Date::toLocaleString() const {
    const auto fields = localFields(time_);
    return formatted(fields, "%x %X");
}

::jxx::Ptr<::jxx::lang::String> Date::toGMTString() const {
    static const std::array<const char*, 12> months{
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    const auto fields = utcFields(time_);
    std::ostringstream stream;
    stream << fields.tm_mday << ' '
           << months[static_cast<std::size_t>(fields.tm_mon)] << ' '
           << fields.tm_year + 1900 << ' '
           << std::setw(2) << std::setfill('0') << fields.tm_hour << ':'
           << std::setw(2) << fields.tm_min << ':'
           << std::setw(2) << fields.tm_sec << " GMT";
    return ::jxx::NEW<::jxx::lang::String>(stream.str());
}

::jxx::lang::jint Date::getTimezoneOffset() const {
    return -TimeZone::getDefault()->getOffset(time_) / 60000;
}

::jxx::lang::jbool Date::after(const ::jxx::Ptr<Date>& when) const {
    if (when == nullptr) throw ::jxx::lang::NullPointerException();
    return time_ > when->time_;
}
::jxx::lang::jbool Date::before(const ::jxx::Ptr<Date>& when) const {
    if (when == nullptr) throw ::jxx::lang::NullPointerException();
    return time_ < when->time_;
}
::jxx::lang::jint Date::compareTo(const ::jxx::Ptr<Date>& other) const {
    if (other == nullptr) throw ::jxx::lang::NullPointerException();
    return time_ < other->time_ ? -1 : time_ > other->time_ ? 1 : 0;
}
::jxx::lang::jbool Date::equals(
    const ::jxx::Ptr<::jxx::lang::Object>& object) const {
    const auto other = ::jxx::CAST<Date>(object);
    return other != nullptr && time_ == other->time_;
}
::jxx::lang::jint Date::hashCode() const {
    const auto value = static_cast<std::uint64_t>(time_);
    return static_cast<::jxx::lang::jint>(value ^ (value >> 32U));
}
::jxx::Ptr<::jxx::lang::String> Date::toString() const {
    static const std::array<const char*, 7> weekDays{
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    static const std::array<const char*, 12> months{
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    const auto fields = localFields(time_);
    const auto abbreviation = TimeZone::getDefault()->getAbbreviation(time_);
    std::ostringstream stream;
    stream << weekDays[static_cast<std::size_t>(fields.tm_wday)] << ' '
           << months[static_cast<std::size_t>(fields.tm_mon)] << ' '
           << std::setw(2) << std::setfill('0') << fields.tm_mday << ' '
           << std::setw(2) << fields.tm_hour << ':'
           << std::setw(2) << fields.tm_min << ':'
           << std::setw(2) << fields.tm_sec << ' '
           << (abbreviation == nullptr ? TimeZone::getDefault()->getID()->utf8()
                                       : abbreviation->utf8()) << ' '
           << fields.tm_year + 1900;
    return ::jxx::NEW<::jxx::lang::String>(stream.str());
}

} // namespace jxx::util
