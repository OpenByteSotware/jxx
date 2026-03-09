
#pragma once
#include <unordered_map>
#include <optional>
#include "jxx.util.Iterator.h"

namespace jxx { namespace util {

template <typename K, typename V>
class HashMap {
public:
    HashMap() = default;

    int size() const noexcept { return static_cast<int>(map_.size()); }
    bool isEmpty() const noexcept { return map_.empty(); }

    std::optional<V> get(const K& key) const {
        auto it = map_.find(key); if (it==map_.end()) return std::nullopt; return it->second;
    }
    bool containsKey(const K& key) const { return map_.find(key)!=map_.end(); }
    bool containsValue(const V& value) const { for (auto& kv : map_) if (kv.second==value) return true; return false; }

    V put(const K& key, const V& value) {
        auto it = map_.find(key); V old{}; bool haveOld=false;
        if (it!=map_.end()) { old = it->second; haveOld=true; it->second = value; }
        else map_.emplace(key, value);
        return haveOld?old:V{};
    }

    std::optional<V> remove(const K& key) {
        auto it = map_.find(key); if (it==map_.end()) return std::nullopt; V old = it->second; map_.erase(it); return old;
    }

    void clear() { map_.clear(); }

    const std::unordered_map<K,V>& entrySet() const { return map_; }

private:
    std::unordered_map<K,V> map_;
};

}} // namespace jxx::util
