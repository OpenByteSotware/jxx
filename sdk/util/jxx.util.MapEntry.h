#pragma once

#include "jxx_types.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::util
{
    template<typename K, typename V>
    class MapEntry : public jxx::lang::Object
    {
    public:
        ~MapEntry() override = default;

    public:
        virtual jxx::Ptr<K> getKey() const = 0;
        virtual jxx::Ptr<V> getValue() const = 0;
        virtual jxx::Ptr<V> setValue(jxx::Ptr<V> value) = 0;
    };

    template<typename K, typename V>
    class BasicMapEntry final : public MapEntry<K, V>
    {
    public:
        BasicMapEntry(jxx::Ptr<K> key,
                      jxx::Ptr<V> value)
            : key_(std::move(key)),
              value_(std::move(value))
        {
        }

        ~BasicMapEntry() override = default;

    public:
        jxx::Ptr<K> getKey() const override { return key_; }
        jxx::Ptr<V> getValue() const override { return value_; }

        jxx::Ptr<V> setValue(jxx::Ptr<V> value) override
        {
            auto old = value_;
            value_ = std::move(value);
            return old;
        }

    private:
        jxx::Ptr<K> key_;
        jxx::Ptr<V> value_;
    };
}
