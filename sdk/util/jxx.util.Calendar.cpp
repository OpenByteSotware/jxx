#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Calendar.h"
#include "util/jxx.util.GregorianCalendar.h"
#include "util/jxx.util.Locale.h"
#include "io/jxx.io.InvalidObjectException.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"


namespace jxx::util {

::jxx::Ptr<::jxx::lang::ClassAny>
Calendar::Class() {
    return JxxClassInfoMarker::Class();
}

void Calendar::civil_from_days(int z, int& y, unsigned& m, unsigned& d) {
    z += 719468;
    const int era = (z >= 0 ? z : z - 146096) / 146097;
    const unsigned doe = (unsigned)(z - era * 146097);
    const unsigned yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;
    y = (int)yoe + era * 400;
    const unsigned doy = doe - (365*yoe + yoe/4 - yoe/100);
    const unsigned mp = (5*doy + 2)/153;
    d = doy - (153*mp+2)/5 + 1;
    m = mp + (mp < 10 ? 3 : -9);
    y += (m <= 2);
}

void Calendar::epoch_to_local_parts(::jxx::lang::jlong epochMillis, ::jxx::lang::jint tzOffsetMillis,
                                       int& y, unsigned& mo, unsigned& da,
                                       int& hh, int& mm, int& ss, int& ms,
                                       int& dow_java) {
    long long local = (long long)epochMillis + (long long)tzOffsetMillis;

    ms = (int)(local % 1000);
    if (ms < 0) ms += 1000;

    long long totalSec = local / 1000;
    if (local < 0 & (local % 1000)) --totalSec;

    long long secOfDay = totalSec % 86400;
    if (secOfDay < 0) secOfDay += 86400;

    hh = (int)(secOfDay / 3600);
    mm = (int)((secOfDay % 3600) / 60);
    ss = (int)(secOfDay % 60);

    long long days = totalSec / 86400;
    if (totalSec < 0 & (totalSec % 86400)) --days;

    Calendar::civil_from_days((int)days, y, mo, da);

    int sun0 = (int)((days + 4) % 7);
    if (sun0 < 0) sun0 += 7;
    dow_java = sun0 + 1;
}

Calendar::Calendar() {
    std::time_t now = std::time(nullptr);
    millis_ = (::jxx::lang::jlong)now * 1000;
    tz_ = TimeZone::getDefault();
}

::jxx::Ptr<Calendar> Calendar::getInstance() {
    return ::jxx::CAST<Calendar>(::jxx::NEW<GregorianCalendar>());
}

::jxx::Ptr<Calendar> Calendar::getInstance(
    const ::jxx::Ptr<TimeZone>& zone) {
    if (zone == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    return ::jxx::CAST<Calendar>(::jxx::NEW<GregorianCalendar>(zone));
}

::jxx::Ptr<Calendar> Calendar::getInstance(
    const ::jxx::Ptr<Locale>& locale) {
    if (locale == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    return ::jxx::CAST<Calendar>(::jxx::NEW<GregorianCalendar>(locale));
}

::jxx::Ptr<Calendar> Calendar::getInstance(
    const ::jxx::Ptr<TimeZone>& zone,
    const ::jxx::Ptr<Locale>& locale) {
    if (zone == nullptr || locale == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    return ::jxx::CAST<Calendar>(
        ::jxx::NEW<GregorianCalendar>(zone, locale));
}

::jxx::lang::jint Calendar::compareTo(const ::jxx::Ptr<Calendar>& other) const {
    if (other == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    if (millis_ < other->millis_) return -1;
    if (millis_ > other->millis_) return 1;
    return 0;
}

::jxx::lang::jbool Calendar::before(
    const ::jxx::Ptr<::jxx::lang::Object>& when) const {
    const auto calendar = ::jxx::CAST<Calendar>(when);
    return calendar != nullptr && millis_ < calendar->millis_;
}

::jxx::lang::jbool Calendar::after(
    const ::jxx::Ptr<::jxx::lang::Object>& when) const {
    const auto calendar = ::jxx::CAST<Calendar>(when);
    return calendar != nullptr && millis_ > calendar->millis_;
}

::jxx::lang::jbool Calendar::equals(
    const ::jxx::Ptr<::jxx::lang::Object>& other) const {
    if (other.get() == this) return true;
    const auto calendar = ::jxx::CAST<Calendar>(other);
    if (calendar == nullptr || millis_ != calendar->millis_) return false;
    if (tz_ == nullptr || calendar->tz_ == nullptr) return tz_ == calendar->tz_;
    return tz_->equals(::jxx::CAST<::jxx::lang::Object>(calendar->tz_));
}

::jxx::lang::jint Calendar::hashCode() const {
    const auto timeHash = static_cast<::jxx::lang::jint>(millis_ ^ (millis_ >> 32));
    const auto zoneHash = tz_ == nullptr ? 0 : tz_->hashCode();
    return timeHash ^ zoneHash;
}

::jxx::lang::jlong Calendar::getTimeInMillis() const { return millis_; }
void Calendar::setTimeInMillis(::jxx::lang::jlong millis) { millis_ = millis; }

::jxx::Ptr<Date> Calendar::getTime() const { return ::jxx::NEW<Date>(millis_); }

void Calendar::setTime(const ::jxx::Ptr<Date>& date) {
    if (!date) throw ::jxx::lang::NullPointerException(::jxx::NEW<::jxx::lang::String>("null"));
    millis_ = date->getTime();
}

::jxx::Ptr<TimeZone> Calendar::getTimeZone() const {
    return tz_ ? tz_ : TimeZone::getDefault();
}

void Calendar::setTimeZone(const ::jxx::Ptr<TimeZone>& tz) {
    if (!tz) throw ::jxx::lang::NullPointerException(::jxx::NEW<::jxx::lang::String>("null"));
    tz_ = std::move(tz);
}

void Calendar::setFirstDayOfWeek(::jxx::lang::jint value) {
    if (value < SUNDAY || value > SATURDAY) {
        throw ::jxx::lang::IllegalArgumentException();
    }
    firstDayOfWeek_ = value;
}

::jxx::lang::jint Calendar::getFirstDayOfWeek() const noexcept {
    return firstDayOfWeek_;
}

void Calendar::setMinimalDaysInFirstWeek(::jxx::lang::jint value) {
    if (value < 1 || value > 7) {
        throw ::jxx::lang::IllegalArgumentException();
    }
    minimalDaysInFirstWeek_ = value;
}

::jxx::lang::jint Calendar::getMinimalDaysInFirstWeek() const noexcept {
    return minimalDaysInFirstWeek_;
}

::jxx::lang::jint Calendar::get(::jxx::lang::jint field) const {
    auto tz = getTimeZone();
    ::jxx::lang::jint off = tz ? tz->getOffset(millis_) : 0;

    int y, hh, mm, ss, ms, dow;
    unsigned mo, da;
    epoch_to_local_parts(millis_, off, y, mo, da, hh, mm, ss, ms, dow);

    static constexpr ::jxx::lang::jint cumulativeDays[12] =
        {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    const auto leap = (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
    const auto dayOfYear = cumulativeDays[mo - 1] +
        static_cast<::jxx::lang::jint>(da) + ((leap && mo > 2) ? 1 : 0);
    const auto rawOffset = tz ? tz->getRawOffset() : 0;
    const auto dayOfMonth = static_cast<::jxx::lang::jint>(da);
    const auto normalizedDow = [](::jxx::lang::jint value) {
        auto result = value % 7;
        return result < 0 ? result + 7 : result;
    };
    const auto weekNumber = [&](::jxx::lang::jint day,
                                ::jxx::lang::jint firstDayDow) {
        const auto offset = normalizedDow(firstDayDow - firstDayOfWeek_);
        auto firstWeekStart = 1 - offset;
        if (7 - offset < minimalDaysInFirstWeek_) firstWeekStart += 7;
        return day < firstWeekStart ? 0 : (day - firstWeekStart) / 7 + 1;
    };
    const auto firstDowOfMonth = normalizedDow(dow - ((dayOfMonth - 1) % 7) - 1) + 1;
    const auto firstDowOfYear = normalizedDow(dow - ((dayOfYear - 1) % 7) - 1) + 1;

    switch (field) {
        case ERA: return y <= 0 ? 0 : 1;
        case YEAR: return y <= 0 ? 1 - y : y;
        case MONTH: return static_cast<::jxx::lang::jint>(mo - 1);
        case WEEK_OF_YEAR: return weekNumber(dayOfYear, firstDowOfYear);
        case WEEK_OF_MONTH: return weekNumber(dayOfMonth, firstDowOfMonth);
        case DAY_OF_MONTH: return dayOfMonth;
        case DAY_OF_YEAR: return dayOfYear;
        case DAY_OF_WEEK: return static_cast<::jxx::lang::jint>(dow);
        case DAY_OF_WEEK_IN_MONTH:
            return (static_cast<::jxx::lang::jint>(da) - 1) / 7 + 1;
        case AM_PM: return hh < 12 ? AM : PM;
        case HOUR: return static_cast<::jxx::lang::jint>(hh % 12);
        case HOUR_OF_DAY: return static_cast<::jxx::lang::jint>(hh);
        case MINUTE: return static_cast<::jxx::lang::jint>(mm);
        case SECOND: return static_cast<::jxx::lang::jint>(ss);
        case MILLISECOND: return static_cast<::jxx::lang::jint>(ms);
        case ZONE_OFFSET: return rawOffset;
        case DST_OFFSET: return off - rawOffset;
        default:
            throw ::jxx::lang::IndexOutOfBoundsException(::jxx::NEW<::jxx::lang::String>("Unsupported Calendar field"));
    }
}


void Calendar::writeObject(
    const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) {
    if (out == nullptr) throw ::jxx::lang::NullPointerException();
    out->writeLong(millis_);
    const auto zone = getTimeZone();
    out->writeObject(::jxx::CAST<::jxx::lang::Object>(
        zone == nullptr ? nullptr : zone->getID()));
}

void Calendar::readObject(
    const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) {
    if (in == nullptr) throw ::jxx::lang::NullPointerException();
    const auto millis = in->readLong();
    const auto zoneId = ::jxx::CAST<::jxx::lang::String>(in->readObject());
    if (zoneId == nullptr) {
        throw ::jxx::io::InvalidObjectException(
            ::jxx::NEW<::jxx::lang::String>("Calendar time-zone ID is null"));
    }
    const auto zone = TimeZone::getTimeZone(zoneId);
    if (zone == nullptr) {
        throw ::jxx::io::InvalidObjectException(
            ::jxx::NEW<::jxx::lang::String>("Calendar time zone is unavailable"));
    }
    millis_ = millis;
    tz_ = zone;
}

void Calendar::readObjectNoData() {
    millis_ = 0;
    tz_ = TimeZone::getDefault();
}

} // namespace jxx::util
