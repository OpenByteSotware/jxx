#include <limits>
#include <thread>

#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace jxx::util::concurrent {

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

#define UNIT_FACTORY(NAME) ::jxx::Ptr<TimeUnit> TimeUnit::NAME() { static auto value = ::jxx::Ptr<TimeUnit>(new TimeUnit(Kind::NAME)); return value; }
UNIT_FACTORY(NANOSECONDS)
UNIT_FACTORY(MICROSECONDS)
UNIT_FACTORY(MILLISECONDS)
UNIT_FACTORY(SECONDS)
UNIT_FACTORY(MINUTES)
UNIT_FACTORY(HOURS)
UNIT_FACTORY(DAYS)
#undef UNIT_FACTORY

::jxx::lang::jlong TimeUnit::toNanos(::jxx::lang::jlong duration) const { return saturatedMultiply(duration, factor(kind_)); }
::jxx::lang::jlong TimeUnit::convert(::jxx::lang::jlong duration, const ::jxx::Ptr<TimeUnit>& source) const {
    if (source == nullptr) throw ::jxx::lang::NullPointerException();
    const auto nanos = source->toNanos(duration);
    return nanos / factor(kind_);
}
::jxx::lang::jlong TimeUnit::toMicros(::jxx::lang::jlong d) const { return toNanos(d) / 1000LL; }
::jxx::lang::jlong TimeUnit::toMillis(::jxx::lang::jlong d) const { return toNanos(d) / 1000000LL; }
::jxx::lang::jlong TimeUnit::toSeconds(::jxx::lang::jlong d) const { return toNanos(d) / 1000000000LL; }
::jxx::lang::jlong TimeUnit::toMinutes(::jxx::lang::jlong d) const { return toSeconds(d) / 60LL; }
::jxx::lang::jlong TimeUnit::toHours(::jxx::lang::jlong d) const { return toMinutes(d) / 60LL; }
::jxx::lang::jlong TimeUnit::toDays(::jxx::lang::jlong d) const { return toHours(d) / 24LL; }
std::chrono::nanoseconds TimeUnit::toChrono(::jxx::lang::jlong d) const { return std::chrono::nanoseconds(toNanos(d)); }
void TimeUnit::sleep(::jxx::lang::jlong timeout) const { if (timeout > 0) std::this_thread::sleep_for(toChrono(timeout)); }
void TimeUnit::timedJoin(const ::jxx::Ptr<::jxx::lang::Thread>& thread, ::jxx::lang::jlong timeout) const {
    if (thread == nullptr) throw ::jxx::lang::NullPointerException();
    if (timeout > 0) thread->join(toMillis(timeout));
}

} // namespace jxx::util::concurrent
