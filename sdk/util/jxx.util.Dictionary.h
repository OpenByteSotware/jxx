#pragma once

#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Enumeration.h"

namespace jxx {
namespace util {

template <typename K, typename V>
class Dictionary : virtual public jxx::lang::Object {
public:
    virtual ~Dictionary() = default;
    virtual jxx::lang::jint size() = 0;
    virtual jxx::lang::jbool isEmpty() = 0;
    virtual jxx::Ptr<jxx::util::Enumeration<K>> keys() = 0;
    virtual jxx::Ptr<jxx::util::Enumeration<V>> elements() = 0;
    virtual jxx::Ptr<V> get(jxx::Ptr<jxx::lang::Object> key) = 0;
    virtual jxx::Ptr<V> put(jxx::Ptr<K> key, jxx::Ptr<V> value) = 0;
    virtual jxx::Ptr<V> remove(jxx::Ptr<jxx::lang::Object> key) = 0;
};

} // namespace util
} // namespace jxx
