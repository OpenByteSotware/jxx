#pragma once

#include "lang/jxx.lang.Object.h"

namespace jxx { template <typename T> class Ptr; }

namespace jxx {
namespace util {

template <typename K, typename V>
class MapEntry : virtual public jxx::lang::Object {
public:
    virtual ~MapEntry() = default;
    virtual jxx::Ptr<K> getKey() = 0;
    virtual jxx::Ptr<V> getValue() = 0;
    virtual jxx::Ptr<V> setValue(jxx::Ptr<V> value) = 0;
    virtual jbool equals(jxx::Ptr<jxx::lang::Object> o) override = 0;
    virtual jint hashCode() override = 0;
};

} // namespace util
} // namespace jxx
