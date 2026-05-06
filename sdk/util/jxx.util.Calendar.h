#pragma once
#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.String.h"
#include "jxx.util.Date.h"
#include "jxx.util.TimeZone.h"
namespace jxx::util {
class Calendar : public jxx::lang::Object {
public:
    static constexpr jint YEAR=1, MONTH=2, DAY_OF_MONTH=5, DAY_OF_WEEK=7;
    static constexpr jint HOUR_OF_DAY=11, MINUTE=12, SECOND=13, MILLISECOND=14;

    Calendar();
    static jxx::Ptr<Calendar> getInstance();

    jlong getTimeInMillis() const;
    void setTimeInMillis(jlong millis);

    jxx::Ptr<Date> getTime() const;
    void setTime(jxx::Ptr<Date> date);

    jxx::Ptr<TimeZone> getTimeZone() const;
    void setTimeZone(jxx::Ptr<TimeZone> tz);

    jint get(jint field) const;
    void add(jint field, jint amount);

private:
    jlong millis_ = 0;
    jxx::Ptr<TimeZone> tz_;

    static int days_from_civil(int y, unsigned m, unsigned d);
    static void civil_from_days(int z, int& y, unsigned& m, unsigned& d);
};
using Calender = Calendar;
}