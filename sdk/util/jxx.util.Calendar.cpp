#include "jxx.util.Calendar.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.String.h"

#include <ctime>

using namespace jxx::lang;

namespace jxx::util {

int Calendar::days_from_civil(int y, unsigned m, unsigned d) {
    y -= m <= 2;
    const int era = (y >= 0 ? y : y - 399) / 400;
    const unsigned yoe = (unsigned)(y - era * 400);
    const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + (int)doe - 719468;
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

static inline void epoch_to_local_parts(jlong epochMillis, jint tzOffsetMillis,
                                       int& y, unsigned& mo, unsigned& da,
                                       int& hh, int& mm, int& ss, int& ms,
                                       int& dow_java) {
    long long local = (long long)epochMillis + (long long)tzOffsetMillis;

    ms = (int)(local % 1000);
    if (ms < 0) ms += 1000;

    long long totalSec = local / 1000;
    if (local < 0 && (local % 1000)) --totalSec;

    long long secOfDay = totalSec % 86400;
    if (secOfDay < 0) secOfDay += 86400;

    hh = (int)(secOfDay / 3600);
    mm = (int)((secOfDay % 3600) / 60);
    ss = (int)(secOfDay % 60);

    long long days = totalSec / 86400;
    if (totalSec < 0 && (totalSec % 86400)) --days;

    Calendar::civil_from_days((int)days, y, mo, da);

    int sun0 = (int)((days + 4) % 7);
    if (sun0 < 0) sun0 += 7;
    dow_java = sun0 + 1;
}

Calendar::Calendar() {
    std::time_t now = std::time(nullptr);
    millis_ = (jlong)now * 1000;
    tz_ = TimeZone::getDefault();
}

jxx::Ptr<Calendar> Calendar::getInstance() {
    return std::make_shared<Calendar>();
}

jlong Calendar::getTimeInMillis() const { return millis_; }
void Calendar::setTimeInMillis(jlong millis) { millis_ = millis; }

jxx::Ptr<Date> Calendar::getTime() const { return std::make_shared<Date>(millis_); }

void Calendar::setTime(jxx::Ptr<Date> date) {
    if (!date) throw jxx::lang::NullPointerException(std::make_shared<jxx::lang::String>("null"));
    millis_ = date->getTime();
}

jxx::Ptr<TimeZone> Calendar::getTimeZone() const {
    return tz_ ? tz_ : TimeZone::getDefault();
}

void Calendar::setTimeZone(jxx::Ptr<TimeZone> tz) {
    if (!tz) throw jxx::lang::NullPointerException(std::make_shared<jxx::lang::String>("null"));
    tz_ = std::move(tz);
}

jint Calendar::get(jint field) const {
    auto tz = getTimeZone();
    jint off = tz ? tz->getOffset(millis_) : 0;

    int y, hh, mm, ss, ms, dow;
    unsigned mo, da;
    epoch_to_local_parts(millis_, off, y, mo, da, hh, mm, ss, ms, dow);

    switch (field) {
        case YEAR: return (jint)y;
        case MONTH: return (jint)(mo - 1);
        case DAY_OF_MONTH: return (jint)da;
        case HOUR_OF_DAY: return (jint)hh;
        case MINUTE: return (jint)mm;
        case SECOND: return (jint)ss;
        case MILLISECOND: return (jint)ms;
        case DAY_OF_WEEK: return (jint)dow;
        default:
            throw jxx::lang::IndexOutOfBoundsException(std::make_shared<jxx::lang::String>("Unsupported Calendar field"));
    }
}

void Calendar::add(jint field, jint amount) {
    if (amount == 0) return;
    switch (field) {
    case MILLISECOND: millis_ += (jlong)amount; return;
    case SECOND: millis_ += (jlong)amount * 1000; return;
    case MINUTE: millis_ += (jlong)amount * 60000; return;
    case HOUR_OF_DAY: millis_ += (jlong)amount * 3600000; return;
    case DAY_OF_MONTH: millis_ += (jlong)amount * 86400000; return;
    default: break;
    }
    jlong epoch = millis_;
    jint off = getTimeZone()->getOffset(epoch);
    int y, hh, mm, ss, ms, dow; unsigned mo, da;
    epoch_to_parts(epoch, off, y, mo, da, hh, mm, ss, ms, dow);
    if (field == YEAR) y += amount;
    else if (field == MONTH) {
        int newMonth = (int)mo + amount;
        while (newMonth <= 0) { newMonth += 12; --y; }
        while (newMonth > 12) { newMonth -= 12; ++y; }
        mo = (unsigned)newMonth;
    }
    else {
        throw jxx::lang::IndexOutOfBoundsException(std::make_shared<jxx::lang::String>("Unsupported Calendar add field"));
    }
    auto is_leap = [](int yy) { return (yy % 4 == 0 && yy % 100 != 0) || (yy % 400 == 0); };
    static const int mdays[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    int dim = mdays[mo - 1] + ((mo == 2 && is_leap(y)) ? 1 : 0);
    if ((int)da > dim) da = (unsigned)dim;
    int days = days_from_civil(y, mo, da);
    long long localSeconds = (long long)days * 86400 + hh * 3600 + mm * 60 + ss;
    long long newMillisLocal = localSeconds * 1000 + ms;
    jlong candidateEpoch = (jlong)(newMillisLocal - off);
    jint off2 = getTimeZone()->getOffset(candidateEpoch);
    millis_ = (jlong)(newMillisLocal - off2);
}

} // namespace jxx::util
