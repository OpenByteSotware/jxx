#pragma once

#include "util/jxx.util.Comparator.h"
#include "util/jxx.util.Map.h"

namespace jxx::util
{
    template<typename K, typename V>
    class SortedMap : public Map<K, V>
    {
    protected:
        SortedMap() = default;

    public:
        ~SortedMap() override = default;

    public:
        virtual jxx::Ptr<Comparator<K>> comparator() const = 0;
        virtual jxx::Ptr<K> firstKey() const = 0;
        virtual jxx::Ptr<K> lastKey() const = 0;

        virtual jxx::Ptr<SortedMap<K, V>> headMap(jxx::Ptr<K> toKey) const = 0;
        virtual jxx::Ptr<SortedMap<K, V>> subMap(jxx::Ptr<K> fromKey,
                                                 jxx::Ptr<K> toKey) const = 0;
        virtual jxx::Ptr<SortedMap<K, V>> tailMap(jxx::Ptr<K> fromKey) const = 0;
    };
}
