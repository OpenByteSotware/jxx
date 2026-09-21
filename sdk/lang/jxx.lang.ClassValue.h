#pragma once

#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <limits>
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
                generation = entry.generation;
                if (entry.computingGeneration == generation) {
                    const auto* key = type.get();
                    condition_.wait(lock, [this, key, generation] {
                        const auto found = entries_.find(key);
                        return found == entries_.end() ||
                               found->second.computingGeneration != generation;
                    });
                    continue;
                }
                entry.computingGeneration = generation;
            }

            T computed;
            try {
                computed = computeValue(type);
            }
            catch (...) {
                std::lock_guard<std::mutex> lock(mutex_);
                auto found = entries_.find(type.get());
                if (found != entries_.end() &&
                    found->second.generation == generation &&
                    found->second.computingGeneration == generation) {
                    found->second.computingGeneration = noGeneration_;
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
                    entry.computingGeneration = noGeneration_;
                    condition_.notify_all();
                    return entry.value;
                }
                if (entry.computingGeneration == generation) {
                    entry.computingGeneration = noGeneration_;
                }
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
    static constexpr std::uint64_t noGeneration_ =
        (std::numeric_limits<std::uint64_t>::max)();

    struct Entry final {
        T value{};
        std::uint64_t generation = 0;
        bool initialized = false;
        std::uint64_t computingGeneration = noGeneration_;
    };

    std::mutex mutex_;
    std::condition_variable condition_;
    std::unordered_map<const ClassAny*, Entry> entries_;
};

} // namespace jxx::lang
