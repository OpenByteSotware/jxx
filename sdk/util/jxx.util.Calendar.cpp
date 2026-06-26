#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.String.h"
#include "jxx.util.Calendar.h"


namespace jxx::util {

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

void Calendar::epoch_to_local_parts(jxx::lang::jlong epochMillis, jxx::lang::jint tzOffsetMillis,
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
    millis_ = (jxx::lang::jlong)now * 1000;
    tz_ = TimeZone::getDefault();
}

jxx::Ptr<Calendar> Calendar::getInstance() {
    return jxx::NEW<Calendar>();
}

jxx::lang::jlong Calendar::getTimeInMillis() const { return millis_; }
void Calendar::setTimeInMillis(jxx::lang::jlong millis) { millis_ = millis; }

jxx::Ptr<Date> Calendar::getTime() const { return jxx::NEW<Date>(millis_); }

void Calendar::setTime(jxx::Ptr<Date> date) {
    if (!date) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("null"));
    millis_ = date->getTime();
}

jxx::Ptr<TimeZone> Calendar::getTimeZone() const {
    return tz_ ? tz_ : TimeZone::getDefault();
}

void Calendar::setTimeZone(jxx::Ptr<TimeZone> tz) {
    if (!tz) throw jxx::lang::NullPointerException(jxx::NEW<jxx::lang::String>("null"));
    tz_ = std::move(tz);
}

jxx::lang::jint Calendar::get(jxx::lang::jint field) const {
    auto tz = getTimeZone();
    jxx::lang::jint off = tz ? tz->getOffset(millis_) : 0;

    int y, hh, mm, ss, ms, dow;
    unsigned mo, da;
    epoch_to_local_parts(millis_, off, y, mo, da, hh, mm, ss, ms, dow);

    switch (field) {
        case YEAR: return (jxx::lang::jint)y;
        case MONTH: return (jxx::lang::jint)(mo - 1);
        case DAY_OF_MONTH: return (jxx::lang::jint)da;
        case HOUR_OF_DAY: return (jxx::lang::jint)hh;
        case MINUTE: return (jxx::lang::jint)mm;
        case SECOND: return (jxx::lang::jint)ss;
        case MILLISECOND: return (jxx::lang::jint)ms;
        case DAY_OF_WEEK: return (jxx::lang::jint)dow;
        default:
            throw jxx::lang::IndexOutOfBoundsException(jxx::NEW<jxx::lang::String>("Unsupported Calendar field"));
    }
}

} // namespace jxx::util
