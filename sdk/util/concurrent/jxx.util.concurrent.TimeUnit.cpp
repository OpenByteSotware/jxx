#include <limits>
#include <thread>

#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace jxx::util::concurrent {

namespace {
const char* timeUnitName(::jxx::util::concurrent::TimeUnit::Kind kind) noexcept {
    using Kind = ::jxx::util::concurrent::TimeUnit::Kind;
    switch (kind) {
    case Kind::NANOSECONDS: return "NANOSECONDS";
    case Kind::MICROSECONDS: return "MICROSECONDS";
    case Kind::MILLISECONDS: return "MILLISECONDS";
    case Kind::SECONDS: return "SECONDS";
    case Kind::MINUTES: return "MINUTES";
    case Kind::HOURS: return "HOURS";
    case Kind::DAYS: return "DAYS";
    }
    return "";
}
} // namespace


TimeUnit::TimeUnit(Kind kind) : kind_(kind) {}

::jxx::lang::jlong TimeUnit::factor(Kind kind) noexcept {
    switch (kind) {
        case Kind::NANOSECONDS: return 1LL;
        case Kind::MICROSECONDS: return 1000LL;
        case Kind::MILLISECONDS: return 1000000LL;
        case Kind::SECONDS: return 1000000000LL;
        case Kind::MINUTES: return 60000000000LL;
        case Kind::HOURS: return 3600000000000LL;
        case Kind::DAYS: return 86400000000000LL;
    }
    return 1LL;
}

::jxx::lang::jlong TimeUnit::saturatedMultiply(::jxx::lang::jlong value, ::jxx::lang::jlong multiplier) noexcept {
    if (value == 0 || multiplier == 0) return 0;
    const auto maximum = std::numeric_limits<::jxx::lang::jlong>::max();
    const auto minimum = std::numeric_limits<::jxx::lang::jlong>::min();
    if (value > 0 && value > maximum / multiplier) return maximum;
    if (value < 0 && value < minimum / multiplier) return minimum;
    return value * multiplier;
}

::jxx::lang::jlong TimeUnit::convertToFactor_(
    ::jxx::lang::jlong duration,
    ::jxx::lang::jlong targetFactor) const noexcept {
    const auto sourceFactor = factor(kind_);
    if (sourceFactor == targetFactor) return duration;
    if (sourceFactor < targetFactor) {
        return duration / (targetFactor / sourceFactor);
    }
    return saturatedMultiply(duration, sourceFactor / targetFactor);
}

::jxx::lang::jint TimeUnit::excessNanos_(
    ::jxx::lang::jlong duration,
    ::jxx::lang::jlong millis) const noexcept {
    switch (kind_) {
    case Kind::NANOSECONDS:
        return static_cast<::jxx::lang::jint>(
            duration - millis * 1000000LL);
    case Kind::MICROSECONDS:
        return static_cast<::jxx::lang::jint>(
            (duration - millis * 1000LL) * 1000LL);
    default:
        return 0;
    }
}

#define UNIT_FACTORY(NAME) ::jxx::Ptr<TimeUnit> TimeUnit::NAME() { static auto value = ::jxx::NEW<TimeUnit>(Kind::NAME); return value; }
UNIT_FACTORY(NANOSECONDS)
UNIT_FACTORY(MICROSECONDS)
UNIT_FACTORY(MILLISECONDS)
UNIT_FACTORY(SECONDS)
UNIT_FACTORY(MINUTES)
UNIT_FACTORY(HOURS)
UNIT_FACTORY(DAYS)
#undef UNIT_FACTORY

::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<TimeUnit>, 1U>>
TimeUnit::values() {
    auto result = ::jxx::NEW<::jxx::lang::JxxArray<
        ::jxx::Ptr<TimeUnit>, 1U>>(7U);
    (*result)[0] = NANOSECONDS();
    (*result)[1] = MICROSECONDS();
    (*result)[2] = MILLISECONDS();
    (*result)[3] = SECONDS();
    (*result)[4] = MINUTES();
    (*result)[5] = HOURS();
    (*result)[6] = DAYS();
    return result;
}

::jxx::Ptr<TimeUnit> TimeUnit::valueOf(
    const ::jxx::Ptr<::jxx::lang::String>& nameValue) {
    if (nameValue == nullptr) throw ::jxx::lang::NullPointerException();
    const auto text = nameValue->utf8();
    if (text == "NANOSECONDS") return NANOSECONDS();
    if (text == "MICROSECONDS") return MICROSECONDS();
    if (text == "MILLISECONDS") return MILLISECONDS();
    if (text == "SECONDS") return SECONDS();
    if (text == "MINUTES") return MINUTES();
    if (text == "HOURS") return HOURS();
    if (text == "DAYS") return DAYS();
    throw ::jxx::lang::IllegalArgumentException();
}

::jxx::Ptr<::jxx::lang::String> TimeUnit::name() const {
    return ::jxx::NEW<::jxx::lang::String>(timeUnitName(kind_));
}

::jxx::lang::jint TimeUnit::ordinal() const noexcept {
    return static_cast<::jxx::lang::jint>(kind_);
}

::jxx::Ptr<::jxx::lang::String> TimeUnit::toString() const {
    return name();
}

::jxx::lang::jlong TimeUnit::toNanos(::jxx::lang::jlong duration) const { return saturatedMultiply(duration, factor(kind_)); }
::jxx::lang::jlong TimeUnit::convert(
    ::jxx::lang::jlong duration,
    const ::jxx::Ptr<TimeUnit>& source) const {
    if (source == nullptr) throw ::jxx::lang::NullPointerException();

    const auto sourceFactor = factor(source->kind_);
    const auto targetFactor = factor(kind_);
    if (sourceFactor == targetFactor) return duration;

    if (sourceFactor < targetFactor) {
        return duration / (targetFactor / sourceFactor);
    }

    return saturatedMultiply(duration, sourceFactor / targetFactor);
}
::jxx::lang::jlong TimeUnit::toMicros(::jxx::lang::jlong d) const {
    return convertToFactor_(d, 1000LL);
}
::jxx::lang::jlong TimeUnit::toMillis(::jxx::lang::jlong d) const {
    return convertToFactor_(d, 1000000LL);
}
::jxx::lang::jlong TimeUnit::toSeconds(::jxx::lang::jlong d) const {
    return convertToFactor_(d, 1000000000LL);
}
::jxx::lang::jlong TimeUnit::toMinutes(::jxx::lang::jlong d) const {
    return convertToFactor_(d, 60000000000LL);
}
::jxx::lang::jlong TimeUnit::toHours(::jxx::lang::jlong d) const {
    return convertToFactor_(d, 3600000000000LL);
}
::jxx::lang::jlong TimeUnit::toDays(::jxx::lang::jlong d) const {
    return convertToFactor_(d, 86400000000000LL);
}
std::chrono::nanoseconds TimeUnit::toChrono(::jxx::lang::jlong d) const { return std::chrono::nanoseconds(toNanos(d)); }
void TimeUnit::sleep(::jxx::lang::jlong timeout) const {
    if (timeout <= 0) return;

    const auto millis = toMillis(timeout);
    const auto nanoRemainder = excessNanos_(timeout, millis);
    ::jxx::lang::Thread::sleep(millis, nanoRemainder);
}
void TimeUnit::timedWait(
    const ::jxx::Ptr<::jxx::lang::Object>& object,
    ::jxx::lang::jlong timeout) const {
    if (object == nullptr) throw ::jxx::lang::NullPointerException();
    if (timeout <= 0) return;
    const auto millis = toMillis(timeout);
    const auto nanoRemainder = excessNanos_(timeout, millis);
    object->wait(millis, nanoRemainder);
}

void TimeUnit::timedJoin(
    const ::jxx::Ptr<::jxx::lang::Thread>& thread,
    ::jxx::lang::jlong timeout) const {
    if (thread == nullptr) throw ::jxx::lang::NullPointerException();
    if (timeout <= 0) return;

    const auto millis = toMillis(timeout);
    const auto nanoRemainder = excessNanos_(timeout, millis);
    thread->join(millis, nanoRemainder);
}

} // namespace jxx::util::concurrent
