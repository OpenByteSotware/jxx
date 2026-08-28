#pragma once

#include "lang/jxx.lang.Object.h"

namespace jxx {
namespace util {

template <typename K, typename V>
class MapEntry {
public:
    virtual ~MapEntry() = default;
    virtual jxx::Ptr<K> getKey() = 0;
    virtual jxx::Ptr<V> getValue() = 0;
    virtual jxx::Ptr<V> setValue(const jxx::Ptr<V> value) = 0;
    virtual jxx::lang::jbool equals(const jxx::Ptr<jxx::lang::Object>& o) = 0;
    virtual jxx::lang::jint hashCode() = 0;
};

} // namespace util
} // namespace jxx
