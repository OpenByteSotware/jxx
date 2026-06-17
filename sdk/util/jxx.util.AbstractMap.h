#pragma once

#include <stdexcept>

#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.Map.h"

namespace jxx::util
{
    namespace internal
    {
        template<typename T>
        inline bool ptrEquals(jxx::Ptr<T> a, jxx::Ptr<T> b)
        {
            return (!a && !b) || (a && b && a->equals(b));
        }
    }

    template<typename K, typename V>
    class AbstractMap
        : public jxx::lang::Object
        , public Map<K, V>
    {
    protected:
        AbstractMap() = default;

    public:
        ~AbstractMap() override = default;

    public:
        jxx::lang::jbool isEmpty() const override
        {
            return size() == 0;
        }

        jxx::lang::jbool containsKey(jxx::Ptr<K> key) const override
        {
            auto es = entrySet();
            if (!es)
                return false;
            for (jxx::lang::jint i = 0; i < es->size(); ++i)
            {
                auto e = es->get(i);
                if (!e)
                    continue;
                if (internal::ptrEquals(e->getKey(), key))
                    return true;
            }
            return false;
        }

        jxx::lang::jbool containsValue(jxx::Ptr<V> value) const override
        {
            auto es = entrySet();
            if (!es)
                return false;
            for (jxx::lang::jint i = 0; i < es->size(); ++i)
            {
                auto e = es->get(i);
                if (!e)
                    continue;
                if (internal::ptrEquals(e->getValue(), value))
                    return true;
            }
            return false;
        }

        jxx::Ptr<V> get(jxx::Ptr<K> key) const override
        {
            auto es = entrySet();
            if (!es)
                return nullptr;
            for (jxx::lang::jint i = 0; i < es->size(); ++i)
            {
                auto e = es->get(i);
                if (!e)
                    continue;
                if (internal::ptrEquals(e->getKey(), key))
                    return e->getValue();
            }
            return nullptr;
        }

        jxx::Ptr<V> put(jxx::Ptr<K>, jxx::Ptr<V>) override
        {
            throw std::runtime_error("AbstractMap::put unsupported by default");
        }

        void putAll(jxx::Ptr<Map<K, V>> m) override
        {
            if (!m)
                return;
            auto es = m->entrySet();
            if (!es)
                return;
            for (jxx::lang::jint i = 0; i < es->size(); ++i)
            {
                auto e = es->get(i);
                if (e)
                    put(e->getKey(), e->getValue());
            }
        }

        void clear() override
        {
            auto es = entrySet();
            if (es)
                es->clear();
        }

        jxx::Ptr<Collection<V>> values() const override
        {
            auto out = std::make_shared<ArrayList<V>>();
            auto es = entrySet();
            if (!es)
                return out;
            for (jxx::lang::jint i = 0; i < es->size(); ++i)
            {
                auto e = es->get(i);
                if (e)
                    out->add(e->getValue());
            }
            return out;
        }

        // Remaining keySet(), entrySet(), size(), remove() stay abstract.
    };
}
