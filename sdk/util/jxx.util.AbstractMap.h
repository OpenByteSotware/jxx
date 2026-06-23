#pragma once

#include "util/jxx.util.Map.h"
#include "util/jxx.util.AbstractCollection.h"
#include "util/jxx.util.AbstractSet.h"

namespace jxx {
namespace util {

template <typename K, typename V>
class AbstractMap : public virtual Map<K, V> {
protected:
    jxx::Ptr<Set<K>> keySetView;
    jxx::Ptr<Collection<V>> valuesView;

public:
    virtual ~AbstractMap() = default;
    virtual jxx::Ptr<Set<MapEntry<K, V>>> entrySet() = 0;

    virtual jxx::lang::jint size() override { return entrySet()->size(); }
    virtual jxx::lang::jbool isEmpty() override { return size() == 0; }

    virtual jxx::lang::jbool containsValue(jxx::Ptr<jxx::lang::Object> value) override {
        auto it = entrySet()->iterator();
        if (value == nullptr) {
            while (it->hasNext()) if (it->next()->getValue() == nullptr) return true;
        } else {
            while (it->hasNext()) {
                auto v = it->next()->getValue();
                if (v != nullptr && value->equals(jxx::lang::ptr_static_cast<jxx::lang::Object>(v))) return true;
            }
        }
        return false;
    }

    virtual jxx::lang::jbool containsKey(jxx::Ptr<jxx::lang::Object> key) override {
        auto it = entrySet()->iterator();
        if (key == nullptr) {
            while (it->hasNext()) if (it->next()->getKey() == nullptr) return true;
        } else {
            while (it->hasNext()) {
                auto k = it->next()->getKey();
                if (k != nullptr && key->equals(jxx::lang::ptr_static_cast<jxx::lang::Object>(k))) return true;
            }
        }
        return false;
    }

    virtual jxx::Ptr<V> get(jxx::Ptr<jxx::lang::Object> key) override {
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
                if (k != nullptr && key->equals(jxx::lang::ptr_static_cast<jxx::lang::Object>(k))) return e->getValue();
            }
        }
        return nullptr;
    }

    virtual jxx::Ptr<V> put(jxx::Ptr<K> /*key*/, jxx::Ptr<V> /*value*/) override {
        throw UnsupportedOperationException();
    }

    virtual jxx::Ptr<V> remove(jxx::Ptr<jxx::lang::Object> key) override {
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
                if (k != nullptr && key->equals(jxx::lang::ptr_static_cast<jxx::lang::Object>(k))) {
                    jxx::Ptr<V> oldValue = e->getValue();
                    it->remove();
                    return oldValue;
                }
            }
        }
        return nullptr;
    }

    virtual void putAll(jxx::Ptr<Map<K, V>> m) override {
        if (m == nullptr) throw NullPointerException();
        auto it = m->entrySet()->iterator();
        while (it->hasNext()) {
            auto e = it->next();
            put(e->getKey(), e->getValue());
        }
    }

    virtual void clear() override { entrySet()->clear(); }

    class KeySet : public AbstractSet<K> {
    private:
        AbstractMap<K, V>* map_;
        class KeyIterator : public virtual Iterator<K> {
        private:
            jxx::Ptr<Iterator<MapEntry<K, V>>> inner_;
        public:
            explicit KeyIterator(jxx::Ptr<Iterator<MapEntry<K, V>>> inner) : inner_(inner) {}
            virtual ~KeyIterator() = default;
            virtual jxx::lang::jbool hasNext() override { return inner_->hasNext(); }
            virtual jxx::Ptr<K> next() override { return inner_->next()->getKey(); }
            virtual void remove() override { inner_->remove(); }
        };
    public:
        explicit KeySet(AbstractMap<K, V>* map) : map_(map) {}
        virtual ~KeySet() = default;
        virtual jxx::lang::jint size() override { return map_->size(); }
        virtual jxx::lang::jbool isEmpty() override { return map_->isEmpty(); }
        virtual jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> o) override { return map_->containsKey(o); }
        virtual jxx::Ptr<Iterator<K>> iterator() override { return jxx::Ptr<Iterator<K>>(new KeyIterator(map_->entrySet()->iterator())); }
        virtual jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>>> toArray() override { return AbstractCollection<K>::toArray(); }
        virtual jxx::lang::jbool add(jxx::Ptr<K> /*e*/) override { throw UnsupportedOperationException(); }
        virtual jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> o) override { return map_->remove(o) != nullptr; }
        virtual jxx::lang::jbool containsAll(jxx::Ptr<wildcard::CollectionAny> c) override { return AbstractCollection<K>::containsAll(c); }
        virtual jxx::lang::jbool addAll(jxx::Ptr<wildcard::CollectionExtends<K>> c) override { return AbstractCollection<K>::addAll(c); }
        virtual jxx::lang::jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) override { return AbstractSet<K>::removeAll(c); }
        virtual jxx::lang::jbool retainAll(jxx::Ptr<wildcard::CollectionAny> c) override { return AbstractCollection<K>::retainAll(c); }
        virtual void clear() override { map_->clear(); }
    };

    class Values : public AbstractCollection<V> {
    private:
        AbstractMap<K, V>* map_;
        class ValueIterator : public virtual Iterator<V> {
        private:
            jxx::Ptr<Iterator<MapEntry<K, V>>> inner_;
        public:
            explicit ValueIterator(jxx::Ptr<Iterator<MapEntry<K, V>>> inner) : inner_(inner) {}
            virtual ~ValueIterator() = default;
            virtual jxx::lang::jbool hasNext() override { return inner_->hasNext(); }
            virtual jxx::Ptr<V> next() override { return inner_->next()->getValue(); }
            virtual void remove() override { inner_->remove(); }
        };
    public:
        explicit Values(AbstractMap<K, V>* map) : map_(map) {}
        virtual ~Values() = default;
        virtual jxx::lang::jint size() override { return map_->size(); }
        virtual jxx::lang::jbool isEmpty() override { return map_->isEmpty(); }
        virtual jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> o) override { return map_->containsValue(o); }
        virtual jxx::Ptr<Iterator<V>> iterator() override { return jxx::Ptr<Iterator<V>>(new ValueIterator(map_->entrySet()->iterator())); }
        virtual jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>>> toArray() override { return AbstractCollection<V>::toArray(); }
        virtual jxx::lang::jbool add(jxx::Ptr<V> /*e*/) override { throw UnsupportedOperationException(); }
        virtual jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> o) override {
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
                    if (v != nullptr && o->equals(jxx::lang::ptr_static_cast<jxx::lang::Object>(v))) { it->remove(); return true; }
                }
            }
            return false;
        }
        virtual jxx::lang::jbool containsAll(jxx::Ptr<wildcard::CollectionAny> c) override { return AbstractCollection<V>::containsAll(c); }
        virtual jxx::lang::jbool addAll(jxx::Ptr<wildcard::CollectionExtends<V>> c) override { return AbstractCollection<V>::addAll(c); }
        virtual jxx::lang::jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) override { return AbstractCollection<V>::removeAll(c); }
        virtual jxx::lang::jbool retainAll(jxx::Ptr<wildcard::CollectionAny> c) override { return AbstractCollection<V>::retainAll(c); }
        virtual void clear() override { map_->clear(); }
    };

    virtual jxx::Ptr<Set<K>> keySet() override {
        if (keySetView == nullptr) keySetView = jxx::Ptr<Set<K>>(new KeySet(this));
        return keySetView;
    }

    virtual jxx::Ptr<Collection<V>> values() override {
        if (valuesView == nullptr) valuesView = jxx::Ptr<Collection<V>>(new Values(this));
        return valuesView;
    }

    virtual jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> o) override {
        auto m = jxx::lang::ptr_checked_cast<Map<K, V>>(o);
        if (m == nullptr) return false;
        if (m->size() != this->size()) return false;
        auto it = this->entrySet()->iterator();
        while (it->hasNext()) {
            auto e = it->next();
            auto key = e->getKey();
            auto value = e->getValue();
            auto otherValue = m->get(key);
            if (value == nullptr) {
                if (!(otherValue == nullptr && m->containsKey(key))) return false;
            } else {
                if (otherValue == nullptr || !value->equals(otherValue)) return false;
            }
        }
        return true;
    }

    virtual jxx::lang::jint hashCode() override {
        jxx::lang::jint h = 0;
        auto it = entrySet()->iterator();
        while (it->hasNext()) h += it->next()->hashCode();
        return h;
    }
};

} // namespace util
} // namespace jxx
