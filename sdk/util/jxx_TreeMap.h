#pragma once
#include <map>
#include <optional>
#include <vector>
#include <stdexcept>
#include <utility>
#include <algorithm>
#include <iterator>
#include <functional>

namespace jxx::util {

    // ---------------------------
    // Java 8-like Map<K,V> interface + Entry
    // ---------------------------
    template <typename Key, typename V>
    class Map {
    public:
        using key_type = Key;
        using value_type = V;
        using value_optional = std::optional<V>;

        // ---- Java-like Entry<K,V> snapshot ----
        // Value type (copyable) that carries key + optional value.
        // - getKey(), getValue(), setValue() (setter updates the Entry snapshot and returns previous).
        // - equals() like Java: keys and values compared with == on Key and std::optional<V>.
        class Entry {
        public:
            Entry() = default;
            Entry(const Key& k, const value_optional& v) : key_(k), val_(v) {}
            Entry(Key&& k, value_optional&& v) : key_(std::move(k)), val_(std::move(v)) {}

            const Key& getKey() const noexcept { return key_; }
            const value_optional& getValue() const noexcept { return val_; }

            // Like Java Map.Entry::setValue: returns previous value.
            value_optional setValue(value_optional newVal) {
                value_optional prev = std::move(val_);
                val_ = std::move(newVal);
                return prev;
            }

            // Equality like Java: key equality && value equality
            bool equals(const Entry& other) const {
                return key_ == other.key_ && val_ == other.val_;
            }

            // C++ convenience
            bool operator==(const Entry& other) const { return equals(other); }
            bool operator!=(const Entry& other) const { return !equals(other); }

        private:
            Key            key_{};
            value_optional val_{};
        };

        using entry_type = Entry;

        virtual ~Map() = default;

        // ---- Core abstract API (pure virtual) ----
        virtual std::size_t size() const noexcept = 0;
        virtual bool isEmpty() const noexcept = 0;        // Java isEmpty()
        virtual bool empty()  const noexcept = 0;         // C++ alias
        virtual void clear() = 0;

        virtual bool containsKey(const Key& key) const = 0;
        virtual bool containsValue(const value_optional& val) const = 0;

        // get returns nullopt if absent OR present-null (Java ambiguity preserved)
        virtual value_optional get(const Key& key) const = 0;

        // put returns previous value (or null) — ambiguity preserved
        virtual value_optional put(const Key& key, value_optional val) = 0;

        // remove by key; returns previous (or null if absent)
        virtual value_optional remove(const Key& key) = 0;

        // Materialized "views"
        virtual std::vector<Key> keySet() const = 0;
        virtual std::vector<value_optional> values() const = 0;
        virtual std::vector<Entry> entrySet() const = 0;

        // ---- Java 8 default-like helpers (virtual with default bodies) ----

        // getOrDefault(key, defaultValue)
        virtual value_optional getOrDefault(const Key& key, value_optional defaultValue) const {
            // If key is present (even with null value), return get(key). Otherwise default.
            if (containsKey(key)) return get(key);
            return defaultValue;
        }

        // putIfAbsent: only put if no mapping exists.
        virtual value_optional putIfAbsent(const Key& key, value_optional value) {
            if (containsKey(key)) return get(key);
            (void)put(key, std::move(value));
            return value_optional{}; // previous is null
        }

        // remove(key, value): remove only if mapped to that value.
        virtual bool remove(const Key& key, const value_optional& value) {
            if (!containsKey(key)) return false;
            if (get(key) == value) { (void)remove(key); return true; }
            return false;
        }

        // replace(key, value): replace mapping if present; returns previous or null
        virtual value_optional replace(const Key& key, value_optional newValue) {
            if (!containsKey(key)) return value_optional{};
            return put(key, std::move(newValue));
        }

        // replace(key, oldValue, newValue): conditional replace
        virtual bool replace(const Key& key, const value_optional& oldValue, value_optional newValue) {
            if (!containsKey(key)) return false;
            if (get(key) == oldValue) { (void)put(key, std::move(newValue)); return true; }
            return false;
        }

        // computeIfAbsent: if no mapping, compute; if function returns null → no insert
        virtual value_optional computeIfAbsent(
            const Key& key,
            const std::function<value_optional(const Key&)>& mappingFunction
        ) {
            if (containsKey(key)) return get(key);
            value_optional nv = mappingFunction(key);
            if (nv.has_value()) { (void)put(key, nv); }
            return nv; // nullopt means no insert
        }

        // computeIfPresent: only if mapping exists (even if value is null)
        // If function returns null → remove mapping
        virtual value_optional computeIfPresent(
            const Key& key,
            const std::function<value_optional(const Key&, const value_optional&)>& remappingFunction
        ) {
            if (!containsKey(key)) return value_optional{};
            value_optional cur = get(key);
            value_optional nv = remappingFunction(key, cur);
            if (!nv.has_value()) { (void)remove(key); return value_optional{}; }
            (void)put(key, nv);
            return nv;
        }

        // compute: always call function with current (or null if absent).
        // If returns null → remove if present; final return is the new value (or null).
        virtual value_optional compute(
            const Key& key,
            const std::function<value_optional(const Key&, const value_optional&)>& remappingFunction
        ) {
            value_optional cur = containsKey(key) ? get(key) : value_optional{};
            value_optional nv = remappingFunction(key, cur);
            if (!nv.has_value()) {
                if (containsKey(key)) (void)remove(key);
                return value_optional{};
            }
            (void)put(key, nv);
            return nv;
        }

        // merge: if absent, put value; else apply remappingFunction(existing, value).
        // If function returns null → remove mapping.
        virtual value_optional merge(
            const Key& key,
            value_optional value,
            const std::function<value_optional(const value_optional&, const value_optional&)>& remappingFunction
        ) {
            if (!containsKey(key)) {
                // In Java, 'value' must be non-null; here we allow nullopt but won't insert nullopt.
                if (value.has_value()) { (void)put(key, value); return value; }
                return value_optional{}; // do nothing on nullopt
            }
            value_optional cur = get(key);
            value_optional nv = remappingFunction(cur, value);
            if (!nv.has_value()) { (void)remove(key); return value_optional{}; }
            (void)put(key, nv);
            return nv;
        }

        // putAll: convenience
        template <typename M>
        void putAll(const M& other) {
            for (const auto& e : other.entrySet()) { (void)put(e.getKey(), e.getValue()); }
        }
    };


    // ---------------------------
    // TreeMap<K,V> implementation (Java 8 style)
    // ---------------------------
    template <typename Key, typename V, typename Compare = std::less<Key>>
    class TreeMap : public Map<Key, V> {
    public:
        using typename Map<Key, V>::value_optional;
        using typename Map<Key, V>::Entry; // snapshot Entry

        using map_type = std::map<Key, value_optional, Compare>;
        using iterator = typename map_type::iterator;
        using const_iterator = typename map_type::const_iterator;
        using const_reverse_iterator = typename map_type::const_reverse_iterator;

        // ---- Live, mutable entry handle (like Java Entry from views/iterators) ----
        // Holds an iterator into the map. setValue() mutates the underlying map.
        class EntryRef {
        public:
            EntryRef() = default;
            explicit EntryRef(iterator it) : it_(it) {}

            bool valid() const noexcept { return it_ != iterator{}; }

            const Key& getKey() const { return it_->first; }
            const value_optional& getValue() const { return it_->second; }

            value_optional setValue(value_optional newVal) {
                value_optional prev = std::move(it_->second);
                it_->second = std::move(newVal);
                return prev;
            }

            // Snapshot copy (useful to return by value)
            Entry snapshot() const { return Entry{ it_->first, it_->second }; }

        private:
            iterator it_{};
        };

        TreeMap() = default;
        explicit TreeMap(Compare comp) : map_(comp) {}

        // ----- Core size/state -----
        std::size_t size() const noexcept override { return map_.size(); }
        bool isEmpty() const noexcept override { return map_.empty(); }
        bool empty()  const noexcept override { return map_.empty(); }
        void clear() override { map_.clear(); }

        // ----- Basic queries -----
        bool containsKey(const Key& key) const override {
            return map_.find(key) != map_.end();
        }

        // O(n) scan, same as Java Map.containsValue
        bool containsValue(const value_optional& val) const override {
            return std::any_of(map_.begin(), map_.end(),
                [&](const auto& kv) { return kv.second == val; });
        }

        // ----- put/get/remove (match TreeMap semantics) -----
        value_optional put(const Key& key, value_optional val) override {
            auto it = map_.lower_bound(key);
            if (it == map_.end() || comp()(key, it->first)) {
                // not present; insert and return null
                map_.emplace(key, std::move(val));
                return value_optional{}; // null
            }
            else {
                // present; swap and return previous
                value_optional prev = std::move(it->second);
                it->second = std::move(val);
                return prev;
            }
        }

        value_optional get(const Key& key) const override {
            auto it = map_.find(key);
            return (it == map_.end()) ? value_optional{} : it->second;
        }

        value_optional remove(const Key& key) override {
            auto it = map_.find(key);
            if (it == map_.end()) return value_optional{};
            value_optional prev = std::move(it->second);
            map_.erase(it);
            return prev;
        }

        // ----- first/last -----
        const Key& firstKey() const {
            if (map_.empty()) throw std::out_of_range("TreeMap.firstKey on empty map");
            return map_.begin()->first;
        }

        const Key& lastKey() const {
            if (map_.empty()) throw std::out_of_range("TreeMap.lastKey on empty map");
            return std::prev(map_.end())->first;
        }

        // ----- Navigational keys (null if none) -----
        std::optional<Key> lowerKey(const Key& k) const {
            auto it = map_.lower_bound(k);
            if (it == map_.begin()) return std::nullopt;
            --it;
            if (comp()(it->first, k)) return it->first;
            return std::nullopt;
        }

        std::optional<Key> floorKey(const Key& k) const {
            auto it = map_.upper_bound(k);
            if (it == map_.begin()) return std::nullopt;
            --it;
            return it->first;
        }

        std::optional<Key> ceilingKey(const Key& k) const {
            auto it = map_.lower_bound(k);
            if (it == map_.end()) return std::nullopt;
            return it->first;
        }

        std::optional<Key> higherKey(const Key& k) const {
            auto it = map_.upper_bound(k);
            if (it == map_.end()) return std::nullopt;
            return it->first;
        }

        // ----- Entry navigation (snapshot Entry) -----
        std::optional<Entry> firstEntry() const {
            if (map_.empty()) return std::nullopt;
            const auto& e = *map_.begin();
            return Entry{ e.first, e.second };
        }

        std::optional<Entry> lastEntry() const {
            if (map_.empty()) return std::nullopt;
            const auto& e = *std::prev(map_.end());
            return Entry{ e.first, e.second };
        }

        std::optional<Entry> lowerEntry(const Key& k) const {
            auto kOpt = lowerKey(k);
            if (!kOpt) return std::nullopt;
            const auto it = map_.find(*kOpt);
            return Entry{ it->first, it->second };
        }

        std::optional<Entry> floorEntry(const Key& k) const {
            auto kOpt = floorKey(k);
            if (!kOpt) return std::nullopt;
            const auto it = map_.find(*kOpt);
            return Entry{ it->first, it->second };
        }

        std::optional<Entry> ceilingEntry(const Key& k) const {
            auto kOpt = ceilingKey(k);
            if (!kOpt) return std::nullopt;
            const auto it = map_.find(*kOpt);
            return Entry{ it->first, it->second };
        }

        std::optional<Entry> higherEntry(const Key& k) const {
            auto kOpt = higherKey(k);
            if (!kOpt) return std::nullopt;
            const auto it = map_.find(*kOpt);
            return Entry{ it->first, it->second };
        }

        // ----- Iteration (ascending and descending) -----
        const_iterator begin()  const noexcept { return map_.begin(); }
        const_iterator end()    const noexcept { return map_.end(); }
        const_iterator cbegin() const noexcept { return map_.cbegin(); }
        const_iterator cend()   const noexcept { return map_.cend(); }

        const_reverse_iterator rbegin() const noexcept { return map_.rbegin(); }
        const_reverse_iterator rend()   const noexcept { return map_.rend(); }

        // ----- KeySet / Values / EntrySet (materialized snapshots) -----
        std::vector<Key> keySet() const override {
            std::vector<Key> keys;
            keys.reserve(map_.size());
            for (const auto& kv : map_) keys.push_back(kv.first);
            return keys;
        }

        std::vector<value_optional> values() const override {
            std::vector<value_optional> vals;
            vals.reserve(map_.size());
            for (const auto& kv : map_) vals.push_back(kv.second);
            return vals;
        }

        std::vector<Entry> entrySet() const override {
            std::vector<Entry> entries;
            entries.reserve(map_.size());
            for (const auto& kv : map_) entries.emplace_back(kv.first, kv.second);
            return entries;
        }

        // ---- Live entry reference retrieval (mutates map like Java's Entry::setValue) ----
        std::optional<EntryRef> findEntryRef(const Key& key) {
            auto it = map_.find(key);
            if (it == map_.end()) return std::nullopt;
            return EntryRef{ it };
        }

        // Expose comparator
        Compare comparator() const { return comp(); }

    private:
        const Compare& comp() const { return map_.key_comp(); }
        map_type map_;
    };

}