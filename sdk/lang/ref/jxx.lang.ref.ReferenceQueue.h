#pragma once

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"
#include "lang/ref/jxx.lang.ref.ReferenceState.h"

namespace jxx::lang::ref {

template <typename T>
class Reference;

namespace detail {

template <typename T>
struct ReferenceQueueCore final {
    struct Entry final {
        Reference<T>* reference = nullptr;
        std::weak_ptr<T> referent;
        std::weak_ptr<void> lifetime;
        std::shared_ptr<ReferenceState> state;
    };

    std::mutex mutex;
    std::condition_variable changed;
    std::vector<Entry> entries;
    std::vector<Reference<T>*> queued;
};

} // namespace detail

template <typename T>
class ReferenceQueue
    : public ::jxx::lang::ClassBase<
          ReferenceQueue<T>, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<ReferenceQueue<T>, JxxSuper>;

    ReferenceQueue()
        : core_(std::make_shared<detail::ReferenceQueueCore<T>>()) {
    }

    ::jxx::Ptr<Reference<T>> poll() {
        std::lock_guard<std::mutex> lock(core_->mutex);
        refreshLocked_();
        return takeLocked_();
    }

    ::jxx::Ptr<Reference<T>> remove() {
        return remove(static_cast<::jxx::lang::jlong>(0));
    }

    ::jxx::Ptr<Reference<T>> remove(::jxx::lang::jlong timeout) {
        if (timeout < 0) {
            throw ::jxx::lang::IllegalArgumentException("timeout < 0");
        }
        std::unique_lock<std::mutex> lock(core_->mutex);
        const auto deadline = timeout == 0
            ? std::chrono::steady_clock::time_point::max()
            : std::chrono::steady_clock::now() +
                  std::chrono::milliseconds(timeout);
        for (;;) {
            refreshLocked_();
            if (auto value = takeLocked_()) return value;
            if (timeout != 0 && std::chrono::steady_clock::now() >= deadline) {
                return nullptr;
            }
            const auto slice = std::chrono::milliseconds(10);
            if (timeout == 0) {
                core_->changed.wait_for(lock, slice);
            } else {
                core_->changed.wait_until(lock,
                    std::min(deadline, std::chrono::steady_clock::now() + slice));
            }
        }
    }

private:
    friend class Reference<T>;
    using Core = detail::ReferenceQueueCore<T>;

    void refreshLocked_() {
        auto& entries = core_->entries;
        for (auto it = entries.begin(); it != entries.end();) {
            if (it->lifetime.expired()) {
                it = entries.erase(it);
                continue;
            }
            if (!it->state->cleared.load(std::memory_order_acquire) &&
                !it->state->enqueued.load(std::memory_order_acquire) &&
                !it->state->dequeued.load(std::memory_order_acquire) &&
                it->referent.expired()) {
                it->state->cleared.store(true, std::memory_order_release);
                it->state->enqueued.store(true, std::memory_order_release);
                core_->queued.push_back(it->reference);
            }
            ++it;
        }
    }

    ::jxx::Ptr<Reference<T>> takeLocked_() {
        while (!core_->queued.empty()) {
            auto* raw = core_->queued.front();
            core_->queued.erase(core_->queued.begin());
            auto found = std::find_if(core_->entries.begin(), core_->entries.end(),
                [raw](const typename Core::Entry& entry) {
                    return entry.reference == raw && !entry.lifetime.expired();
                });
            if (found == core_->entries.end()) continue;
            found->state->enqueued.store(false, std::memory_order_release);
            found->state->dequeued.store(true, std::memory_order_release);
            return ::jxx::CAST<Reference<T>>(raw->thisPtr());
        }
        return nullptr;
    }

    std::shared_ptr<Core> core_;
};

} // namespace jxx::lang::ref
