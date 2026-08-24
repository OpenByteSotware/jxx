#pragma once

#include "util/jxx.util.NavigableSet.h"
#include "util/jxx.util.SortedMap.h"

namespace jxx::util
{
    template<typename K, typename V>
    class NavigableMap : public SortedMap<K, V>
    {
    protected:
        NavigableMap() = default;

    public:
        ~NavigableMap() override = default;

    public:
        virtual jxx::Ptr<MapEntry<K, V>> lowerEntry(const jxx::Ptr<K> key) const = 0;
        virtual jxx::Ptr<K> lowerKey(const jxx::Ptr<K> key) const = 0;
        virtual jxx::Ptr<MapEntry<K, V>> floorEntry(const jxx::Ptr<K> key) const = 0;
        virtual jxx::Ptr<K> floorKey(const jxx::Ptr<K> key) const = 0;
        virtual jxx::Ptr<MapEntry<K, V>> ceilingEntry(const jxx::Ptr<K> key) const = 0;
        virtual jxx::Ptr<K> ceilingKey(const jxx::Ptr<K> key) const = 0;
        virtual jxx::Ptr<MapEntry<K, V>> higherEntry(const jxx::Ptr<K> key) const = 0;
        virtual jxx::Ptr<K> higherKey(const jxx::Ptr<K> key) const = 0;
        virtual jxx::Ptr<MapEntry<K, V>> firstEntry() const = 0;
        virtual jxx::Ptr<MapEntry<K, V>> lastEntry() const = 0;
        virtual jxx::Ptr<MapEntry<K, V>> pollFirstEntry() = 0;
        virtual jxx::Ptr<MapEntry<K, V>> pollLastEntry() = 0;
        virtual jxx::Ptr<NavigableMap<K, V>> descendingMap() const = 0;
        virtual jxx::Ptr<NavigableSet<K>> navigableKeySet() const = 0;
        virtual jxx::Ptr<NavigableSet<K>> descendingKeySet() const = 0;
        virtual jxx::Ptr<NavigableMap<K, V>> subMap(const jxx::Ptr<K> fromKey,
                                                    jxx::lang::jbool fromInclusive,
                                                    jxx::Ptr<K> toKey,
                                                    jxx::lang::jbool toInclusive) const = 0;
        virtual jxx::Ptr<NavigableMap<K, V>> headMap(const jxx::Ptr<K> toKey,
                                                     jxx::lang::jbool inclusive) const = 0;
        virtual jxx::Ptr<NavigableMap<K, V>> tailMap(const jxx::Ptr<K> fromKey,
                                                     jxx::lang::jbool inclusive) const = 0;
    };
}
