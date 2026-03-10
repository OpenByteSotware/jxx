#pragma once
#include <set>
#include <optional>
#include "jxx.util.NoSuchElementException.h"

namespace jxx { namespace util {

template <typename T, typename Cmp = std::less<T>>
class TreeSet {
public:
    using SetType = std::set<T,Cmp>;

    TreeSet() = default;
    explicit TreeSet(const Cmp& comp) : set_(comp) {}

    bool add(const T& v) { return set_.insert(v).second; }
    bool remove(const T& v) { return set_.erase(v) > 0; }
    bool contains(const T& v) const { return set_.find(v) != set_.end(); }

    int  size() const noexcept { return (int)set_.size(); }
    bool isEmpty() const noexcept { return set_.empty(); }

    T first() const { if (set_.empty()) throw NoSuchElementException("TreeSet.first on empty"); return *set_.begin(); }
    T last()  const { if (set_.empty()) throw NoSuchElementException("TreeSet.last on empty"); auto it=set_.end(); --it; return *it; }

    std::optional<T> higher(const T& v) const { auto it = set_.upper_bound(v); if (it==set_.end()) return std::nullopt; return *it; }
    std::optional<T> lower (const T& v) const { auto it = set_.lower_bound(v); if (it==set_.begin()) return std::nullopt; if (it==set_.end() || *it==v) { if (it==set_.begin()) return std::nullopt; --it; return *it; } else { --it; return *it; } }
    std::optional<T> ceiling(const T& v) const { auto it = set_.lower_bound(v); if (it==set_.end()) return std::nullopt; return *it; }
    std::optional<T> floor  (const T& v) const { auto it = set_.upper_bound(v); if (it==set_.begin()) return std::nullopt; --it; if (*it<=v) return *it; return std::nullopt; }

    const SetType& toStdSet() const { return set_; }

private:
    SetType set_;
};

}} // namespace jxx::util
