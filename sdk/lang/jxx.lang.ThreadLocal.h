#pragma once

#include <mutex>
#include <thread>
#include <unordered_map>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/function/jxx.util.function.Supplier.h"

namespace jxx::lang {

template <typename T>
class ThreadLocal : public ClassBase<ThreadLocal<T>, Object> {
public:
    using JxxSuper = Object;
    using Super = ClassBase<ThreadLocal<T>, JxxSuper>;

    ThreadLocal() = default;
    ~ThreadLocal() override = default;

    jxx::Ptr<T> get() {
        std::lock_guard<std::mutex> lock(mutex_);
        const auto id = std::this_thread::get_id();
        const auto found = values_.find(id);
        if (found != values_.end()) return found->second;
        auto value = initialValue();
        values_[id] = value;
        return value;
    }

    void set(const jxx::Ptr<T>& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        values_[std::this_thread::get_id()] = value;
    }

    void remove() {
        std::lock_guard<std::mutex> lock(mutex_);
        values_.erase(std::this_thread::get_id());
    }

    static jxx::Ptr<ThreadLocal<T>> withInitial(
        const jxx::Ptr<jxx::util::function::Supplier<T>>& supplier) {
        if (supplier == nullptr) throw NullPointerException();
        class SuppliedThreadLocal final : public ThreadLocal<T> {
        public:
            explicit SuppliedThreadLocal(
                const jxx::Ptr<jxx::util::function::Supplier<T>>& value)
                : supplier_(value) {}
        protected:
            jxx::Ptr<T> initialValue() override { return supplier_->get(); }
        private:
            jxx::Ptr<jxx::util::function::Supplier<T>> supplier_;
        };
        return jxx::CAST<ThreadLocal<T>>(jxx::NEW<SuppliedThreadLocal>(supplier));
    }

protected:
    virtual jxx::Ptr<T> initialValue() { return nullptr; }

    jxx::Ptr<T> currentValueIfPresent(jbool& present) {
        std::lock_guard<std::mutex> lock(mutex_);
        const auto found = values_.find(std::this_thread::get_id());
        present = found != values_.end();
        return present ? found->second : nullptr;
    }

    void setForCurrentThread(const jxx::Ptr<T>& value) { set(value); }

private:
    std::mutex mutex_;
    std::unordered_map<std::thread::id, jxx::Ptr<T>> values_;
};

} // namespace jxx::lang
