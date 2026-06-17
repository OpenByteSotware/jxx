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
        virtual jxx::Ptr<MapEntry<K, V>> lowerEntry(jxx::Ptr<K> key) const = 0;
        virtual jxx::Ptr<K> lowerKey(jxx::Ptr<K> key) const = 0;

        virtual jxx::Ptr<MapEntry<K, V>> floorEntry(jxx::Ptr<K> key) const = 0;
        virtual jxx::Ptr<K> floorKey(jxx::Ptr<K> key) const = 0;

        virtual jxx::Ptr<MapEntry<K, V>> ceilingEntry(jxx::Ptr<K> key) const = 0;
        virtual jxx::Ptr<K> ceilingKey(jxx::Ptr<K> key) const = 0;

        virtual jxx::Ptr<MapEntry<K, V>> higherEntry(jxx::Ptr<K> key) const = 0;
        virtual jxx::Ptr<K> higherKey(jxx::Ptr<K> key) const = 0;

        virtual jxx::Ptr<MapEntry<K, V>> firstEntry() const = 0;
        virtual jxx::Ptr<MapEntry<K, V>> lastEntry() const = 0;
        virtual jxx::Ptr<MapEntry<K, V>> pollFirstEntry() = 0;
        virtual jxx::Ptr<MapEntry<K, V>> pollLastEntry() = 0;

        virtual jxx::Ptr<NavigableMap<K, V>> descendingMap() const = 0;
        virtual jxx::Ptr<NavigableSet<K>> navigableKeySet() const = 0;
        virtual jxx::Ptr<NavigableSet<K>> descendingKeySet() const = 0;

        virtual jxx::Ptr<NavigableMap<K, V>> headMap(jxx::Ptr<K> toKey,
                                                     jxx::lang::jbool inclusive) const = 0;
        virtual jxx::Ptr<NavigableMap<K, V>> subMap(jxx::Ptr<K> fromKey,
                                                    jxx::lang::jbool fromInclusive,
                                                    jxx::Ptr<K> toKey,
                                                    jxx::lang::jbool toInclusive) const = 0;
        virtual jxx::Ptr<NavigableMap<K, V>> tailMap(jxx::Ptr<K> fromKey,
                                                     jxx::lang::jbool inclusive) const = 0;
    };
}
