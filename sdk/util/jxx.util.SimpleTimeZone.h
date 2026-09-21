#pragma once

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "util/jxx.util.TimeZone.h"

namespace jxx::util {

class SimpleTimeZone final : public TimeZone {
public:
    static constexpr ::jxx::lang::jint WALL_TIME = 0;
    static constexpr ::jxx::lang::jint STANDARD_TIME = 1;
    static constexpr ::jxx::lang::jint UTC_TIME = 2;

    SimpleTimeZone(::jxx::lang::jint rawOffset,
                   const ::jxx::Ptr<::jxx::lang::String>& identifier);
    SimpleTimeZone(::jxx::lang::jint rawOffset,
                   const ::jxx::Ptr<::jxx::lang::String>& identifier,
                   ::jxx::lang::jint startMonth,
                   ::jxx::lang::jint startDay,
                   ::jxx::lang::jint startDayOfWeek,
                   ::jxx::lang::jint startTime,
                   ::jxx::lang::jint endMonth,
                   ::jxx::lang::jint endDay,
                   ::jxx::lang::jint endDayOfWeek,
                   ::jxx::lang::jint endTime);
    SimpleTimeZone(::jxx::lang::jint rawOffset,
                   const ::jxx::Ptr<::jxx::lang::String>& identifier,
                   ::jxx::lang::jint startMonth,
                   ::jxx::lang::jint startDay,
                   ::jxx::lang::jint startDayOfWeek,
                   ::jxx::lang::jint startTime,
                   ::jxx::lang::jint startTimeMode,
                   ::jxx::lang::jint endMonth,
                   ::jxx::lang::jint endDay,
                   ::jxx::lang::jint endDayOfWeek,
                   ::jxx::lang::jint endTime,
                   ::jxx::lang::jint endTimeMode,
                   ::jxx::lang::jint dstSavings);

    ::jxx::Ptr<::jxx::lang::String> getID() const override;
    ::jxx::lang::jint getOffset(::jxx::lang::jlong epochMillis) const override;
    ::jxx::Ptr<::jxx::lang::String> getAbbreviation(
        ::jxx::lang::jlong epochMillis) const override;
    ::jxx::lang::jint getRawOffset() const override;
    void setRawOffset(::jxx::lang::jint offset);
    ::jxx::lang::jbool useDaylightTime() const override;
    ::jxx::lang::jbool inDaylightTime(const ::jxx::Ptr<Date> date) const override;
    ::jxx::lang::jint getDSTSavings() const;
    void setDSTSavings(::jxx::lang::jint milliseconds);
    void setStartYear(::jxx::lang::jint year);
    void setStartRule(::jxx::lang::jint month, ::jxx::lang::jint day,
                      ::jxx::lang::jint dayOfWeek, ::jxx::lang::jint time);
    void setEndRule(::jxx::lang::jint month, ::jxx::lang::jint day,
                    ::jxx::lang::jint dayOfWeek, ::jxx::lang::jint time);
    ::jxx::lang::jbool hasSameRules(const ::jxx::Ptr<TimeZone>& other) const;
    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& other) const override;
    ::jxx::lang::jint hashCode() const override;

private:
    struct Rule { ::jxx::lang::jint month=0, day=0, dayOfWeek=0, time=0, mode=WALL_TIME; };
    static void validateRule_(const Rule& rule);
    static ::jxx::lang::jlong transition_(::jxx::lang::jint year,
        const Rule& rule, ::jxx::lang::jint rawOffset,
        ::jxx::lang::jint daylightBefore);

    ::jxx::Ptr<::jxx::lang::String> id_;
    ::jxx::lang::jint rawOffset_ = 0;
    ::jxx::lang::jint dstSavings_ = 3600000;
    ::jxx::lang::jint startYear_ = 0;
    ::jxx::lang::jbool usesDaylight_ = false;
    Rule start_;
    Rule end_;
};

} // namespace jxx::util
