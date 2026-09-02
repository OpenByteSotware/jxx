#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::util
{

    template <typename K, typename V>
    class MapEntry
        : public jxx::lang::InterfaceBase<
        MapEntry<K, V>> {
    public:
        ~MapEntry() override = default;

        virtual jxx::Ptr<K> getKey() = 0;

        virtual jxx::Ptr<V> getValue() = 0;

        virtual jxx::Ptr<V> setValue(
            const jxx::Ptr<V>& value) = 0;

        virtual jxx::lang::jbool equals(
            const jxx::Ptr<
                jxx::lang::Object>& object)
            const = 0;

        virtual jxx::lang::jint hashCode()
            const = 0;
    };

} // namespace jxx::util