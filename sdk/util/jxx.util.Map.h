#pragma once

#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Set.h"
#include "util/jxx.util.Collection.h"
#include "util/jxx.util.MapEntry.h"
#include "util/function/jxx.util.function.BiConsumer.h"
#include "util/function/jxx.util.function.BiFunction.h"
#include "util/function/jxx.util.function.Function.h"
#include "lang/jxx.lang.ClassInfo.h"

namespace jxx {
namespace util {

template <typename K, typename V>
class Map : public jxx::lang::InterfaceBase<Map<K,V>>
{
public:
    virtual ~Map() = default;

    virtual jxx::lang::jint size() = 0;
    virtual jxx::lang::jbool isEmpty() { return size() == 0; }
    virtual jxx::lang::jbool containsKey(const jxx::Ptr<jxx::lang::Object>& key) = 0;
    virtual jxx::lang::jbool containsValue(const jxx::Ptr<jxx::lang::Object>& value) = 0;
    virtual jxx::Ptr<V> get(const jxx::Ptr<jxx::lang::Object>& key) = 0;
    virtual jxx::Ptr<V> put(const jxx::Ptr<K>& key, const jxx::Ptr<V>& value) = 0;
    virtual jxx::Ptr<V> remove(const jxx::Ptr<jxx::lang::Object>& key) = 0;
    virtual void putAll(const jxx::Ptr<Map<K, V>>& m) = 0;
    virtual void clear() = 0;
    virtual jxx::Ptr<Set<K>> keySet() = 0;
    virtual jxx::Ptr<Collection<V>> values() = 0;
    virtual jxx::Ptr<Set<MapEntry<K, V>>> entrySet() = 0;
    virtual jxx::lang::jbool equals(const jxx::Ptr<jxx::lang::Object>& o) const = 0;
    virtual jxx::lang::jint hashCode() const = 0;

    virtual jxx::Ptr<V> getOrDefault(const jxx::Ptr<jxx::lang::Object>& key, const jxx::Ptr<V>& defaultValue) {
        jxx::Ptr<V> v = get(key);
        if (v != nullptr || containsKey(key)) return v;
        return defaultValue;
    }

    virtual void forEach(const jxx::Ptr<function::BiConsumer<K, V>>& action) {
        if (action == nullptr) throw jxx::lang::NullPointerException();
        auto es = entrySet();
        auto it = es->iterator();
        while (it->hasNext()) {
            auto e = it->next();
            action->accept(e->getKey(), e->getValue());
        }
    }

    virtual void replaceAll(const jxx::Ptr<function::BiFunction<K, V, V>>& function) {
        if (function == nullptr) throw jxx::lang::NullPointerException();
        auto es = entrySet();
        auto it = es->iterator();
        while (it->hasNext()) {
            auto e = it->next();
            e->setValue(function->apply(e->getKey(), e->getValue()));
        }
    }

    virtual jxx::Ptr<V> putIfAbsent(const jxx::Ptr<K>& key, const jxx::Ptr<V>& value) {
        jxx::Ptr<V> v = get(jxx::CAST<jxx::lang::Object>(key));
        if (v == nullptr) v = put(key, value);
        return v;
    }

    virtual jxx::lang::jbool remove(
        const jxx::Ptr<jxx::lang::Object>& key,
        const jxx::Ptr<jxx::lang::Object>& value) {

        auto current = get(key);

        if (current == nullptr) {
            if (value != nullptr || !containsKey(key)) {
                return false;
            }
        }
        else {
            auto currentObject =
                jxx::CAST<jxx::lang::Object>(current);

            if (currentObject == nullptr ||
                !currentObject->equals(value)) {

                return false;
            }
        }

        remove(key);
        return true;
    }

    virtual jxx::lang::jbool replace(
        const jxx::Ptr<K>& key,
        const jxx::Ptr<V>& oldValue,
        const jxx::Ptr<V>& newValue) {

        auto objectKey =
            jxx::CAST<jxx::lang::Object>(key);

        auto current = get(objectKey);

        if (current == nullptr) {
            if (oldValue != nullptr || !containsKey(objectKey)) {
                return false;
            }
        }
        else {
            auto currentObject =
                jxx::CAST<jxx::lang::Object>(current);

            auto oldObject =
                jxx::CAST<jxx::lang::Object>(oldValue);

            if (currentObject == nullptr ||
                !currentObject->equals(oldObject)) {

                return false;
            }
        }

        put(key, newValue);
        return true;
    }

    virtual jxx::Ptr<V> replace(const jxx::Ptr<K>& key, const jxx::Ptr<V>& value) {
        jxx::Ptr<V> cur = get(key);
        if (cur != nullptr || containsKey(key)) return put(key, value);
        return nullptr;
    }

    virtual jxx::Ptr<V> computeIfAbsent(const jxx::Ptr<K>& key, const jxx::Ptr<function::Function<K, V>>& mappingFunction) {
        if (mappingFunction == nullptr) throw jxx::lang::NullPointerException();
        jxx::Ptr<V> v = get(jxx::CAST<jxx::lang::Object>(key));
        if (v == nullptr) {
            jxx::Ptr<V> newValue = mappingFunction->apply(key);
            if (newValue != nullptr) {
                put(key, newValue);
                return newValue;
            }
        }
        return v;
    }

    virtual jxx::Ptr<V> computeIfPresent(const jxx::Ptr<K>& key, const jxx::Ptr<function::BiFunction<K, V, V>>& remappingFunction) {
        if (remappingFunction == nullptr) throw jxx::lang::NullPointerException();
        jxx::Ptr<V> oldValue = get(jxx::CAST<jxx::lang::Object>(key));
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

    virtual jxx::Ptr<V> compute(const jxx::Ptr<K>& key, const jxx::Ptr<function::BiFunction<K, V, V>>& remappingFunction) {
        if (remappingFunction == nullptr) throw jxx::lang::NullPointerException();
        jxx::Ptr<V> oldValue = get(jxx::CAST<jxx::lang::Object>(key));
        jxx::Ptr<V> newValue = remappingFunction->apply(key, oldValue);
        if (newValue == nullptr) {
            if (oldValue != nullptr || containsKey(key)) remove(key);
            return nullptr;
        }
        put(key, newValue);
        return newValue;
    }

    virtual jxx::Ptr<V> merge(const jxx::Ptr<K>& key, const jxx::Ptr<V>& value, const jxx::Ptr<function::BiFunction<V, V, V>>& remappingFunction) {
        if (value == nullptr || remappingFunction == nullptr) throw jxx::lang::NullPointerException();
        jxx::Ptr<V> oldValue = get(jxx::CAST<jxx::lang::Object>(key));
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
