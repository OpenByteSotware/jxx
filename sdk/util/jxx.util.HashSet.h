
#pragma once
#include <unordered_set>

namespace jxx { namespace util {

template <typename T>
class HashSet {
public:
    HashSet() = default;
    int size() const noexcept { return static_cast<int>(set_.size()); }
    bool isEmpty() const noexcept { return set_.empty(); }
    bool add(const T& v) { return set_.insert(v).second; }
    bool contains(const T& v) const { return set_.find(v)!=set_.end(); }
    bool remove(const T& v) { return set_.erase(v)>0; }
    void clear() { set_.clear(); }
private:
    std::unordered_set<T> set_;
};

}} // namespace jxx::util
