#include "text/jxx.text.SimpleDateFormat.h"

#include <array>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "text/jxx.text.ParseException.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.Locale.h"
#include "util/jxx.util.TimeZone.h"

namespace jxx::text {
namespace {

std::tm localFields(::jxx::lang::jlong epochMillis,
                    ::jxx::lang::jint offsetMillis) {
    std::time_t seconds = static_cast<std::time_t>(
        (epochMillis + offsetMillis) / 1000LL);
    std::tm fields{};
#if defined(_WIN32)
    gmtime_s(&fields, &seconds);
#else
    gmtime_r(&seconds, &fields);
#endif
    return fields;
}

std::string number(int value, int width) {
    std::ostringstream stream;
    stream << std::setfill('0') << std::setw(width) << value;
    return stream.str();
}

const std::array<const char*, 12> shortMonths = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const std::array<const char*, 12> longMonths = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"};
const std::array<const char*, 7> shortDays = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const std::array<const char*, 7> longDays = {
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

} // namespace

SimpleDateFormat::SimpleDateFormat()
    : SimpleDateFormat(
          ::jxx::NEW<::jxx::lang::String>("M/d/yy h:mm a"),
          ::jxx::util::Locale::getDefault()) {}

SimpleDateFormat::SimpleDateFormat(
    const ::jxx::Ptr<::jxx::lang::String>& pattern)
    : SimpleDateFormat(pattern, ::jxx::util::Locale::getDefault()) {}

SimpleDateFormat::SimpleDateFormat(
    const ::jxx::Ptr<::jxx::lang::String>& pattern,
    const ::jxx::Ptr<::jxx::util::Locale>& locale)
    : DateFormat(pattern, locale), pattern_(pattern), locale_(locale),
      timeZone_(::jxx::util::TimeZone::getDefault()),
      twoDigitYearStart_(::jxx::NEW<::jxx::util::Date>(-631152000000LL)) {
    if (pattern_ == nullptr || locale_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    validatePattern_(pattern_->utf8());
}

void SimpleDateFormat::validatePattern_(const std::string& pattern) const {
    bool quoted = false;
    const std::string valid = "GyYMLwWDdFEu aHkKhmsSzZX";
    for (std::size_t index = 0; index < pattern.size(); ++index) {
        const char value = pattern[index];
        if (value == '\'') {
            if (index + 1 < pattern.size() && pattern[index + 1] == '\'') {
                ++index;
            } else {
                quoted = !quoted;
            }
        } else if (!quoted && ((value >= 'A' && value <= 'Z') ||
                              (value >= 'a' && value <= 'z')) &&
                   valid.find(value) == std::string::npos) {
            throw ::jxx::lang::IllegalArgumentException();
        }
    }
    if (quoted) throw ::jxx::lang::IllegalArgumentException();
}

::jxx::Ptr<::jxx::lang::String> SimpleDateFormat::format(
    const ::jxx::Ptr<::jxx::util::Date>& date) const {
    if (date == nullptr) throw ::jxx::lang::NullPointerException();
    return formatImpl_(date->getTime());
}

::jxx::Ptr<::jxx::lang::String> SimpleDateFormat::format(
    ::jxx::lang::jlong epochMillis) const {
    return formatImpl_(epochMillis);
}

::jxx::Ptr<::jxx::lang::String> SimpleDateFormat::formatImpl_(
    ::jxx::lang::jlong epochMillis) const {
    const auto offset = timeZone_ == nullptr ? 0 : timeZone_->getOffset(epochMillis);
    const auto fields = localFields(epochMillis, offset);
    const auto pattern = pattern_->utf8();
    std::string output;
    bool quoted = false;

    for (std::size_t index = 0; index < pattern.size();) {
        const char symbol = pattern[index];
        if (symbol == '\'') {
            if (index + 1 < pattern.size() && pattern[index + 1] == '\'') {
                output.push_back('\''); index += 2; continue;
            }
            quoted = !quoted; ++index; continue;
        }
        if (quoted || !((symbol >= 'A' && symbol <= 'Z') ||
                        (symbol >= 'a' && symbol <= 'z'))) {
            output.push_back(symbol); ++index; continue;
        }
        std::size_t end = index + 1;
        while (end < pattern.size() && pattern[end] == symbol) ++end;
        const int count = static_cast<int>(end - index);
        const int year = fields.tm_year + 1900;
        switch (symbol) {
        case 'G': output += year >= 1 ? "AD" : "BC"; break;
        case 'y': case 'Y':
            output += count == 2 ? number(year % 100, 2) : number(year, count); break;
        case 'M': case 'L':
            if (count >= 4) output += longMonths[fields.tm_mon];
            else if (count == 3) output += shortMonths[fields.tm_mon];
            else output += number(fields.tm_mon + 1, count); break;
        case 'd': output += number(fields.tm_mday, count); break;
        case 'D': output += number(fields.tm_yday + 1, count); break;
        case 'E': output += count >= 4 ? longDays[fields.tm_wday] : shortDays[fields.tm_wday]; break;
        case 'u': output += number(fields.tm_wday == 0 ? 7 : fields.tm_wday, count); break;
        case 'a': output += fields.tm_hour < 12 ? "AM" : "PM"; break;
        case 'H': output += number(fields.tm_hour, count); break;
        case 'k': output += number(fields.tm_hour == 0 ? 24 : fields.tm_hour, count); break;
        case 'K': output += number(fields.tm_hour % 12, count); break;
        case 'h': output += number((fields.tm_hour % 12) == 0 ? 12 : fields.tm_hour % 12, count); break;
        case 'm': output += number(fields.tm_min, count); break;
        case 's': output += number(fields.tm_sec, count); break;
        case 'S': output += number(static_cast<int>((epochMillis % 1000 + 1000) % 1000), count); break;
        case 'z':
            if (timeZone_ != nullptr) {
                const auto abbreviation = timeZone_->getAbbreviation(epochMillis);
                output += abbreviation == nullptr ? timeZone_->getID()->utf8() : abbreviation->utf8();
            }
            break;
        case 'Z': {
            const int totalMinutes = offset / 60000;
            const int absolute = totalMinutes < 0 ? -totalMinutes : totalMinutes;
            output += totalMinutes < 0 ? "-" : "+";
            output += number(absolute / 60, 2) + number(absolute % 60, 2);
            break;
        }
        case 'X': {
            const int totalMinutes = offset / 60000;
            const int absolute = totalMinutes < 0 ? -totalMinutes : totalMinutes;
            if (totalMinutes == 0) output += "Z";
            else {
                output += totalMinutes < 0 ? "-" : "+";
                output += number(absolute / 60, 2);
                if (count >= 2) output += number(absolute % 60, 2);
                if (count >= 3) output.insert(output.size() - 2, ":");
            }
            break;
        }
        default:
            output.append(static_cast<std::size_t>(count), symbol);
            break;
        }
        index = end;
    }
    return ::jxx::NEW<::jxx::lang::String>(output);
}


::jxx::Ptr<::jxx::util::Date> SimpleDateFormat::parse(
    const ::jxx::Ptr<::jxx::lang::String>& source) const {
    if (source == nullptr) throw ::jxx::lang::NullPointerException();

    const auto pattern = pattern_->utf8();
    const auto value = source->utf8();
    std::tm fields{};
    fields.tm_mday = 1;

    std::string nativePattern;
    bool quoted = false;
    for (std::size_t index = 0; index < pattern.size();) {
        const char symbol = pattern[index];
        if (symbol == '\'') {
            if (index + 1 < pattern.size() && pattern[index + 1] == '\'') {
                nativePattern.push_back('\''); index += 2; continue;
            }
            quoted = !quoted; ++index; continue;
        }
        if (quoted || !((symbol >= 'A' && symbol <= 'Z') ||
                        (symbol >= 'a' && symbol <= 'z'))) {
            nativePattern.push_back(symbol); ++index; continue;
        }
        std::size_t end = index + 1;
        while (end < pattern.size() && pattern[end] == symbol) ++end;
        const int count = static_cast<int>(end - index);
        switch (symbol) {
        case 'y': case 'Y': nativePattern += count == 2 ? "%y" : "%Y"; break;
        case 'M': case 'L': nativePattern += count >= 3 ? "%b" : "%m"; break;
        case 'd': nativePattern += "%d"; break;
        case 'D': nativePattern += "%j"; break;
        case 'E': nativePattern += "%a"; break;
        case 'H': case 'k': nativePattern += "%H"; break;
        case 'K': case 'h': nativePattern += "%I"; break;
        case 'm': nativePattern += "%M"; break;
        case 's': nativePattern += "%S"; break;
        case 'a': nativePattern += "%p"; break;
        default:
            throw ParseException(
                ::jxx::NEW<::jxx::lang::String>("Unsupported parse pattern"),
                static_cast<::jxx::lang::jint>(index));
        }
        index = end;
    }

    std::istringstream stream(value);
    stream >> std::get_time(&fields, nativePattern.c_str());
    if (stream.fail()) {
        throw ParseException(
            ::jxx::NEW<::jxx::lang::String>("Unparseable date"), 0);
    }
    stream >> std::ws;
    if (!stream.eof()) {
        throw ParseException(
            ::jxx::NEW<::jxx::lang::String>("Unparseable date"),
            static_cast<::jxx::lang::jint>(stream.tellg()));
    }

    if (!lenient_) {
        if (fields.tm_mon < 0 || fields.tm_mon > 11 ||
            fields.tm_mday < 1 || fields.tm_mday > 31 ||
            fields.tm_hour < 0 || fields.tm_hour > 23 ||
            fields.tm_min < 0 || fields.tm_min > 59 ||
            fields.tm_sec < 0 || fields.tm_sec > 59) {
            throw ParseException(
                ::jxx::NEW<::jxx::lang::String>("Unparseable date"), 0);
        }
    }

#if defined(_WIN32)
    const auto seconds = _mkgmtime(&fields);
#else
    const auto seconds = timegm(&fields);
#endif
    if (seconds == static_cast<std::time_t>(-1)) {
        throw ParseException(
            ::jxx::NEW<::jxx::lang::String>("Unparseable date"), 0);
    }
    const auto localMillis = static_cast<::jxx::lang::jlong>(seconds) * 1000LL;
    const auto offset = timeZone_ == nullptr ? 0 : timeZone_->getOffset(localMillis);
    return ::jxx::NEW<::jxx::util::Date>(localMillis - offset);
}

void SimpleDateFormat::applyPattern(
    const ::jxx::Ptr<::jxx::lang::String>& pattern) {
    if (pattern == nullptr) throw ::jxx::lang::NullPointerException();
    validatePattern_(pattern->utf8());
    pattern_ = pattern;
}
void SimpleDateFormat::applyLocalizedPattern(
    const ::jxx::Ptr<::jxx::lang::String>& pattern) { applyPattern(pattern); }
::jxx::Ptr<::jxx::lang::String> SimpleDateFormat::toPattern() const { return pattern_; }
::jxx::Ptr<::jxx::lang::String> SimpleDateFormat::toLocalizedPattern() const { return pattern_; }
void SimpleDateFormat::setTimeZone(const ::jxx::Ptr<::jxx::util::TimeZone>& zone) {
    if (zone == nullptr) throw ::jxx::lang::NullPointerException();
    timeZone_ = zone;
}
::jxx::Ptr<::jxx::util::TimeZone> SimpleDateFormat::getTimeZone() const { return timeZone_; }
void SimpleDateFormat::setLenient(::jxx::lang::jbool value) noexcept { lenient_ = value; }
::jxx::lang::jbool SimpleDateFormat::isLenient() const noexcept { return lenient_; }
void SimpleDateFormat::set2DigitYearStart(const ::jxx::Ptr<::jxx::util::Date>& date) {
    if (date == nullptr) throw ::jxx::lang::NullPointerException();
    twoDigitYearStart_ = ::jxx::NEW<::jxx::util::Date>(date->getTime());
}
::jxx::Ptr<::jxx::util::Date> SimpleDateFormat::get2DigitYearStart() const {
    return ::jxx::NEW<::jxx::util::Date>(twoDigitYearStart_->getTime());
}
::jxx::lang::jbool SimpleDateFormat::equals(
    const ::jxx::Ptr<::jxx::lang::Object>& other) const {
    const auto value = ::jxx::CAST<SimpleDateFormat>(other);
    return value != nullptr && pattern_->equals(value->pattern_) &&
           lenient_ == value->lenient_ &&
           timeZone_->getID()->equals(value->timeZone_->getID());
}
::jxx::lang::jint SimpleDateFormat::hashCode() const { return pattern_->hashCode(); }
::jxx::Ptr<::jxx::lang::Object> SimpleDateFormat::cloneImpl() const {
    auto copy = ::jxx::NEW<SimpleDateFormat>(pattern_, locale_);
    copy->timeZone_ = timeZone_;
    copy->lenient_ = lenient_;
    copy->twoDigitYearStart_ = get2DigitYearStart();
    return ::jxx::CAST<::jxx::lang::Object>(copy);
}

} // namespace jxx::text
