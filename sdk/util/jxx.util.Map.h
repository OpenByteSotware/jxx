#pragma once

#include "util/jxx.util.Collection.h"
#include "util/jxx.util.MapEntry.h"
#include "util/jxx.util.Set.h"

namespace jxx::util
{
    template<typename K, typename V>
    class Map
    {
    protected:
        Map() = default;

    public:
        virtual ~Map() = default;

    public:
        virtual jxx::lang::jint size() const = 0;
        virtual jxx::lang::jbool isEmpty() const = 0;
        virtual jxx::lang::jbool containsKey(jxx::Ptr<K> key) const = 0;
        virtual jxx::lang::jbool containsValue(jxx::Ptr<V> value) const = 0;
        virtual jxx::Ptr<V> get(jxx::Ptr<K> key) const = 0;
        virtual jxx::Ptr<V> put(jxx::Ptr<K> key, jxx::Ptr<V> value) = 0;
        virtual void putAll(jxx::Ptr<Map<K, V>> m) = 0;
        virtual jxx::Ptr<V> remove(jxx::Ptr<K> key) = 0;
        virtual void clear() = 0;
        virtual jxx::Ptr<Set<K>> keySet() const = 0;
        virtual jxx::Ptr<Collection<V>> values() const = 0;
        virtual jxx::Ptr<Set<MapEntry<K, V>>> entrySet() const = 0;
    };
}
