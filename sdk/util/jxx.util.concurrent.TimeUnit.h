#pragma once

#include <limits>

#include "io/jxx.lang.Object.h"
#include "io/jxx.lang.Exceptions.h"

namespace jxx { template <typename T> class Ptr; }

namespace jxx {
namespace util {
namespace concurrent {

class TimeUnit : virtual public jxx::lang::Object {
public:
    enum class Kind : jxx::lang::jint {
        NANOSECONDS  = 0,
        MICROSECONDS = 1,
        MILLISECONDS = 2,
        SECONDS      = 3,
        MINUTES      = 4,
        HOURS        = 5,
        DAYS         = 6
    };

private:
    Kind kind_;

    static inline jxx::lang::jlong unitToNanos(Kind kind) {
        switch (kind) {
            case Kind::NANOSECONDS:  return static_cast<jxx::lang::jlong>(1LL);
            case Kind::MICROSECONDS: return static_cast<jxx::lang::jlong>(1000LL);
            case Kind::MILLISECONDS: return static_cast<jxx::lang::jlong>(1000LL * 1000LL);
            case Kind::SECONDS:      return static_cast<jxx::lang::jlong>(1000LL * 1000LL * 1000LL);
            case Kind::MINUTES:      return static_cast<jxx::lang::jlong>(60LL * 1000LL * 1000LL * 1000LL);
            case Kind::HOURS:        return static_cast<jxx::lang::jlong>(60LL * 60LL * 1000LL * 1000LL * 1000LL);
            case Kind::DAYS:         return static_cast<jxx::lang::jlong>(24LL * 60LL * 60LL * 1000LL * 1000LL * 1000LL);
        }
        throw IllegalStateException();
    }

    static inline jxx::lang::jlong safeMul(jxx::lang::jlong a, jxx::lang::jlong b) {
        if (a == 0 || b == 0) return static_cast<jxx::lang::jlong>(0);
        if (a > 0 && b > 0 && a > (std::numeric_limits<jxx::lang::jlong>::max() / b)) {
            return std::numeric_limits<jxx::lang::jlong>::max();
        }
        if (a < 0 && b < 0 && a < (std::numeric_limits<jxx::lang::jlong>::max() / b)) {
            return std::numeric_limits<jxx::lang::jlong>::max();
        }
        if (a > 0 && b < 0 && b < (std::numeric_limits<jxx::lang::jlong>::min() / a)) {
            return std::numeric_limits<jxx::lang::jlong>::min();
        }
        if (a < 0 && b > 0 && a < (std::numeric_limits<jxx::lang::jlong>::min() / b)) {
            return std::numeric_limits<jxx::lang::jlong>::min();
        }
        return static_cast<jxx::lang::jlong>(a * b);
    }

    static inline jxx::lang::jlong nanosTo(Kind target, jxx::lang::jlong nanos) {
        const jxx::lang::jlong div = unitToNanos(target);
        return static_cast<jxx::lang::jlong>(nanos / div);
    }

    explicit TimeUnit(Kind kind)
        : kind_(kind) {
    }

public:
    virtual ~TimeUnit() = default;

    virtual Kind kind() const {
        return kind_;
    }

    virtual jxx::lang::jlong convert(
        jxx::lang::jlong sourceDuration,
        jxx::Ptr<TimeUnit> sourceUnit) {
        if (sourceUnit == nullptr) {
            throw NullPointerException();
        }
        const jxx::lang::jlong sourceNanos = safeMul(sourceDuration, unitToNanos(sourceUnit->kind()));
        return nanosTo(kind_, sourceNanos);
    }

    virtual jxx::lang::jlong toNanos(jxx::lang::jlong duration) {
        return safeMul(duration, unitToNanos(kind_));
    }

    virtual jxx::lang::jlong toMicros(jxx::lang::jlong duration) {
        return nanosTo(Kind::MICROSECONDS, toNanos(duration));
    }

    virtual jxx::lang::jlong toMillis(jxx::lang::jlong duration) {
        return nanosTo(Kind::MILLISECONDS, toNanos(duration));
    }

    virtual jxx::lang::jlong toSeconds(jxx::lang::jlong duration) {
        return nanosTo(Kind::SECONDS, toNanos(duration));
    }

    virtual jxx::lang::jlong toMinutes(jxx::lang::jlong duration) {
        return nanosTo(Kind::MINUTES, toNanos(duration));
    }

    virtual jxx::lang::jlong toHours(jxx::lang::jlong duration) {
        return nanosTo(Kind::HOURS, toNanos(duration));
    }

    virtual jxx::lang::jlong toDays(jxx::lang::jlong duration) {
        return nanosTo(Kind::DAYS, toNanos(duration));
    }

    static jxx::Ptr<TimeUnit> NANOSECONDS() {
        static jxx::Ptr<TimeUnit> value(new TimeUnit(Kind::NANOSECONDS));
        return value;
    }

    static jxx::Ptr<TimeUnit> MICROSECONDS() {
        static jxx::Ptr<TimeUnit> value(new TimeUnit(Kind::MICROSECONDS));
        return value;
    }

    static jxx::Ptr<TimeUnit> MILLISECONDS() {
        static jxx::Ptr<TimeUnit> value(new TimeUnit(Kind::MILLISECONDS));
        return value;
    }

    static jxx::Ptr<TimeUnit> SECONDS() {
        static jxx::Ptr<TimeUnit> value(new TimeUnit(Kind::SECONDS));
        return value;
    }

    static jxx::Ptr<TimeUnit> MINUTES() {
        static jxx::Ptr<TimeUnit> value(new TimeUnit(Kind::MINUTES));
        return value;
    }

    static jxx::Ptr<TimeUnit> HOURS() {
        static jxx::Ptr<TimeUnit> value(new TimeUnit(Kind::HOURS));
        return value;
    }

    static jxx::Ptr<TimeUnit> DAYS() {
        static jxx::Ptr<TimeUnit> value(new TimeUnit(Kind::DAYS));
        return value;
    }
};

} // namespace concurrent
} // namespace util
} // namespace jxx
