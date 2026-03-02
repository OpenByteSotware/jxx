#pragma once

#include <unordered_map>
#include <optional>
#include <vector>
#include <stdexcept>
#include <mutex>
#include <utility>
#include <type_traits>
#include <functional>
#include <cstddef>

namespace jxx::util {

        // Java-like Dictionary<K,V> abstract base class (Java 8 compatible shape).
        // We use std::optional<V> to represent the Java notion of "null" returns.
        //
        // Methods:
        //  - size(), isEmpty()
        //  - get(key) -> optional<V>
        //  - put(key, value) -> previous optional<V>
        //  - remove(key) -> previous optional<V>
        //  - keys() -> vector<Key> (snapshot)
        //  - elements() -> vector<V> (snapshot; for impls that allow null values, you'd
        //    need a different shape; Hashtable forbids null so this is fine).
        template <typename Key, typename V>
        class Dictionary {
        public:
            using value_optional = std::optional<V>;

            virtual ~Dictionary() = default;

            // Core
            virtual std::size_t size() const noexcept = 0;
            virtual bool isEmpty() const noexcept = 0;

            // CRUD
            virtual value_optional get(const Key& key) const = 0;
            virtual value_optional put(const Key& key, value_optional value) = 0;
            virtual value_optional remove(const Key& key) = 0;

            // Snapshots
            virtual std::vector<Key> keys() const = 0;
            virtual std::vector<V>   elements() const = 0;
        };

    // Java 8-like Hashtable<K,V>:
    // - Synchronized via std::mutex (coarse-grained, like Java's synchronized methods).
    // - Disallows null keys and null values (enforced at runtime).
    // - Inherits Dictionary<K,V>, implements Map<K,V> + Map::Entry support (snapshots).
    template <
        typename Key,
        typename V,
        typename Hash = std::hash<Key>,
        typename KeyEq = std::equal_to<Key>
    >
    class Hashtable : public Dictionary<Key, V>, public Map<Key, V> {
    public:
        using value_optional = typename Map<Key, V>::value_optional;
        using Entry = typename Map<Key, V>::Entry;

        using map_type = std::unordered_map<Key, V, Hash, KeyEq>;

        // ---- Constructors (approximate Java semantics) ----
        // Java Hashtable default: initialCapacity=11, loadFactor=0.75
        explicit Hashtable(std::size_t initialCapacity = 11, float loadFactor = 0.75f)
        {
            if (loadFactor <= 0.0f) throw std::invalid_argument("loadFactor must be > 0");
            std::lock_guard<std::mutex> g(mutex_);
            table_.max_load_factor(loadFactor);
            table_.reserve(initialCapacity);
        }

        template <typename M>
        explicit Hashtable(const M& other, std::size_t initialCapacity = 11, float loadFactor = 0.75f)
            : Hashtable(initialCapacity, loadFactor)
        {
            this->putAll(other); // Map<K,V> default helper (we override where needed)
        }

        ~Hashtable() override = default;

        // ---- Core size/state ----
        std::size_t size() const noexcept override {
            std::lock_guard<std::mutex> g(mutex_);
            return table_.size();
        }

        bool isEmpty() const noexcept override {
            std::lock_guard<std::mutex> g(mutex_);
            return table_.empty();
        }

        bool empty() const noexcept override { return isEmpty(); } // Map alias

        void clear() override {
            std::lock_guard<std::mutex> g(mutex_);
            table_.clear();
        }

        // ---- Basic queries ----
        bool containsKey(const Key& key) const override {
            checkNullKey(key);
            std::lock_guard<std::mutex> g(mutex_);
            return table_.find(key) != table_.end();
        }

        // Java Hashtable.contains(Object value) — linear scan
        bool containsValue(const value_optional& val) const override {
            // In Hashtable, null values are not allowed, so containsValue(null) is always false.
            if (!val.has_value()) return false;
            std::lock_guard<std::mutex> g(mutex_);
            for (const auto& kv : table_) {
                if (kv.second == *val) return true;
            }
            return false;
        }

        // Legacy alias like Java's Hashtable.contains(Object value)
        bool contains(const value_optional& val) const {
            return containsValue(val);
        }

        // ---- get/put/remove ----
        // get: returns nullopt when absent (Hashtable cannot store null values).
        value_optional get(const Key& key) const override {
            checkNullKey(key);
            std::lock_guard<std::mutex> g(mutex_);
            auto it = table_.find(key);
            if (it == table_.end()) return value_optional{};
            return it->second; // present
        }

        // put: returns previous value (or null if absent); null values are forbidden.
        value_optional put(const Key& key, value_optional val) override {
            checkNullKey(key);
            checkNullValue(val);
            std::lock_guard<std::mutex> g(mutex_);
            auto it = table_.find(key);
            if (it == table_.end()) {
                table_.emplace(key, *val);
                return value_optional{}; // previous null
            }
            else {
                value_optional prev = it->second;
                it->second = *val;
                return prev;
            }
        }

        // remove: returns previous value (or null if absent)
        value_optional remove(const Key& key) override {
            checkNullKey(key);
            std::lock_guard<std::mutex> g(mutex_);
            auto it = table_.find(key);
            if (it == table_.end()) return value_optional{};
            value_optional prev = it->second;
            table_.erase(it);
            return prev;
        }

        // ---- Views (materialized snapshots) ----
        // Map<K,V> snapshots:
        std::vector<Key> keySet() const override {
            std::lock_guard<std::mutex> g(mutex_);
            std::vector<Key> out;
            out.reserve(table_.size());
            for (const auto& kv : table_) out.push_back(kv.first);
            return out;
        }

        std::vector<value_optional> values() const override {
            std::lock_guard<std::mutex> g(mutex_);
            std::vector<value_optional> out;
            out.reserve(table_.size());
            for (const auto& kv : table_) out.emplace_back(kv.second);
            return out;
        }

        std::vector<Entry> entrySet() const override {
            std::lock_guard<std::mutex> g(mutex_);
            std::vector<Entry> out;
            out.reserve(table_.size());
            for (const auto& kv : table_) out.emplace_back(kv.first, value_optional{ kv.second });
            return out;
        }

        // Dictionary<K,V> snapshots:
        std::vector<Key> keys() const override { return keySet(); }

        std::vector<V> elements() const override {
            std::lock_guard<std::mutex> g(mutex_);
            std::vector<V> out;
            out.reserve(table_.size());
            for (const auto& kv : table_) out.push_back(kv.second);
            return out;
        }

        // ---- Map default-like helpers (OVERRIDDEN for atomicity & non-null rules) ----

        // getOrDefault: synchronized wrapper, returns default if absent
        value_optional getOrDefault(const Key& key, value_optional defaultValue) const override {
            checkNullKey(key);
            std::lock_guard<std::mutex> g(mutex_);
            auto it = table_.find(key);
            if (it != table_.end()) return it->second;
            return defaultValue; // may be nullopt
        }

        // putIfAbsent: atomic
        value_optional putIfAbsent(const Key& key, value_optional value) override {
            checkNullKey(key);
            checkNullValue(value);
            std::lock_guard<std::mutex> g(mutex_);
            auto it = table_.find(key);
            if (it != table_.end()) return it->second;
            table_.emplace(key, *value);
            return value_optional{}; // previous null
        }

        // remove(key, value): atomic conditional remove
        bool remove(const Key& key, const value_optional& value) override {
            checkNullKey(key);
            std::lock_guard<std::mutex> g(mutex_);
            auto it = table_.find(key);
            if (it == table_.end()) return false;
            if (value.has_value() && it->second == *value) {
                table_.erase(it);
                return true;
            }
            return false;
        }

        // replace(key, value): atomic; value must be non-null
        value_optional replace(const Key& key, value_optional newValue) override {
            checkNullKey(key);
            checkNullValue(newValue);
            std::lock_guard<std::mutex> g(mutex_);
            auto it = table_.find(key);
            if (it == table_.end()) return value_optional{};
            value_optional prev = it->second;
            it->second = *newValue;
            return prev;
        }

        // replace(key, oldValue, newValue): atomic; newValue must be non-null
        bool replace(const Key& key, const value_optional& oldValue, value_optional newValue) override {
            checkNullKey(key);
            checkNullValue(newValue);
            if (!oldValue.has_value()) return false; // cannot match null in Hashtable
            std::lock_guard<std::mutex> g(mutex_);
            auto it = table_.find(key);
            if (it == table_.end()) return false;
            if (it->second == *oldValue) {
                it->second = *newValue;
                return true;
            }
            return false;
        }

        // computeIfAbsent: atomic; if mappingFunction returns null → no insert
        value_optional computeIfAbsent(
            const Key& key,
            const std::function<value_optional(const Key&)>& mappingFunction
        ) override {
            checkNullKey(key);
            std::lock_guard<std::mutex> g(mutex_);
            auto it = table_.find(key);
            if (it != table_.end()) return it->second;
            value_optional nv = mappingFunction(key); // executed under lock (synchronized)
            if (!nv.has_value()) return value_optional{};
            table_.emplace(key, *nv);
            return nv;
        }

        // computeIfPresent: atomic; if remapping returns null → remove
        value_optional computeIfPresent(
            const Key& key,
            const std::function<value_optional(const Key&, const value_optional&)>& remappingFunction
        ) override {
            checkNullKey(key);
            std::lock_guard<std::mutex> g(mutex_);
            auto it = table_.find(key);
            if (it == table_.end()) return value_optional{};
            value_optional cur = it->second;
            value_optional nv = remappingFunction(key, cur); // under lock
            if (!nv.has_value()) {
                table_.erase(it);
                return value_optional{};
            }
            it->second = *nv;
            return nv;
        }

        // compute: atomic; if result null → remove (if present)
        value_optional compute(
            const Key& key,
            const std::function<value_optional(const Key&, const value_optional&)>& remappingFunction
        ) override {
            checkNullKey(key);
            std::lock_guard<std::mutex> g(mutex_);
            auto it = table_.find(key);
            value_optional cur = (it == table_.end()) ? value_optional{} : value_optional{ it->second };
            value_optional nv = remappingFunction(key, cur); // under lock
            if (!nv.has_value()) {
                if (it != table_.end()) table_.erase(it);
                return value_optional{};
            }
            if (it == table_.end()) {
                table_.emplace(key, *nv);
            }
            else {
                it->second = *nv;
            }
            return nv;
        }

        // merge: atomic; value must be non-null; remapper may return null → remove
        value_optional merge(
            const Key& key,
            value_optional value,
            const std::function<value_optional(const value_optional&, const value_optional&)>& remappingFunction
        ) override {
            checkNullKey(key);
            checkNullValue(value); // Java Hashtable forbids merging with null value
            std::lock_guard<std::mutex> g(mutex_);
            auto it = table_.find(key);
            if (it == table_.end()) {
                table_.emplace(key, *value);
                return value;
            }
            value_optional cur = it->second;
            value_optional nv = remappingFunction(cur, value); // under lock
            if (!nv.has_value()) {
                table_.erase(it);
                return value_optional{};
            }
            it->second = *nv;
            return nv;
        }

        // putAll: synchronized copy
        template <typename M>
        void putAll(const M& other) {
            std::lock_guard<std::mutex> g(mutex_);
            for (const auto& e : other.entrySet()) {
                checkNullKey(e.getKey());
                checkNullValue(e.getValue());
                table_[e.getKey()] = *(e.getValue());
            }
        }

        // ---- Capacity/load helpers (optional) ----
        void setLoadFactor(float lf) {
            if (lf <= 0.0f) throw std::invalid_argument("loadFactor must be > 0");
            std::lock_guard<std::mutex> g(mutex_);
            table_.max_load_factor(lf);
        }

        float loadFactor() const {
            std::lock_guard<std::mutex> g(mutex_);
            return table_.max_load_factor();
        }

        void ensureCapacity(std::size_t expectedSize) {
            std::lock_guard<std::mutex> g(mutex_);
            table_.reserve(expectedSize);
        }

    private:
        // Helper: enforce "no null" constraints similar to Java Hashtable.
        static void checkNullValue(const value_optional& v) {
            if (!v.has_value()) throw std::invalid_argument("Hashtable does not permit null values");
        }

        static void checkNullKey(const Key& key) {
            if constexpr (std::is_pointer<Key>::value) {
                if (key == nullptr) throw std::invalid_argument("Hashtable does not permit null keys");
            }
            // (Optional) add checks for smart pointers if you use them as keys.
        }

        mutable std::mutex mutex_;
        map_type table_;
    };

} // namespace j8