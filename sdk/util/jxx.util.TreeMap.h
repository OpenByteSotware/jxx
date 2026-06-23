#pragma once

#include <map>
#include <utility>
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.AbstractMap.h"
#include "util/jxx.util.ComparatorSuper.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.Cloneable.h"
#include "io/jxx.io.Serializable.h"

namespace jxx {
namespace util {

template <typename K, typename V>
class TreeMap
    : public AbstractMap<K, V>
    , public virtual jxx::lang::Cloneable
    , public virtual jxx::io::Serializable {
private:
    struct KeyLess {
        jxx::Ptr<ComparatorSuper<K>> comp_;

        bool operator()(const jxx::Ptr<K>& a, const jxx::Ptr<K>& b) const {
            if (a == nullptr || b == nullptr) {
                throw NullPointerException();
            }
            if (comp_ != nullptr) {
                return comp_->compare(a, b) < 0;
            }
            auto comparable = jxx::lang::ptr_checked_cast<jxx::lang::Comparable<K>>(a);
            if (comparable == nullptr) {
                throw ClassCastException();
            }
            return comparable->compareTo(b) < 0;
        }
    };

    using InternalMap = std::map<jxx::Ptr<K>, jxx::Ptr<V>, KeyLess>;

    InternalMap map_;
    jxx::Ptr<ComparatorSuper<K>> comparator_;
    jint modCount_;
    jxx::Ptr<Set<MapEntry<K, V>>> entrySetView_;

public:
    TreeMap()
        : map_(KeyLess{nullptr})
        , comparator_(nullptr)
        , modCount_(0)
        , entrySetView_(nullptr) {
    }

    explicit TreeMap(jxx::Ptr<ComparatorSuper<K>> comparator)
        : map_(KeyLess{comparator})
        , comparator_(comparator)
        , modCount_(0)
        , entrySetView_(nullptr) {
    }

    // Standalone approximation of TreeMap(Map<? extends K, ? extends V> m)
    explicit TreeMap(jxx::Ptr<Map<K, V>> m)
        : TreeMap() {
        if (m == nullptr) throw NullPointerException();
        putAll(m);
    }

    // Standalone approximation of TreeMap(SortedMap<K, ? extends V> m)
    explicit TreeMap(jxx::Ptr<TreeMap<K, V>> m)
        : TreeMap(m == nullptr ? jxx::Ptr<ComparatorSuper<K>>(nullptr) : m->comparator()) {
        if (m == nullptr) throw NullPointerException();
        putAll(m);
    }

    virtual ~TreeMap() = default;

    virtual jxx::lang::jint size() override { return static_cast<jxx::lang::jint>(map_.size()); }
    virtual jxx::lang::jbool isEmpty() override { return map_.empty(); }

    virtual jxx::Ptr<ComparatorSuper<K>> comparator() {
        return comparator_;
    }

    virtual jxx::lang::jbool containsKey(jxx::Ptr<jxx::lang::Object> key) override {
        auto castKey = jxx::lang::ptr_checked_cast<K>(key);
        if (castKey == nullptr) return false;
        return map_.find(castKey) != map_.end();
    }

    virtual jxx::lang::jbool containsValue(jxx::Ptr<jxx::lang::Object> value) override {
        if (value == nullptr) {
            for (const auto& kv : map_) if (kv.second == nullptr) return true;
        } else {
            for (const auto& kv : map_) {
                if (kv.second != nullptr && value->equals(jxx::lang::ptr_static_cast<jxx::lang::Object>(kv.second))) return true;
            }
        }
        return false;
    }

    virtual jxx::Ptr<V> get(jxx::Ptr<jxx::lang::Object> key) override {
        auto castKey = jxx::lang::ptr_checked_cast<K>(key);
        if (castKey == nullptr) return nullptr;
        auto it = map_.find(castKey);
        if (it == map_.end()) return nullptr;
        return it->second;
    }

    virtual jxx::Ptr<V> put(jxx::Ptr<K> key, jxx::Ptr<V> value) override {
        if (key == nullptr) throw NullPointerException();
        auto it = map_.find(key);
        if (it == map_.end()) {
            map_.emplace(key, value);
            ++modCount_;
            return nullptr;
        }
        jxx::Ptr<V> oldValue = it->second;
        it->second = value;
        return oldValue;
    }

    virtual jxx::Ptr<V> remove(jxx::Ptr<jxx::lang::Object> key) override {
        auto castKey = jxx::lang::ptr_checked_cast<K>(key);
        if (castKey == nullptr) return nullptr;
        auto it = map_.find(castKey);
        if (it == map_.end()) return nullptr;
        jxx::Ptr<V> oldValue = it->second;
        map_.erase(it);
        ++modCount_;
        return oldValue;
    }

    virtual void putAll(jxx::Ptr<Map<K, V>> m) override {
        if (m == nullptr) throw NullPointerException();
        auto it = m->entrySet()->iterator();
        while (it->hasNext()) {
            auto e = it->next();
            put(e->getKey(), e->getValue());
        }
    }

    virtual void clear() override {
        if (!map_.empty()) {
            map_.clear();
            ++modCount_;
        }
    }

    virtual jxx::Ptr<K> firstKey() {
        if (map_.empty()) throw NoSuchElementException();
        return map_.begin()->first;
    }

    virtual jxx::Ptr<K> lastKey() {
        if (map_.empty()) throw NoSuchElementException();
        auto it = map_.end();
        --it;
        return it->first;
    }

    virtual jxx::Ptr<MapEntry<K, V>> firstEntry() {
        if (map_.empty()) return nullptr;
        return makeEntryView(map_.begin()->first);
    }

    virtual jxx::Ptr<MapEntry<K, V>> lastEntry() {
        if (map_.empty()) return nullptr;
        auto it = map_.end();
        --it;
        return makeEntryView(it->first);
    }

    virtual jxx::Ptr<MapEntry<K, V>> pollFirstEntry() {
        if (map_.empty()) return nullptr;
        auto key = map_.begin()->first;
        auto entry = makeEntryView(key);
        map_.erase(map_.begin());
        ++modCount_;
        return entry;
    }

    virtual jxx::Ptr<MapEntry<K, V>> pollLastEntry() {
        if (map_.empty()) return nullptr;
        auto it = map_.end();
        --it;
        auto key = it->first;
        auto entry = makeEntryView(key);
        map_.erase(it);
        ++modCount_;
        return entry;
    }

    virtual jxx::Ptr<MapEntry<K, V>> lowerEntry(jxx::Ptr<K> key) {
        if (key == nullptr) throw NullPointerException();
        auto it = map_.lower_bound(key);
        if (it == map_.begin()) return nullptr;
        if (it == map_.end() || !keysEqual(it->first, key)) {
            --it;
            return makeEntryView(it->first);
        }
        if (it == map_.begin()) return nullptr;
        --it;
        return makeEntryView(it->first);
    }

    virtual jxx::Ptr<K> lowerKey(jxx::Ptr<K> key) {
        auto e = lowerEntry(key);
        return e == nullptr ? nullptr : e->getKey();
    }

    virtual jxx::Ptr<MapEntry<K, V>> floorEntry(jxx::Ptr<K> key) {
        if (key == nullptr) throw NullPointerException();
        auto it = map_.upper_bound(key);
        if (it == map_.begin()) return nullptr;
        --it;
        return makeEntryView(it->first);
    }

    virtual jxx::Ptr<K> floorKey(jxx::Ptr<K> key) {
        auto e = floorEntry(key);
        return e == nullptr ? nullptr : e->getKey();
    }

    virtual jxx::Ptr<MapEntry<K, V>> ceilingEntry(jxx::Ptr<K> key) {
        if (key == nullptr) throw NullPointerException();
        auto it = map_.lower_bound(key);
        if (it == map_.end()) return nullptr;
        return makeEntryView(it->first);
    }

    virtual jxx::Ptr<K> ceilingKey(jxx::Ptr<K> key) {
        auto e = ceilingEntry(key);
        return e == nullptr ? nullptr : e->getKey();
    }

    virtual jxx::Ptr<MapEntry<K, V>> higherEntry(jxx::Ptr<K> key) {
        if (key == nullptr) throw NullPointerException();
        auto it = map_.upper_bound(key);
        if (it == map_.end()) return nullptr;
        return makeEntryView(it->first);
    }

    virtual jxx::Ptr<K> higherKey(jxx::Ptr<K> key) {
        auto e = higherEntry(key);
        return e == nullptr ? nullptr : e->getKey();
    }

    virtual jxx::Ptr<Set<MapEntry<K, V>>> entrySet() override {
        if (entrySetView_ == nullptr) entrySetView_ = jxx::Ptr<Set<MapEntry<K, V>>>(new EntrySet(this));
        return entrySetView_;
    }

    virtual jxx::Ptr<jxx::lang::Object> clone() {
        jxx::Ptr<TreeMap<K, V>> cloned(new TreeMap<K, V>(comparator_));
        for (const auto& kv : map_) {
            cloned->map_.emplace(kv.first, kv.second);
        }
        return cloned;
    }

private:
    jxx::lang::jbool keysEqual(jxx::Ptr<K> a, jxx::Ptr<K> b) {
        if (a == nullptr) return b == nullptr;
        if (b == nullptr) return false;
        if (comparator_ != nullptr) {
            return comparator_->compare(a, b) == 0;
        }
        auto comparable = jxx::lang::ptr_checked_cast<jxx::lang::Comparable<K>>(a);
        if (comparable == nullptr) throw ClassCastException();
        return comparable->compareTo(b) == 0;
    }

protected:
    class EntryView : public virtual MapEntry<K, V> {
    private:
        TreeMap<K, V>* map_;
        jxx::Ptr<K> key_;
    public:
        EntryView(TreeMap<K, V>* map, jxx::Ptr<K> key) : map_(map), key_(key) {}
        virtual ~EntryView() = default;
        virtual jxx::Ptr<K> getKey() override { return key_; }
        virtual jxx::Ptr<V> getValue() override {
            auto it = map_->map_.find(key_);
            return it == map_->map_.end() ? nullptr : it->second;
        }
        virtual jxx::Ptr<V> setValue(jxx::Ptr<V> value) override { return map_->put(key_, value); }
        virtual jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> o) override {
            auto other = jxx::lang::ptr_checked_cast<MapEntry<K, V>>(o);
            if (other == nullptr) return false;
            auto k1 = getKey();
            auto v1 = getValue();
            auto k2 = other->getKey();
            auto v2 = other->getValue();
            jxx::lang::jbool keyEqual = (k1 == nullptr) ? (k2 == nullptr) : k1->equals(k2);
            jxx::lang::jbool valueEqual = (v1 == nullptr) ? (v2 == nullptr) : v1->equals(v2);
            return keyEqual && valueEqual;
        }
        virtual jxx::lang::jint hashCode() override {
            jxx::lang::jint kh = (key_ == nullptr) ? 0 : key_->hashCode();
            auto value = getValue();
            jxx::lang::jint vh = (value == nullptr) ? 0 : value->hashCode();
            return kh ^ vh;
        }
    };

    jxx::Ptr<MapEntry<K, V>> makeEntryView(jxx::Ptr<K> key) {
        return jxx::Ptr<MapEntry<K, V>>(new EntryView(this, key));
    }

    class EntryIterator : public virtual Iterator<MapEntry<K, V>> {
    private:
        TreeMap<K, V>* map_;
        typename InternalMap::iterator current_;
        typename InternalMap::iterator end_;
        jxx::Ptr<K> lastReturnedKey_;
        jxx::lang::jbool canRemove_;
        jxx::lang::jint expectedModCount_;
    public:
        explicit EntryIterator(TreeMap<K, V>* map)
            : map_(map)
            , current_(map->map_.begin())
            , end_(map->map_.end())
            , lastReturnedKey_(nullptr)
            , canRemove_(false)
            , expectedModCount_(map->modCount_) {
        }
        virtual ~EntryIterator() = default;
        virtual jxx::lang::jbool hasNext() override { return current_ != end_; }
        virtual jxx::Ptr<MapEntry<K, V>> next() override {
            checkForComodification();
            if (current_ == end_) throw NoSuchElementException();
            lastReturnedKey_ = current_->first;
            ++current_;
            canRemove_ = true;
            return map_->makeEntryView(lastReturnedKey_);
        }
        virtual void remove() override {
            if (!canRemove_) throw IllegalStateException();
            checkForComodification();
            map_->remove(lastReturnedKey_);
            expectedModCount_ = map_->modCount_;
            canRemove_ = false;
            lastReturnedKey_ = nullptr;
        }
    private:
        void checkForComodification() {
            if (map_->modCount_ != expectedModCount_) throw ConcurrentModificationException();
        }
    };

    class EntrySet : public AbstractSet<MapEntry<K, V>> {
    private:
        TreeMap<K, V>* map_;
    public:
        explicit EntrySet(TreeMap<K, V>* map) : map_(map) {}
        virtual ~EntrySet() = default;
        virtual jxx::lang::jint size() override { return map_->size(); }
        virtual jxx::lang::jbool isEmpty() override { return map_->isEmpty(); }
        virtual jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> o) override {
            auto e = jxx::lang::ptr_checked_cast<MapEntry<K, V>>(o);
            if (e == nullptr) return false;
            auto value = map_->get(e->getKey());
            if (value == nullptr) return e->getValue() == nullptr && map_->containsKey(e->getKey());
            return value->equals(e->getValue());
        }
        virtual jxx::Ptr<Iterator<MapEntry<K, V>>> iterator() override {
            return jxx::Ptr<Iterator<MapEntry<K, V>>>(new EntryIterator(map_));
        }
        virtual jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>>> toArray() override {
            return AbstractCollection<MapEntry<K, V>>::toArray();
        }
        virtual jxx::lang::jbool add(jxx::Ptr<MapEntry<K, V>> /*e*/) override { throw UnsupportedOperationException(); }
        virtual jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> o) override {
            auto e = jxx::lang::ptr_checked_cast<MapEntry<K, V>>(o);
            if (e == nullptr) return false;
            auto value = map_->get(e->getKey());
            if (value == nullptr) {
                if (!(e->getValue() == nullptr && map_->containsKey(e->getKey()))) return false;
            } else if (!value->equals(e->getValue())) {
                return false;
            }
            map_->remove(e->getKey());
            return true;
        }
        virtual jxx::lang::jbool containsAll(jxx::Ptr<wildcard::CollectionAny> c) override { return AbstractCollection<MapEntry<K, V>>::containsAll(c); }
        virtual jxx::lang::jbool addAll(jxx::Ptr<wildcard::CollectionExtends<MapEntry<K, V>>> /*c*/) override { throw UnsupportedOperationException(); }
        virtual jxx::lang::jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) override { return AbstractSet<MapEntry<K, V>>::removeAll(c); }
        virtual jxx::lang::jbool retainAll(jxx::Ptr<wildcard::CollectionAny> c) override { return AbstractCollection<MapEntry<K, V>>::retainAll(c); }
        virtual void clear() override { map_->clear(); }
    };
};

} // namespace util
} // namespace jxx
