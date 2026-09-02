#pragma once
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "util/jxx.util.Map.h"
#include "util/jxx.util.AbstractCollection.h"
#include "util/jxx.util.AbstractSet.h"
#include <exception>

namespace jxx {
namespace util {

template <typename K, typename V>
class AbstractMap : public jxx::lang::ClassBase<AbstractMap<K,V>, jxx::lang::Object, Map<K, V>> {
protected:
    jxx::Ptr<Set<K>> keySetView;
    jxx::Ptr<Collection<V>> valuesView;

public:
    virtual ~AbstractMap() override = default;
    virtual jxx::Ptr<Set<MapEntry<K, V>>> entrySet() = 0;

    virtual jxx::lang::jint size() override { return entrySet()->size(); }
    virtual jxx::lang::jbool isEmpty() override { return size() == 0; }

    virtual jxx::lang::jbool containsValue(const jxx::Ptr<jxx::lang::Object>& value) override {
        auto it = entrySet()->iterator();
        if (value == nullptr) {
            while (it->hasNext()) if (it->next()->getValue() == nullptr) return true;
        } else {
            while (it->hasNext()) {
                auto v = it->next()->getValue();
                if (v != nullptr && value->equals(jxx::CAST<jxx::lang::Object>(v))) return true;
            }
        }
        return false;
    }

    virtual jxx::lang::jbool containsKey(const jxx::Ptr<jxx::lang::Object>& key) override {
        auto it = entrySet()->iterator();
        if (key == nullptr) {
            while (it->hasNext()) if (it->next()->getKey() == nullptr) return true;
        } else {
            while (it->hasNext()) {
                auto k = it->next()->getKey();
                if (k != nullptr && key->equals(jxx::CAST<jxx::lang::Object>(k))) return true;
            }
        }
        return false;
    }

    virtual jxx::Ptr<V> get(const jxx::Ptr<jxx::lang::Object>& key) override {
        auto it = entrySet()->iterator();
        if (key == nullptr) {
            while (it->hasNext()) {
                auto e = it->next();
                if (e->getKey() == nullptr) return e->getValue();
            }
        } else {
            while (it->hasNext()) {
                auto e = it->next();
                auto k = e->getKey();
                if (k != nullptr && key->equals(jxx::CAST<jxx::lang::Object>(k))) return e->getValue();
            }
        }
        return nullptr;
    }

    virtual jxx::Ptr<V> put(const jxx::Ptr<K>&/*key*/, const jxx::Ptr<V>&/*value*/) override {
        throw std::exception();
    }

    virtual jxx::Ptr<V> remove(const jxx::Ptr<jxx::lang::Object>& key) override {
        auto it = entrySet()->iterator();
        if (key == nullptr) {
            while (it->hasNext()) {
                auto e = it->next();
                if (e->getKey() == nullptr) {
                    jxx::Ptr<V> oldValue = e->getValue();
                    it->remove();
                    return oldValue;
                }
            }
        } else {
            while (it->hasNext()) {
                auto e = it->next();
                auto k = e->getKey();
                if (k != nullptr && key->equals(jxx::CAST<jxx::lang::Object>(k))) {
                    jxx::Ptr<V> oldValue = e->getValue();
                    it->remove();
                    return oldValue;
                }
            }
        }
        return nullptr;
    }

    virtual void putAll(const jxx::Ptr<Map<K, V>>& m) override {
        if (m == nullptr) throw std::exception();
        auto it = m->entrySet()->iterator();
        while (it->hasNext()) {
            auto e = it->next();
            put(e->getKey(), e->getValue());
        }
    }

    virtual void clear() override { entrySet()->clear(); }

    class KeySet : public AbstractSet<K> {
    private:
        jxx::Ptr<AbstractMap<K, V>> map_;
        class KeyIterator : public virtual Iterator<K> {
        private:
            jxx::Ptr<Iterator<MapEntry<K, V>>> inner_;
        public:
            explicit KeyIterator(
                const jxx::Ptr<Iterator<MapEntry<K, V>>>& inner)
                : inner_(inner) {}
            virtual ~KeyIterator() = default;
            virtual jxx::lang::jbool hasNext() override { return inner_->hasNext(); }
            virtual jxx::Ptr<K> next() override { return inner_->next()->getKey(); }
            virtual void remove() override { inner_->remove(); }
        };
    public:
        explicit KeySet(
            const jxx::Ptr<AbstractMap<K, V>>& map)
            : map_(map) {
            if (map_ == nullptr) {
                throw std::exception();
            }
        }
        virtual ~KeySet() = default;
        virtual jxx::lang::jint size() override { return map_->size(); }
        virtual jxx::lang::jbool isEmpty() override { return map_->isEmpty(); }
        virtual jxx::lang::jbool contains(const jxx::Ptr<jxx::lang::Object>& o) override { return map_->containsKey(o); }
        virtual jxx::Ptr<Iterator<K>> iterator() override {
            auto iteratorValue = jxx::NEW<KeyIterator>(
                map_->entrySet()->iterator());

            return jxx::CAST<Iterator<K>>(iteratorValue);
        }
        virtual jxx::lang::ObjectArray toArray() override { return AbstractCollection<K>::toArray(); }
        virtual jxx::lang::jbool add(const jxx::Ptr<K>& /*element*/) override { throw std::exception(); }
        virtual jxx::lang::jbool remove(
            const jxx::Ptr<jxx::lang::Object>& object) override {

            if (!map_->containsKey(object)) {
                return false;
            }

            map_->remove(object);
            return true;
        }
        virtual jxx::lang::jbool containsAll(const jxx::Ptr<wildcard::CollectionAny>& c) override { return AbstractCollection<K>::containsAll(c); }
        virtual jxx::lang::jbool addAll(const jxx::Ptr<wildcard::CollectionExtends<K>>& c) override { return AbstractCollection<K>::addAll(c); }
        virtual jxx::lang::jbool removeAll(const jxx::Ptr<wildcard::CollectionAny>& c) override { return AbstractSet<K>::removeAll(c); }
        virtual jxx::lang::jbool retainAll(const jxx::Ptr<wildcard::CollectionAny>& c) override { return AbstractCollection<K>::retainAll(c); }
        virtual void clear() override { map_->clear(); }
    };

    class Values : public AbstractCollection<V> {
    private:
        jxx::Ptr<AbstractMap<K, V>> map_;
        class ValueIterator : public virtual Iterator<V> {
        private:
            jxx::Ptr<Iterator<MapEntry<K, V>>> inner_;
        public:
            explicit ValueIterator(
                const jxx::Ptr<Iterator<MapEntry<K, V>>>& inner)
                : inner_(inner) {}
            virtual ~ValueIterator() = default;
            virtual jxx::lang::jbool hasNext() override { return inner_->hasNext(); }
            virtual jxx::Ptr<V> next() override { return inner_->next()->getValue(); }
            virtual void remove() override { inner_->remove(); }
        };
    public:
        explicit Values(
            const jxx::Ptr<AbstractMap<K, V>>& map)
            : map_(map) {
            if (map_ == nullptr) {
                throw jxx::lang::NullPointerException();
            }
        }
        virtual ~Values() = default;
        virtual jxx::lang::jint size() override { return map_->size(); }
        virtual jxx::lang::jbool isEmpty() override { return map_->isEmpty(); }
        virtual jxx::lang::jbool contains(const jxx::Ptr<jxx::lang::Object>& o) override { return map_->containsValue(o); }
        virtual jxx::Ptr<Iterator<V>> iterator() override {
            auto iteratorValue = jxx::NEW<ValueIterator>(
                map_->entrySet()->iterator());

            return jxx::CAST<Iterator<V>>(iteratorValue);
        }
        virtual jxx::lang::ObjectArray toArray() override { return AbstractCollection<V>::toArray(); }
        virtual jxx::lang::jbool add(const jxx::Ptr<V>& /*element*/) override { throw std::exception(); }
        virtual jxx::lang::jbool remove(const jxx::Ptr<jxx::lang::Object>& o) override {
            auto it = map_->entrySet()->iterator();
            if (o == nullptr) {
                while (it->hasNext()) {
                    auto e = it->next();
                    if (e->getValue() == nullptr) { it->remove(); return true; }
                }
            } else {
                while (it->hasNext()) {
                    auto e = it->next();
                    auto v = e->getValue();
                    if (v != nullptr && o->equals(jxx::CAST<jxx::lang::Object>(v))) { it->remove(); return true; }
                }
            }
            return false;
        }
        virtual jxx::lang::jbool containsAll(const jxx::Ptr<wildcard::CollectionAny>& c) override { return AbstractCollection<V>::containsAll(c); }
        virtual jxx::lang::jbool addAll(const jxx::Ptr<wildcard::CollectionExtends<V>>& c) override { return AbstractCollection<V>::addAll(c); }
        virtual jxx::lang::jbool removeAll(const jxx::Ptr<wildcard::CollectionAny>& c) override { return AbstractCollection<V>::removeAll(c); }
        virtual jxx::lang::jbool retainAll(const jxx::Ptr<wildcard::CollectionAny>& c) override { return AbstractCollection<V>::retainAll(c); }
        virtual void clear() override { map_->clear(); }
    };

    virtual jxx::Ptr<Set<K>> keySet() override {
        if (keySetView == nullptr) {
            const jxx::Ptr<AbstractMap<K, V>> owner =
                jxx::CAST<AbstractMap<K, V>>(this->thisPtr);

            if (owner == nullptr) {
                throw jxx::lang::IllegalStateException();
            }

            auto view = jxx::NEW<KeySet>(owner);
            keySetView = jxx::CAST<Set<K>>(view);
        }
        return keySetView;
    }

    virtual jxx::Ptr<Collection<V>> values() override {
        if (valuesView == nullptr) {
            const jxx::Ptr<AbstractMap<K, V>> owner =
                jxx::CAST<AbstractMap<K, V>>(this->thisPtr);

            if (owner == nullptr) {
                throw jxx::lang::IllegalStateException();
            }

            auto view = jxx::NEW<Values>(owner);
            valuesView = jxx::CAST<Collection<V>>(view);
        }
        return valuesView;
    }

    jxx::lang::jbool equals(
    const jxx::Ptr<jxx::lang::Object>& object)
        const override
    {

        if (object == nullptr) {
            return false;
        }

        if (object.get() == this) {
            return true;
        }

        auto map =
            jxx::CAST<Map<K, V>>(object);

        if (map == nullptr) {
            return false;
        }

        auto* self =
            const_cast<AbstractMap<K, V>*>(
                this);

        if (map->size() != self->size()) {
            return false;
        }

        auto entrySetValue =
            self->entrySet();

        auto iteratorValue =
            entrySetValue->iterator();

        while (iteratorValue->hasNext()) {
            auto entry =
                iteratorValue->next();

            auto keyObject =
                jxx::CAST<jxx::lang::Object>(
                    entry->getKey());

            auto value =
                entry->getValue();

            auto otherValue =
                map->get(keyObject);

            if (value == nullptr) {
                if (otherValue != nullptr ||
                    !map->containsKey(keyObject)) {

                    return false;
                }
            }
            else {
                auto otherObject =
                    jxx::CAST<jxx::lang::Object>(
                        otherValue);

                if (otherObject == nullptr ||
                    !value->equals(otherObject)) {

                    return false;
                }
            }
        }

        return true;
    }

    jxx::lang::jint hashCode()
        const override
    {

        auto* self =
            const_cast<AbstractMap<K, V>*>(
                this);

        auto entrySetValue =
            self->entrySet();

        auto iteratorValue =
            entrySetValue->iterator();

        jxx::lang::jint hash = 0;

        while (iteratorValue->hasNext()) {
            auto entry =
                iteratorValue->next();

            if (entry != nullptr) {
                hash += entry->hashCode();
            }
        }

        return hash;
    }
};

} // namespace util
} // namespace jxx
