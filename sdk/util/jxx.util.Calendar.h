#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "jxx.util.Date.h"
#include "jxx.util.TimeZone.h"

namespace jxx::util {

class Calendar : public jxx::lang::Object {
public:
    // Java 8 field constants (subset)
    static constexpr jxx::lang::jint YEAR = 1;
    static constexpr jxx::lang::jint MONTH = 2;          // 0-based
    static constexpr jxx::lang::jint DAY_OF_MONTH = 5;
    static constexpr jxx::lang::jint DAY_OF_WEEK = 7;    // 1=Sunday..7=Saturday
    static constexpr jxx::lang::jint HOUR_OF_DAY = 11;
    static constexpr jxx::lang::jint MINUTE = 12;
    static constexpr jxx::lang::jint SECOND = 13;
    static constexpr jxx::lang::jint MILLISECOND = 14;

    Calendar();

    static jxx::Ptr<Calendar> getInstance();

    jxx::lang::jlong getTimeInMillis() const;
    void setTimeInMillis(jxx::lang::jlong millis);

    jxx::Ptr<Date> getTime() const;
    void setTime(jxx::Ptr<Date> date);

    jxx::Ptr<TimeZone> getTimeZone() const;
    void setTimeZone(jxx::Ptr<TimeZone> tz);

    jxx::lang::jint get(jxx::lang::jint field) const;

    void add(jxx::lang::jint field, jxx::lang::jint amount);


private:
    jxx::lang::jlong millis_ = 0;
    jxx::Ptr<TimeZone> tz_;

    static int days_from_civil(int y, unsigned m, unsigned d);
    static void civil_from_days(int z, int& y, unsigned& m, unsigned& d);
};

using Calender = Calendar;

} // namespace jxx::util
