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
    // Java 8 field constants (subset)
    static constexpr ::jxx::lang::jint YEAR = 1;
    static constexpr ::jxx::lang::jint MONTH = 2;          // 0-based
    static constexpr ::jxx::lang::jint DAY_OF_MONTH = 5;
    static constexpr ::jxx::lang::jint DAY_OF_WEEK = 7;    // 1=Sunday..7=Saturday
    static constexpr ::jxx::lang::jint HOUR_OF_DAY = 11;
    static constexpr ::jxx::lang::jint MINUTE = 12;
    static constexpr ::jxx::lang::jint SECOND = 13;
    static constexpr ::jxx::lang::jint MILLISECOND = 14;

    Calendar();

    ::jxx::lang::jint compareTo(const ::jxx::Ptr<Calendar>& other) const override;

    void writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override {
        (void)out;
    }
    void readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override {
        (void)in;
    }
    void readObjectNoData() override {}
    static ::jxx::Ptr<Calendar> getInstance();

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
