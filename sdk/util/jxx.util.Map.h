#pragma once

#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Set.h"
#include "util/jxx.util.Collection.h"
#include "util/jxx.util.MapEntry.h"
#include "util/function/jxx.util.function.BiConsumer.h"
#include "util/function/jxx.util.function.BiFunction.h"
#include "util/function/jxx.util.function.Function.h"

namespace jxx {
namespace util {

template <typename K, typename V>
class Map {
public:
    virtual ~Map() = default;

    virtual jxx::lang::jint size() = 0;
    virtual jxx::lang::jbool isEmpty() { return size() == 0; }
    virtual jxx::lang::jbool containsKey(jxx::Ptr<jxx::lang::Object> key) = 0;
    virtual jxx::lang::jbool containsValue(jxx::Ptr<jxx::lang::Object> value) = 0;
    virtual jxx::Ptr<V> get(jxx::Ptr<jxx::lang::Object> key) = 0;
    virtual jxx::Ptr<V> put(jxx::Ptr<K> key, jxx::Ptr<V> value) = 0;
    virtual jxx::Ptr<V> remove(jxx::Ptr<jxx::lang::Object> key) = 0;
    virtual void putAll(jxx::Ptr<Map<K, V>> m) = 0;
    virtual void clear() = 0;
    virtual jxx::Ptr<Set<K>> keySet() = 0;
    virtual jxx::Ptr<Collection<V>> values() = 0;
    virtual jxx::Ptr<Set<MapEntry<K, V>>> entrySet() = 0;
    virtual jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> o) = 0;
    virtual jxx::lang::jint hashCode() = 0;

    virtual jxx::Ptr<V> getOrDefault(jxx::Ptr<jxx::lang::Object> key, jxx::Ptr<V> defaultValue) {
        jxx::Ptr<V> v = get(key);
        if (v != nullptr || containsKey(key)) return v;
        return defaultValue;
    }

    virtual void forEach(jxx::Ptr<function::BiConsumer<K, V>> action) {
        if (action == nullptr) throw jxx::lang::NullPointerException();
        auto es = entrySet();
        auto it = es->iterator();
        while (it->hasNext()) {
            auto e = it->next();
            action->accept(e->getKey(), e->getValue());
        }
    }

    virtual void replaceAll(jxx::Ptr<function::BiFunction<K, V, V>> function) {
        if (function == nullptr) throw jxx::lang::NullPointerException();
        auto es = entrySet();
        auto it = es->iterator();
        while (it->hasNext()) {
            auto e = it->next();
            e->setValue(function->apply(e->getKey(), e->getValue()));
        }
    }

    virtual jxx::Ptr<V> putIfAbsent(jxx::Ptr<K> key, jxx::Ptr<V> value) {
        jxx::Ptr<V> v = get(key);
        if (v == nullptr) v = put(key, value);
        return v;
    }

    virtual jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> key, jxx::Ptr<jxx::lang::Object> value) {
        jxx::Ptr<V> cur = get(key);
        if (cur == nullptr) return false;
        if (!cur->equals(value)) return false;
        remove(key);
        return true;
    }

    virtual jxx::lang::jbool replace(jxx::Ptr<K> key, jxx::Ptr<V> oldValue, jxx::Ptr<V> newValue) {
        jxx::Ptr<V> cur = get(key);
        if (cur == nullptr || !cur->equals(oldValue)) return false;
        put(key, newValue);
        return true;
    }

    virtual jxx::Ptr<V> replace(jxx::Ptr<K> key, jxx::Ptr<V> value) {
        jxx::Ptr<V> cur = get(key);
        if (cur != nullptr || containsKey(key)) return put(key, value);
        return nullptr;
    }

    virtual jxx::Ptr<V> computeIfAbsent(jxx::Ptr<K> key, jxx::Ptr<function::Function<K, V>> mappingFunction) {
        if (mappingFunction == nullptr) throw jxx::lang::NullPointerException();
        jxx::Ptr<V> v = get(key);
        if (v == nullptr) {
            jxx::Ptr<V> newValue = mappingFunction->apply(key);
            if (newValue != nullptr) {
                put(key, newValue);
                return newValue;
            }
        }
        return v;
    }

    virtual jxx::Ptr<V> computeIfPresent(jxx::Ptr<K> key, jxx::Ptr<function::BiFunction<K, V, V>> remappingFunction) {
        if (remappingFunction == nullptr) throw jxx::lang::NullPointerException();
        jxx::Ptr<V> oldValue = get(key);
        if (oldValue != nullptr) {
            jxx::Ptr<V> newValue = remappingFunction->apply(key, oldValue);
            if (newValue != nullptr) {
                put(key, newValue);
                return newValue;
            }
            remove(key);
        }
        return nullptr;
    }

    virtual jxx::Ptr<V> compute(jxx::Ptr<K> key, jxx::Ptr<function::BiFunction<K, V, V>> remappingFunction) {
        if (remappingFunction == nullptr) throw jxx::lang::NullPointerException();
        jxx::Ptr<V> oldValue = get(key);
        jxx::Ptr<V> newValue = remappingFunction->apply(key, oldValue);
        if (newValue == nullptr) {
            if (oldValue != nullptr || containsKey(key)) remove(key);
            return nullptr;
        }
        put(key, newValue);
        return newValue;
    }

    virtual jxx::Ptr<V> merge(jxx::Ptr<K> key, jxx::Ptr<V> value, jxx::Ptr<function::BiFunction<V, V, V>> remappingFunction) {
        if (value == nullptr || remappingFunction == nullptr) throw jxx::lang::NullPointerException();
        jxx::Ptr<V> oldValue = get(key);
        jxx::Ptr<V> newValue = (oldValue == nullptr) ? value : remappingFunction->apply(oldValue, value);
        if (newValue == nullptr) {
            remove(key);
            return nullptr;
        }
        put(key, newValue);
        return newValue;
    }
};

} // namespace util
} // namespace jxx
