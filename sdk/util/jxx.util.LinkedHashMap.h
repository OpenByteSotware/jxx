#pragma once
#include <list>
#include <unordered_map>
#include <optional>
#include <utility>

namespace jxx { namespace util {

template <typename K, typename V, typename Hash = std::hash<K>, typename Eq = std::equal_to<K>>
class LinkedHashMap {
public:
    using Node = std::pair<K,V>;
    using List = std::list<Node>;

    explicit LinkedHashMap(bool accessOrder=false) : accessOrder_(accessOrder) {}

    int  size() const noexcept { return (int)order_.size(); }
    bool isEmpty() const noexcept { return order_.empty(); }

    bool containsKey(const K& key) const { return index_.find(key) != index_.end(); }
    bool containsValue(const V& value) const { for (auto& kv : order_) if (kv.second==value) return true; return false; }

    std::optional<V> get(const K& key) {
        auto it = index_.find(key); if (it==index_.end()) return std::nullopt;
        auto lit = it->second;
        if (accessOrder_) { // move to end on access
            order_.splice(order_.end(), order_, lit);
            it->second = std::prev(order_.end());
        }
        return lit->second;
    }

    V put(const K& key, const V& value) {
        auto it = index_.find(key);
        if (it == index_.end()) {
            order_.emplace_back(key, value);
            index_[key] = std::prev(order_.end());
            return V{};
        } else {
            V old = it->second->second;
            it->second->second = value;
            if (accessOrder_) {
                order_.splice(order_.end(), order_, it->second);
                it->second = std::prev(order_.end());
            }
            return old;
        }
    }

    std::optional<V> remove(const K& key) {
        auto it = index_.find(key); if (it==index_.end()) return std::nullopt;
        V old = it->second->second;
        order_.erase(it->second);
        index_.erase(it);
        return old;
    }

    void clear() { index_.clear(); order_.clear(); }

    // Iteration support
    const List& entryList() const { return order_; }

private:
    bool accessOrder_ = false;
    List order_;
    std::unordered_map<K, typename List::iterator, Hash, Eq> index_;
};

}} // namespace jxx::util
