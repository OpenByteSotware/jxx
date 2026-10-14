#include "text/jxx.text.SimpleDateFormat.h"

#include <array>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <regex>
#include <tuple>
#include <sstream>
#include <string>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "text/jxx.text.ParseException.h"
#include "text/jxx.text.DateFormatSymbols.h"
#include "text/jxx.text.ParsePosition.h"
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

bool leapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

int isoWeeksInYear(int year) {
    std::tm januaryFirst{};
    januaryFirst.tm_year = year - 1900;
    januaryFirst.tm_mon = 0;
    januaryFirst.tm_mday = 1;
#if defined(_WIN32)
    const auto instant = _mkgmtime(&januaryFirst);
    gmtime_s(&januaryFirst, &instant);
#else
    const auto instant = timegm(&januaryFirst);
    gmtime_r(&instant, &januaryFirst);
#endif
    const int isoDay = januaryFirst.tm_wday == 0 ? 7 : januaryFirst.tm_wday;
    return isoDay == 4 || (isoDay == 3 && leapYear(year)) ? 53 : 52;
}

std::tuple<int, int, int> isoWeekFields(const std::tm& fields) {
    const int year = fields.tm_year + 1900;
    const int isoDay = fields.tm_wday == 0 ? 7 : fields.tm_wday;
    int week = (fields.tm_yday + 11 - isoDay) / 7;
    int weekYear = year;
    if (week < 1) {
        weekYear = year - 1;
        week = isoWeeksInYear(weekYear);
    } else if (week > isoWeeksInYear(year)) {
        weekYear = year + 1;
        week = 1;
    }
    return {weekYear, week, isoDay};
}

bool parseNumericOffset(const std::string& text, int& minutes) {
    if (text == "Z" || text == "UTC" || text == "GMT") { minutes = 0; return true; }
    std::smatch match;
    static const std::regex offsetPattern("^(?:GMT)?([+-])(\\d{2})(?::?(\\d{2}))?$");
    if (!std::regex_match(text, match, offsetPattern)) return false;
    const int hours = std::stoi(match[2].str());
    const int mins = match[3].matched ? std::stoi(match[3].str()) : 0;
    if (hours > 23 || mins > 59) return false;
    minutes = hours * 60 + mins;
    if (match[1].str() == "-") minutes = -minutes;
    return true;
}

std::string lowerAscii(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
        [](unsigned char character) { return static_cast<char>(std::tolower(character)); });
    return value;
}

void replaceSymbol(std::string& value, const std::string& custom,
                   const std::string& standard) {
    if (custom.empty()) return;
    const auto loweredCustom = lowerAscii(custom);
    auto loweredValue = lowerAscii(value);
    std::size_t position = 0;
    while ((position = loweredValue.find(loweredCustom, position)) != std::string::npos) {
        value.replace(position, custom.size(), standard);
        loweredValue.replace(position, custom.size(), lowerAscii(standard));
        position += standard.size();
    }
}

void normalizeCustomSymbols(std::string& value,
                            const ::jxx::Ptr<DateFormatSymbols>& symbols) {
    const auto months = symbols->getMonths();
    const auto shortMonthValues = symbols->getShortMonths();
    for (::jxx::lang::jint index = 0; index < 12; ++index) {
        replaceSymbol(value, (*months)[index]->utf8(), shortMonths[index]);
        replaceSymbol(value, (*shortMonthValues)[index]->utf8(), shortMonths[index]);
    }
    const auto weekdays = symbols->getWeekdays();
    const auto shortWeekdayValues = symbols->getShortWeekdays();
    for (::jxx::lang::jint index = 1; index <= 7; ++index) {
        replaceSymbol(value, (*weekdays)[index]->utf8(), shortDays[index - 1]);
        replaceSymbol(value, (*shortWeekdayValues)[index]->utf8(), shortDays[index - 1]);
    }
    const auto amPm = symbols->getAmPmStrings();
    replaceSymbol(value, (*amPm)[0]->utf8(), "AM");
    replaceSymbol(value, (*amPm)[1]->utf8(), "PM");
}

std::string translatePatternLetters(
    const std::string& pattern,
    const std::string& from,
    const std::string& to) {
    if (from.size() != to.size()) {
        throw ::jxx::lang::IllegalArgumentException();
    }
    std::string translated;
    translated.reserve(pattern.size());
    bool quoted = false;
    for (std::size_t index = 0; index < pattern.size(); ++index) {
        const char value = pattern[index];
        if (value == '\'') {
            translated.push_back(value);
            if (index + 1 < pattern.size() && pattern[index + 1] == '\'') {
                translated.push_back(pattern[++index]);
            } else {
                quoted = !quoted;
            }
            continue;
        }
        if (!quoted) {
            const auto position = from.find(value);
            if (position != std::string::npos) {
                translated.push_back(to[position]);
                continue;
            }
        }
        translated.push_back(value);
    }
    return translated;
}

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
    symbols_ = ::jxx::NEW<DateFormatSymbols>(locale_);
    validatePattern_(pattern_->utf8());
}

SimpleDateFormat::SimpleDateFormat(
    const ::jxx::Ptr<::jxx::lang::String>& pattern,
    const ::jxx::Ptr<DateFormatSymbols>& symbols)
    : SimpleDateFormat(pattern, ::jxx::util::Locale::getDefault()) {
    setDateFormatSymbols(symbols);
}

::jxx::Ptr<DateFormatSymbols> SimpleDateFormat::getDateFormatSymbols() const {
    return ::jxx::CAST<DateFormatSymbols>(symbols_->clone());
}
void SimpleDateFormat::setDateFormatSymbols(const ::jxx::Ptr<DateFormatSymbols>& symbols) {
    if (symbols == nullptr) throw ::jxx::lang::NullPointerException();
    symbols_ = ::jxx::CAST<DateFormatSymbols>(symbols->clone());
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
        case 'y':
            output += count == 2 ? number(year % 100, 2) : number(year, count); break;
        case 'Y': {
            const auto weekYear = std::get<0>(isoWeekFields(fields));
            output += count == 2 ? number(weekYear % 100, 2) : number(weekYear, count);
            break;
        }
        case 'M': case 'L':
            if (count >= 4) output += (*symbols_->getMonths())[fields.tm_mon]->utf8();
            else if (count == 3) output += (*symbols_->getShortMonths())[fields.tm_mon]->utf8();
            else output += number(fields.tm_mon + 1, count); break;
        case 'd': output += number(fields.tm_mday, count); break;
        case 'D': output += number(fields.tm_yday + 1, count); break;
        case 'w': output += number(std::get<1>(isoWeekFields(fields)), count); break;
        case 'W': output += number((fields.tm_mday - 1) / 7 + 1, count); break;
        case 'F': output += number((fields.tm_mday - 1) / 7 + 1, count); break;
        case 'E': output += count >= 4 ? (*symbols_->getWeekdays())[fields.tm_wday + 1]->utf8() : (*symbols_->getShortWeekdays())[fields.tm_wday + 1]->utf8(); break;
        case 'u': output += number(fields.tm_wday == 0 ? 7 : fields.tm_wday, count); break;
        case 'a': output += (*symbols_->getAmPmStrings())[fields.tm_hour < 12 ? 0 : 1]->utf8(); break;
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

    std::string pattern = pattern_->utf8();
    std::string value = source->utf8();
    int parsedOffsetMinutes = 0;
    bool hasParsedOffset = false;
    std::smatch zoneMatch;
    static const std::regex terminalZone(
        "^(.*?)(?:\\s*)(Z|UTC|GMT|GMT[+-]\\d{2}(?::?\\d{2})?|[+-]\\d{2}(?::?\\d{2})?)$");
    if (std::regex_match(value, zoneMatch, terminalZone)) {
        const std::string zoneText = zoneMatch[2].str();
        std::size_t last = pattern.find_last_not_of(" ");
        if (last != std::string::npos &&
            (pattern[last] == 'z' || pattern[last] == 'Z' || pattern[last] == 'X')) {
            const char zoneSymbol = pattern[last];
            while (last > 0 && pattern[last - 1] == zoneSymbol) --last;
            pattern.erase(last);
            while (!pattern.empty() && pattern.back() == ' ') pattern.pop_back();
            value = zoneMatch[1].str();
            while (!value.empty() && value.back() == ' ') value.pop_back();
            if (!parseNumericOffset(zoneText, parsedOffsetMinutes)) {
                throw ParseException(::jxx::NEW<::jxx::lang::String>("Unparseable time zone"),
                                     static_cast<::jxx::lang::jint>(zoneMatch.position(2)));
            }
            hasParsedOffset = true;
        }
    }
    normalizeCustomSymbols(value, symbols_);

    int parsedMilliseconds = 0;
    bool hasParsedMilliseconds = false;
    std::size_t millisecondPatternEnd = pattern.find_last_not_of(" ");
    if (millisecondPatternEnd != std::string::npos) {
        // When a zone field is terminal, the earlier zone extraction has already
        // removed it. Therefore a millisecond field immediately before that zone
        // is terminal here as well.
        if (pattern[millisecondPatternEnd] == 'S') {
            std::size_t millisecondPatternStart = millisecondPatternEnd;
            while (millisecondPatternStart > 0 &&
                   pattern[millisecondPatternStart - 1] == 'S') {
                --millisecondPatternStart;
            }
            std::size_t valueEnd = value.find_last_not_of(" ");
            if (valueEnd == std::string::npos ||
                !std::isdigit(static_cast<unsigned char>(value[valueEnd]))) {
                throw ParseException(
                    ::jxx::NEW<::jxx::lang::String>("Unparseable milliseconds"),
                    static_cast<::jxx::lang::jint>(value.size()));
            }
            std::size_t valueStart = valueEnd;
            while (valueStart > 0 &&
                   std::isdigit(static_cast<unsigned char>(value[valueStart - 1]))) {
                --valueStart;
            }
            const auto digits = value.substr(valueStart, valueEnd - valueStart + 1);
            try {
                parsedMilliseconds = std::stoi(digits);
            } catch (...) {
                throw ParseException(
                    ::jxx::NEW<::jxx::lang::String>("Unparseable milliseconds"),
                    static_cast<::jxx::lang::jint>(valueStart));
            }
            if (parsedMilliseconds < 0 || parsedMilliseconds > 999) {
                throw ParseException(
                    ::jxx::NEW<::jxx::lang::String>("Unparseable milliseconds"),
                    static_cast<::jxx::lang::jint>(valueStart));
            }
            hasParsedMilliseconds = true;
            pattern.erase(millisecondPatternStart);
            while (!pattern.empty() && pattern.back() == ' ') pattern.pop_back();
            if (!pattern.empty() && !value.empty() &&
                pattern.back() == value[valueStart - (valueStart > 0 ? 1 : 0)] &&
                valueStart > 0) {
                pattern.pop_back();
                --valueStart;
            }
            value.erase(valueStart);
            while (!value.empty() && value.back() == ' ') value.pop_back();
        }
    }

    int parsedAmPm = -1;
    std::size_t last = pattern.find_last_not_of(" ");
    if (last != std::string::npos && pattern[last] == 'a') {
        const auto loweredValue = lowerAscii(value);
        const auto markerStart = loweredValue.size() >= 2
            ? loweredValue.size() - 2
            : std::string::npos;
        if (markerStart != std::string::npos && markerStart > 0 &&
            std::isspace(static_cast<unsigned char>(value[markerStart - 1])) &&
            (loweredValue.compare(markerStart, 2, "am") == 0 ||
             loweredValue.compare(markerStart, 2, "pm") == 0)) {
            parsedAmPm = loweredValue.compare(markerStart, 2, "pm") == 0 ? 1 : 0;
            while (last > 0 && pattern[last - 1] == 'a') --last;
            pattern.erase(last);
            while (!pattern.empty() && pattern.back() == ' ') pattern.pop_back();
            value.erase(markerStart);
            while (!value.empty() &&
                   std::isspace(static_cast<unsigned char>(value.back()))) {
                value.pop_back();
            }
        }
    }
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
    if (stream.peek() != std::char_traits<char>::eof()) {
        const auto position = stream.tellg();
        throw ParseException(
            ::jxx::NEW<::jxx::lang::String>("Unparseable date"),
            position == std::streampos(-1)
                ? 0
                : static_cast<::jxx::lang::jint>(position));
    }

    if (parsedAmPm >= 0) {
        if (fields.tm_hour < 0 || fields.tm_hour > 12) {
            throw ParseException(
                ::jxx::NEW<::jxx::lang::String>("Unparseable date"), 0);
        }
        if (parsedAmPm == 1 && fields.tm_hour < 12) fields.tm_hour += 12;
        if (parsedAmPm == 0 && fields.tm_hour == 12) fields.tm_hour = 0;
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
    const auto offset = hasParsedOffset
        ? parsedOffsetMinutes * 60000
        : (timeZone_ == nullptr ? 0 : timeZone_->getOffset(localMillis));
    const auto resultMillis = localMillis - offset +
        (hasParsedMilliseconds ? parsedMilliseconds : 0);
    return ::jxx::NEW<::jxx::util::Date>(resultMillis);
}

::jxx::Ptr<::jxx::util::Date> SimpleDateFormat::parse(
    const ::jxx::Ptr<::jxx::lang::String>& source,
    const ::jxx::Ptr<ParsePosition>& position) const {
    if (source == nullptr || position == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    const auto text = source->utf8();
    const auto start = position->getIndex();
    if (start < 0 || static_cast<std::size_t>(start) > text.size()) {
        position->setErrorIndex(start);
        return nullptr;
    }

    // The String-only overload requires complete consumption. Try prefixes
    // longest-first to reproduce ParsePosition's partial-consumption contract.
    for (std::size_t end = text.size(); end > static_cast<std::size_t>(start); --end) {
        try {
            const auto candidate = ::jxx::NEW<::jxx::lang::String>(
                text.substr(static_cast<std::size_t>(start),
                            end - static_cast<std::size_t>(start)));
            const auto result = parse(candidate);
            position->setIndex(static_cast<::jxx::lang::jint>(end));
            position->setErrorIndex(-1);
            return result;
        } catch (const ParseException&) {
        }
    }
    position->setErrorIndex(start);
    return nullptr;
}

void SimpleDateFormat::applyPattern(
    const ::jxx::Ptr<::jxx::lang::String>& pattern) {
    if (pattern == nullptr) throw ::jxx::lang::NullPointerException();
    validatePattern_(pattern->utf8());
    pattern_ = pattern;
}
void SimpleDateFormat::applyLocalizedPattern(
    const ::jxx::Ptr<::jxx::lang::String>& pattern) {
    if (pattern == nullptr) throw ::jxx::lang::NullPointerException();
    const std::string standardChars = "GyMdkHmsSEDFwWahKzZYuXL";
    const auto localChars = symbols_->getLocalPatternChars()->utf8();
    applyPattern(::jxx::NEW<::jxx::lang::String>(
        translatePatternLetters(pattern->utf8(), localChars, standardChars)));
}
::jxx::Ptr<::jxx::lang::String> SimpleDateFormat::toPattern() const { return pattern_; }
::jxx::Ptr<::jxx::lang::String> SimpleDateFormat::toLocalizedPattern() const {
    const std::string standardChars = "GyMdkHmsSEDFwWahKzZYuXL";
    const auto localChars = symbols_->getLocalPatternChars()->utf8();
    return ::jxx::NEW<::jxx::lang::String>(
        translatePatternLetters(pattern_->utf8(), standardChars, localChars));
}
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
    copy->symbols_ = getDateFormatSymbols();
    return ::jxx::CAST<::jxx::lang::Object>(copy);
}

} // namespace jxx::text
