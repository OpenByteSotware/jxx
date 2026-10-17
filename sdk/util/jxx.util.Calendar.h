#pragma once
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.TimeZone.h"

namespace jxx::util {

class Calendar
    : public ::jxx::lang::ClassBase<
          Calendar,
          ::jxx::lang::Object,
          ::jxx::lang::Cloneable,
          ::jxx::io::SerializableI,
          ::jxx::lang::Comparable<Calendar>> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        Calendar,
        JxxSuper,
        ::jxx::lang::Cloneable,
        ::jxx::io::SerializableI,
        ::jxx::lang::Comparable<Calendar>>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();
    static constexpr ::jxx::lang::jint ERA = 0;
    static constexpr ::jxx::lang::jint YEAR = 1;
    static constexpr ::jxx::lang::jint MONTH = 2;
    static constexpr ::jxx::lang::jint WEEK_OF_YEAR = 3;
    static constexpr ::jxx::lang::jint WEEK_OF_MONTH = 4;
    static constexpr ::jxx::lang::jint DAY_OF_MONTH = 5;
    static constexpr ::jxx::lang::jint DATE = DAY_OF_MONTH;
    static constexpr ::jxx::lang::jint DAY_OF_YEAR = 6;
    static constexpr ::jxx::lang::jint DAY_OF_WEEK = 7;
    static constexpr ::jxx::lang::jint DAY_OF_WEEK_IN_MONTH = 8;
    static constexpr ::jxx::lang::jint AM_PM = 9;
    static constexpr ::jxx::lang::jint HOUR = 10;
    static constexpr ::jxx::lang::jint HOUR_OF_DAY = 11;
    static constexpr ::jxx::lang::jint MINUTE = 12;
    static constexpr ::jxx::lang::jint SECOND = 13;
    static constexpr ::jxx::lang::jint MILLISECOND = 14;
    static constexpr ::jxx::lang::jint ZONE_OFFSET = 15;
    static constexpr ::jxx::lang::jint DST_OFFSET = 16;
    static constexpr ::jxx::lang::jint FIELD_COUNT = 17;
    static constexpr ::jxx::lang::jint SUNDAY = 1;
    static constexpr ::jxx::lang::jint MONDAY = 2;
    static constexpr ::jxx::lang::jint TUESDAY = 3;
    static constexpr ::jxx::lang::jint WEDNESDAY = 4;
    static constexpr ::jxx::lang::jint THURSDAY = 5;
    static constexpr ::jxx::lang::jint FRIDAY = 6;
    static constexpr ::jxx::lang::jint SATURDAY = 7;
    static constexpr ::jxx::lang::jint JANUARY = 0;
    static constexpr ::jxx::lang::jint FEBRUARY = 1;
    static constexpr ::jxx::lang::jint MARCH = 2;
    static constexpr ::jxx::lang::jint APRIL = 3;
    static constexpr ::jxx::lang::jint MAY = 4;
    static constexpr ::jxx::lang::jint JUNE = 5;
    static constexpr ::jxx::lang::jint JULY = 6;
    static constexpr ::jxx::lang::jint AUGUST = 7;
    static constexpr ::jxx::lang::jint SEPTEMBER = 8;
    static constexpr ::jxx::lang::jint OCTOBER = 9;
    static constexpr ::jxx::lang::jint NOVEMBER = 10;
    static constexpr ::jxx::lang::jint DECEMBER = 11;
    static constexpr ::jxx::lang::jint UNDECIMBER = 12;
    static constexpr ::jxx::lang::jint AM = 0;
    static constexpr ::jxx::lang::jint PM = 1;

    Calendar();

    ::jxx::lang::jint compareTo(const ::jxx::Ptr<Calendar>& other) const override;
    ::jxx::lang::jbool before(
        const ::jxx::Ptr<::jxx::lang::Object>& when) const;
    ::jxx::lang::jbool after(
        const ::jxx::Ptr<::jxx::lang::Object>& when) const;
    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& other) const override;
    ::jxx::lang::jint hashCode() const override;

    void writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override;
    void readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override;
    void readObjectNoData() override;
    static ::jxx::Ptr<Calendar> getInstance();
    static ::jxx::Ptr<Calendar> getInstance(
        const ::jxx::Ptr<TimeZone>& zone);
    static ::jxx::Ptr<Calendar> getInstance(
        const ::jxx::Ptr<Locale>& locale);
    static ::jxx::Ptr<Calendar> getInstance(
        const ::jxx::Ptr<TimeZone>& zone,
        const ::jxx::Ptr<Locale>& locale);

    ::jxx::lang::jlong getTimeInMillis() const;
    void setTimeInMillis(::jxx::lang::jlong millis);

    ::jxx::Ptr<Date> getTime() const;
    void setTime(const ::jxx::Ptr<Date>& date);

    ::jxx::Ptr<TimeZone> getTimeZone() const;
    void setTimeZone(const ::jxx::Ptr<TimeZone>& tz);

    ::jxx::lang::jint get(::jxx::lang::jint field) const;

private:
    ::jxx::lang::jlong millis_ = 0;
    ::jxx::Ptr<TimeZone> tz_;

    static void civil_from_days(int z, int& y, unsigned& m, unsigned& d);
    static void epoch_to_local_parts(::jxx::lang::jlong epochMillis, ::jxx::lang::jint tzOffsetMillis,
        int& y, unsigned& mo, unsigned& da,
        int& hh, int& mm, int& ss, int& ms,
        int& dow_java);
};

using Calender = Calendar;

} // namespace jxx::util
