#pragma once

#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <unordered_map>

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::lang {

template<typename T>
class ClassValue
    : public ClassBase<ClassValue<T>, Object> {
public:
    using JxxSuper = Object;
    using Super = ClassBase<ClassValue<T>, JxxSuper>;

    virtual ~ClassValue() = default;

    T get(const jxx::Ptr<ClassAny>& type) {
        if (type == nullptr) {
            throw NullPointerException();
        }

        for (;;) {
            std::uint64_t generation = 0;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                auto& entry = entries_[type.get()];
                if (entry.initialized) {
                    return entry.value;
                }
                if (entry.computing) {
                    condition_.wait(lock, [&entry] {
                        return !entry.computing;
                    });
                    continue;
                }
                entry.computing = true;
                generation = entry.generation;
            }

            T computed;
            try {
                computed = computeValue(type);
            }
            catch (...) {
                std::lock_guard<std::mutex> lock(mutex_);
                auto found = entries_.find(type.get());
                if (found != entries_.end() &&
                    found->second.generation == generation) {
                    found->second.computing = false;
                }
                condition_.notify_all();
                throw;
            }

            {
                std::lock_guard<std::mutex> lock(mutex_);
                auto& entry = entries_[type.get()];
                if (entry.generation == generation) {
                    entry.value = computed;
                    entry.initialized = true;
                    entry.computing = false;
                    condition_.notify_all();
                    return entry.value;
                }
                entry.computing = false;
                condition_.notify_all();
            }
        }
    }

    void remove(const jxx::Ptr<ClassAny>& type) {
        if (type == nullptr) {
            throw NullPointerException();
        }
        std::lock_guard<std::mutex> lock(mutex_);
        auto& entry = entries_[type.get()];
        ++entry.generation;
        entry.initialized = false;
        condition_.notify_all();
    }

protected:
    ClassValue()
        : Super() {
    }

    virtual T computeValue(
        const jxx::Ptr<ClassAny>& type) = 0;

private:
    struct Entry final {
        T value{};
        std::uint64_t generation = 0;
        bool initialized = false;
        bool computing = false;
    };

    std::mutex mutex_;
    std::condition_variable condition_;
    std::unordered_map<const ClassAny*, Entry> entries_;
};

} // namespace jxx::lang
