#pragma once
#include <optional>
#include "lang/jxx.lang.h"

namespace jxx { namespace util {

template <typename MapT>
class UnmodifiableMap {
public:
    explicit UnmodifiableMap(const MapT& m) : map_(&m) {}

    int  size() const noexcept { return map_->size(); }
    bool isEmpty() const noexcept { return map_->isEmpty(); }

    // MapT is expected to provide containsKey / containsValue / get(const K&)
    template <typename K>
    bool containsKey(const K& key) const { return map_->containsKey(key); }

    template <typename V>
    bool containsValue(const V& v) const { return map_->containsValue(v); }

    template <typename K>
    auto get(const K& key) const { return map_->get(key); }

    // Mutators throw
    template <typename K, typename V>
    V put(const K&, const V&) { throw jxx::lang::UnsupportedOperationException("unmodifiable map"); }

    template <typename K>
    auto remove(const K&) { throw jxx::lang::UnsupportedOperationException("unmodifiable map"); }

    void clear() { throw jxx::lang::UnsupportedOperationException("unmodifiable map"); }

    // Access to underlying entries when MapT exposes entrySet()
    const auto& entrySet() const { return map_->entrySet(); }

private:
    const MapT* map_;
};

}} // namespace jxx::util
