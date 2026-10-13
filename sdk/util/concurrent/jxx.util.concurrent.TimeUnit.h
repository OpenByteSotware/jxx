#pragma once

#include <chrono>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.Thread.h"
#include "lang/jxx_types.h"

namespace jxx::util::concurrent {

class TimeUnit final
    : public ::jxx::lang::ClassBase<TimeUnit, ::jxx::lang::Object> {
public:
    enum class Kind : ::jxx::lang::jint {
        NANOSECONDS,
        MICROSECONDS,
        MILLISECONDS,
        SECONDS,
        MINUTES,
        HOURS,
        DAYS
    };

    static ::jxx::Ptr<TimeUnit> NANOSECONDS();
    static ::jxx::Ptr<TimeUnit> MICROSECONDS();
    static ::jxx::Ptr<TimeUnit> MILLISECONDS();
    static ::jxx::Ptr<TimeUnit> SECONDS();
    static ::jxx::Ptr<TimeUnit> MINUTES();
    static ::jxx::Ptr<TimeUnit> HOURS();
    static ::jxx::Ptr<TimeUnit> DAYS();

    static ::jxx::Ptr<::jxx::lang::JxxArray<
        ::jxx::Ptr<TimeUnit>, 1U>> values();
    static ::jxx::Ptr<TimeUnit> valueOf(
        const ::jxx::Ptr<::jxx::lang::String>& name);

    ::jxx::Ptr<::jxx::lang::String> name() const;
    ::jxx::lang::jint ordinal() const noexcept;
    ::jxx::Ptr<::jxx::lang::String> toString() const override;

    ::jxx::lang::jlong convert(
        ::jxx::lang::jlong sourceDuration,
        const ::jxx::Ptr<TimeUnit>& sourceUnit) const;
    ::jxx::lang::jlong toNanos(::jxx::lang::jlong duration) const;
    ::jxx::lang::jlong toMicros(::jxx::lang::jlong duration) const;
    ::jxx::lang::jlong toMillis(::jxx::lang::jlong duration) const;
    ::jxx::lang::jlong toSeconds(::jxx::lang::jlong duration) const;
    ::jxx::lang::jlong toMinutes(::jxx::lang::jlong duration) const;
    ::jxx::lang::jlong toHours(::jxx::lang::jlong duration) const;
    ::jxx::lang::jlong toDays(::jxx::lang::jlong duration) const;
    std::chrono::nanoseconds toChrono(::jxx::lang::jlong duration) const;
    void sleep(::jxx::lang::jlong timeout) const;
    void timedWait(
        const ::jxx::Ptr<::jxx::lang::Object>& object,
        ::jxx::lang::jlong timeout) const;
    void timedJoin(
        const ::jxx::Ptr<::jxx::lang::Thread>& thread,
        ::jxx::lang::jlong timeout) const;

public:
    explicit TimeUnit(Kind kind);

    private:
    static ::jxx::lang::jlong factor(Kind kind) noexcept;
    static ::jxx::lang::jlong saturatedMultiply(
        ::jxx::lang::jlong value,
        ::jxx::lang::jlong multiplier) noexcept;
    ::jxx::lang::jlong convertToFactor_(
        ::jxx::lang::jlong duration,
        ::jxx::lang::jlong targetFactor) const noexcept;
    ::jxx::lang::jint excessNanos_(
        ::jxx::lang::jlong duration,
        ::jxx::lang::jlong millis) const noexcept;
    Kind kind_;
};

} // namespace jxx::util::concurrent
