#pragma once

#include <algorithm>
#include <memory>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"
#include "lang/ref/jxx.lang.ref.ReferenceQueue.h"
#include "lang/ref/jxx.lang.ref.ReferenceState.h"

namespace jxx::lang::ref {

template <typename T>
class Reference
    : public ::jxx::lang::ClassBase<Reference<T>, ::jxx::lang::Object> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<Reference<T>, JxxSuper>;

    ~Reference() override {
        unregister_();
    }

    virtual ::jxx::Ptr<T> get() const {
        if (state_->cleared.load(std::memory_order_acquire)) return nullptr;
        return strong_ != nullptr ? strong_ : weak_.lock();
    }

    virtual void clear() {
        state_->cleared.store(true, std::memory_order_release);
        strong_.reset();
        weak_.reset();
    }

    ::jxx::lang::jbool isEnqueued() const {
        refreshQueue_();
        return state_->enqueued.load(std::memory_order_acquire);
    }

    ::jxx::lang::jbool enqueue() {
        auto core = queueCore_;
        if (core == nullptr) return false;
        std::lock_guard<std::mutex> lock(core->mutex);
        if (state_->dequeued.load(std::memory_order_acquire) ||
            state_->enqueued.exchange(true, std::memory_order_acq_rel)) {
            return false;
        }
        state_->cleared.store(true, std::memory_order_release);
        strong_.reset();
        weak_.reset();
        core->queued.push_back(this);
        core->changed.notify_all();
        return true;
    }

protected:
    enum class Strength { Weak, Soft, Phantom };

    Reference(const ::jxx::Ptr<T>& referent,
        const ::jxx::Ptr<ReferenceQueue<T>>& queue,
        Strength strength)
        : weak_(referent),
          strong_(strength == Strength::Soft ? referent : nullptr),
          queueCore_(queue == nullptr ? nullptr : queue->core_),
          state_(std::make_shared<detail::ReferenceState>()),
          lifetime_(std::make_shared<int>(0)),
          strength_(strength) {
        if (queueCore_ != nullptr) {
            std::lock_guard<std::mutex> lock(queueCore_->mutex);
            queueCore_->entries.push_back(
                {this, weak_, lifetime_, state_});
        }
    }

    Strength strength() const noexcept {
        return strength_;
    }

private:
    void refreshQueue_() const {
        auto core = queueCore_;
        if (core == nullptr) return;
        std::lock_guard<std::mutex> lock(core->mutex);
        for (auto& entry : core->entries) {
            if (entry.reference != this) continue;
            if (!entry.state->cleared.load(std::memory_order_acquire) &&
                !entry.state->enqueued.load(std::memory_order_acquire) &&
                !entry.state->dequeued.load(std::memory_order_acquire) &&
                entry.referent.expired()) {
                entry.state->cleared.store(true, std::memory_order_release);
                entry.state->enqueued.store(true, std::memory_order_release);
                core->queued.push_back(entry.reference);
                core->changed.notify_all();
            }
            break;
        }
    }

    void unregister_() noexcept {
        auto core = queueCore_;
        if (core == nullptr) return;
        std::lock_guard<std::mutex> lock(core->mutex);
        core->queued.erase(
            std::remove(core->queued.begin(), core->queued.end(), this),
            core->queued.end());
        core->entries.erase(
            std::remove_if(core->entries.begin(), core->entries.end(),
                [this](const typename detail::ReferenceQueueCore<T>::Entry& entry) {
                    return entry.reference == this;
                }),
            core->entries.end());
        lifetime_.reset();
        core->changed.notify_all();
    }

    mutable std::weak_ptr<T> weak_;
    mutable ::jxx::Ptr<T> strong_;
    std::shared_ptr<detail::ReferenceQueueCore<T>> queueCore_;
    std::shared_ptr<detail::ReferenceState> state_;
    std::shared_ptr<void> lifetime_;
    Strength strength_;
};

} // namespace jxx::lang::ref
