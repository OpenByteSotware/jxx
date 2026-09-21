#pragma once

#include "util/jxx.util.Calendar.h"
#include "util/jxx.util.Locale.h"

namespace jxx::util {

class GregorianCalendar : public Calendar {
public:
    static constexpr ::jxx::lang::jint BC = 0;
    static constexpr ::jxx::lang::jint AD = 1;

    GregorianCalendar();
    explicit GregorianCalendar(const ::jxx::Ptr<TimeZone>& zone);
    explicit GregorianCalendar(const ::jxx::Ptr<Locale>& locale);
    GregorianCalendar(const ::jxx::Ptr<TimeZone>& zone,
                      const ::jxx::Ptr<Locale>& locale);
    GregorianCalendar(::jxx::lang::jint year,
                      ::jxx::lang::jint month,
                      ::jxx::lang::jint dayOfMonth);
    GregorianCalendar(::jxx::lang::jint year,
                      ::jxx::lang::jint month,
                      ::jxx::lang::jint dayOfMonth,
                      ::jxx::lang::jint hourOfDay,
                      ::jxx::lang::jint minute);
    GregorianCalendar(::jxx::lang::jint year,
                      ::jxx::lang::jint month,
                      ::jxx::lang::jint dayOfMonth,
                      ::jxx::lang::jint hourOfDay,
                      ::jxx::lang::jint minute,
                      ::jxx::lang::jint second);

    ::jxx::lang::jbool isLeapYear(::jxx::lang::jint year) const;
    void setGregorianChange(const ::jxx::Ptr<Date>& date);
    ::jxx::Ptr<Date> getGregorianChange() const;
    void set(::jxx::lang::jint field, ::jxx::lang::jint value);
    void set(::jxx::lang::jint year, ::jxx::lang::jint month,
             ::jxx::lang::jint dayOfMonth);
    void set(::jxx::lang::jint year, ::jxx::lang::jint month,
             ::jxx::lang::jint dayOfMonth, ::jxx::lang::jint hourOfDay,
             ::jxx::lang::jint minute, ::jxx::lang::jint second);
    void add(::jxx::lang::jint field, ::jxx::lang::jint amount);
    void roll(::jxx::lang::jint field, ::jxx::lang::jint amount);
    ::jxx::lang::jint getActualMaximum(::jxx::lang::jint field) const;
    ::jxx::lang::jint getActualMinimum(::jxx::lang::jint field) const;
    ::jxx::Ptr<::jxx::lang::String> getCalendarType() const;

private:
    static ::jxx::lang::jlong localToEpoch_(
        ::jxx::lang::jint year, ::jxx::lang::jint month,
        ::jxx::lang::jint day, ::jxx::lang::jint hour,
        ::jxx::lang::jint minute, ::jxx::lang::jint second,
        ::jxx::lang::jint millisecond,
        const ::jxx::Ptr<TimeZone>& zone);
    static ::jxx::lang::jint daysInMonth_(
        ::jxx::lang::jint year, ::jxx::lang::jint month);
    void setLocal_(::jxx::lang::jint year, ::jxx::lang::jint month,
                   ::jxx::lang::jint day, ::jxx::lang::jint hour,
                   ::jxx::lang::jint minute, ::jxx::lang::jint second,
                   ::jxx::lang::jint millisecond);

    ::jxx::Ptr<Locale> locale_;
    ::jxx::Ptr<Date> gregorianChange_;
};

} // namespace jxx::util
