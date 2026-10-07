#pragma once

#include <condition_variable>
#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"
#include "util/jxx.util.concurrent.TimeUnit.h"
#include "util/jxx.util.concurrent.TimeoutException.h"

namespace jxx::util::concurrent {

template<typename V>
class Exchanger final
    : public ::jxx::lang::ClassBase<Exchanger<V>, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<Exchanger<V>, JxxSuper>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    static ::jxx::Ptr<::jxx::lang::ClassAny> Class() {
        return JxxClassInfoMarker::Class();
    }

    Exchanger() : Super() {}

    ::jxx::Ptr<V> exchange(const ::jxx::Ptr<V>& value) {
        return exchange_(value, nullptr);
    }

    ::jxx::Ptr<V> exchange(const ::jxx::Ptr<V>& value,
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) {
        if (unit == nullptr) throw ::jxx::lang::NullPointerException();
        const auto duration = unit->toChrono(timeout);
        return exchange_(value, &duration);
    }

private:
    ::jxx::Ptr<V> exchange_(const ::jxx::Ptr<V>& value,
        const std::chrono::nanoseconds* timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        const auto generation = generation_;
        if (!occupied_) {
            occupied_ = true;
            first_ = value;
            if (timeout == nullptr) {
                condition_.wait(lock, [&] { return generation_ != generation; });
            } else if (!condition_.wait_for(lock, *timeout,
                    [&] { return generation_ != generation; })) {
                occupied_ = false;
                first_.reset();
                throw TimeoutException();
            }
            return second_;
        }

        const auto result = first_;
        second_ = value;
        occupied_ = false;
        first_.reset();
        ++generation_;
        condition_.notify_all();
        return result;
    }

    std::mutex mutex_;
    std::condition_variable condition_;
    ::jxx::Ptr<V> first_;
    ::jxx::Ptr<V> second_;
    ::jxx::lang::jlong generation_ = 0;
    ::jxx::lang::jbool occupied_ = false;
};

} // namespace jxx::util::concurrent
