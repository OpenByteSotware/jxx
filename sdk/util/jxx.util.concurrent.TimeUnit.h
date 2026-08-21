#pragma once
#include "lang/jxx_types.h"
#include <chrono>
#include <limits>
#include "lang/jxx.lang.Object.h"
namespace jxx::util::concurrent {
class TimeUnit final : public jxx::lang::Object {
public:
    enum class Kind : jxx::lang::jint { NANOSECONDS, MICROSECONDS, MILLISECONDS, SECONDS, MINUTES, HOURS, DAYS };
private:
    Kind kind_;
    explicit TimeUnit(Kind kind) : kind_(kind) {}
    static jxx::lang::jlong factor(Kind k) noexcept {
        switch(k){case Kind::NANOSECONDS:return 1LL;case Kind::MICROSECONDS:return 1000LL;case Kind::MILLISECONDS:return 1000000LL;case Kind::SECONDS:return 1000000000LL;case Kind::MINUTES:return 60000000000LL;case Kind::HOURS:return 3600000000000LL;case Kind::DAYS:return 86400000000000LL;} return 1LL;
    }
    static jxx::lang::jlong saturatedMultiply(jxx::lang::jlong value,jxx::lang::jlong multiplier) noexcept {
        if(value==0||multiplier==0)return 0;
        const auto max=std::numeric_limits<jxx::lang::jlong>::max(); const auto min=std::numeric_limits<jxx::lang::jlong>::min();
        if(value>0&&value>max/multiplier)return max; if(value<0&&value<min/multiplier)return min; return value*multiplier;
    }
public:
    static jxx::Ptr<TimeUnit> NANOSECONDS(){static auto v=jxx::Ptr<TimeUnit>(new TimeUnit(Kind::NANOSECONDS));return v;}
    static jxx::Ptr<TimeUnit> MICROSECONDS(){static auto v=jxx::Ptr<TimeUnit>(new TimeUnit(Kind::MICROSECONDS));return v;}
    static jxx::Ptr<TimeUnit> MILLISECONDS(){static auto v=jxx::Ptr<TimeUnit>(new TimeUnit(Kind::MILLISECONDS));return v;}
    static jxx::Ptr<TimeUnit> SECONDS(){static auto v=jxx::Ptr<TimeUnit>(new TimeUnit(Kind::SECONDS));return v;}
    static jxx::Ptr<TimeUnit> MINUTES(){static auto v=jxx::Ptr<TimeUnit>(new TimeUnit(Kind::MINUTES));return v;}
    static jxx::Ptr<TimeUnit> HOURS(){static auto v=jxx::Ptr<TimeUnit>(new TimeUnit(Kind::HOURS));return v;}
    static jxx::Ptr<TimeUnit> DAYS(){static auto v=jxx::Ptr<TimeUnit>(new TimeUnit(Kind::DAYS));return v;}
    Kind kind() const noexcept{return kind_;}
    jxx::lang::jlong convert(jxx::lang::jlong duration,const jxx::Ptr<TimeUnit>& source) const;
    jxx::lang::jlong toNanos(jxx::lang::jlong d) const noexcept{return saturatedMultiply(d,factor(kind_));}
    jxx::lang::jlong toMicros(jxx::lang::jlong d) const noexcept{return toNanos(d)/factor(Kind::MICROSECONDS);}
    jxx::lang::jlong toMillis(jxx::lang::jlong d) const noexcept{return toNanos(d)/factor(Kind::MILLISECONDS);}
    jxx::lang::jlong toSeconds(jxx::lang::jlong d) const noexcept{return toNanos(d)/factor(Kind::SECONDS);}
    jxx::lang::jlong toMinutes(jxx::lang::jlong d) const noexcept{return toNanos(d)/factor(Kind::MINUTES);}
    jxx::lang::jlong toHours(jxx::lang::jlong d) const noexcept{return toNanos(d)/factor(Kind::HOURS);}
    jxx::lang::jlong toDays(jxx::lang::jlong d) const noexcept{return toNanos(d)/factor(Kind::DAYS);}
    std::chrono::nanoseconds toChrono(jxx::lang::jlong d) const noexcept{return std::chrono::nanoseconds(toNanos(d));}
protected:jxx::Ptr<jxx::lang::Object> cloneImpl() const override{return jxx::Ptr<jxx::lang::Object>(new TimeUnit(kind_));}
};
} // namespace jxx::util::concurrent
