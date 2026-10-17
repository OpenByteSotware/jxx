#pragma once

#include <map>
#include <utility>
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.ClassCastException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "util/jxx.util.AbstractMap.h"
#include "util/jxx.util.NavigableMap.h"
#include "util/jxx.util.TreeSet.h"
#include "util/jxx.util.ComparatorSuper.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.Cloneable.h"
#include "io/jxx.io.SerializableI.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.InvalidObjectException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.NoSuchElementException.h"


namespace jxx {
namespace util {

template <typename K, typename V>
class TreeMap : public ::jxx::lang::ClassBase<TreeMap<K,V>, AbstractMap<K, V>,
    NavigableMap<K, V>,
    jxx::lang::Cloneable,
    jxx::io::SerializableI>
 {
private:
    struct KeyLess {
        jxx::Ptr<ComparatorSuper<K>> comp_;

        bool operator()(const jxx::Ptr<K>& a, const jxx::Ptr<K>& b) const {
            if (a == nullptr || b == nullptr) {
                throw ::jxx::lang::NullPointerException();
            }
            if (comp_ != nullptr) {
                return comp_->compareSuper(a, b) < 0;
            }
            auto comparable = jxx::CAST<jxx::lang::Comparable<K>, jxx::lang::Object>(a);
            if (comparable == nullptr) {
                throw ::jxx::lang::ClassCastException();
            }
            return comparable->compareTo(b) < 0;
        }
    };

    using InternalMap = std::map<jxx::Ptr<K>, jxx::Ptr<V>, KeyLess>;

    InternalMap map_;
    jxx::Ptr<ComparatorSuper<K>> comparator_;
    jxx::lang::jint modCount_;
    jxx::Ptr<Set<MapEntry<K, V>>> entrySetView_;

public:
    using JxxSuper = AbstractMap<K, V>;
    using Super = ::jxx::lang::ClassBase<TreeMap<K, V>, JxxSuper,
        NavigableMap<K, V>, ::jxx::lang::Cloneable, ::jxx::io::SerializableI>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;

    void writeObject(
        const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) override {
        if (out == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        out->writeObject(::jxx::CAST<::jxx::lang::Object>(comparator_));
        out->writeInt(size());
        for (const auto& entry : map_) {
            out->writeObject(::jxx::CAST<::jxx::lang::Object>(entry.first));
            out->writeObject(::jxx::CAST<::jxx::lang::Object>(entry.second));
        }
    }

    void readObject(
        const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) override {
        if (in == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        const auto comparatorObject = in->readObject();
        const auto restoredComparator = comparatorObject == nullptr
            ? ::jxx::Ptr<ComparatorSuper<K>>(nullptr)
            : ::jxx::CAST<ComparatorSuper<K>>(comparatorObject);
        if (comparatorObject != nullptr && restoredComparator == nullptr) {
            throw ::jxx::io::InvalidObjectException(
                ::jxx::NEW<::jxx::lang::String>("Invalid TreeMap comparator"));
        }
        const auto count = in->readInt();
        if (count < 0) {
            throw ::jxx::io::InvalidObjectException(
                ::jxx::NEW<::jxx::lang::String>("Negative TreeMap size"));
        }

        InternalMap restored(KeyLess{restoredComparator});
        for (::jxx::lang::jint index = 0; index < count; ++index) {
            const auto key = ::jxx::CAST<K>(in->readObject());
            const auto valueObject = in->readObject();
            const auto value = valueObject == nullptr
                ? ::jxx::Ptr<V>(nullptr)
                : ::jxx::CAST<V>(valueObject);
            if (key == nullptr || (valueObject != nullptr && value == nullptr)) {
                throw ::jxx::io::InvalidObjectException(
                    ::jxx::NEW<::jxx::lang::String>("Invalid TreeMap entry"));
            }
            restored[key] = value;
        }
        map_ = std::move(restored);
        comparator_ = restoredComparator;
        modCount_ = 0;
        entrySetView_ = nullptr;
    }

    void readObjectNoData() override {
        map_.clear();
        comparator_ = nullptr;
        map_ = InternalMap(KeyLess{nullptr});
        modCount_ = 0;
        entrySetView_ = nullptr;
    }

    TreeMap()
        : map_(KeyLess{nullptr})
        , comparator_(nullptr)
        , modCount_(0)
        , entrySetView_(nullptr) {
    }

    explicit TreeMap(const jxx::Ptr<ComparatorSuper<K>>& comparator)
        : map_(KeyLess{comparator})
        , comparator_(comparator)
        , modCount_(0)
        , entrySetView_(nullptr) {
    }

    // Standalone approximation of TreeMap(Map<? extends K, ? extends V> m)
    explicit TreeMap(const jxx::Ptr<Map<K, V>>& m)
        : TreeMap() {
        if (m == nullptr) throw ::jxx::lang::NullPointerException();
        putAll(m);
    }

    // Standalone approximation of TreeMap(SortedMap<K, ? extends V> m)
    explicit TreeMap(const jxx::Ptr<TreeMap<K, V>>& m)
        : TreeMap(m == nullptr ? jxx::Ptr<ComparatorSuper<K>>(nullptr) : m->comparator()) {
        if (m == nullptr) throw ::jxx::lang::NullPointerException();
        putAll(m);
    }

    virtual ~TreeMap() = default;

    virtual jxx::lang::jint size() override { return static_cast<jxx::lang::jint>(map_.size()); }
    virtual jxx::lang::jbool isEmpty() override { return map_.empty(); }

    virtual jxx::Ptr<Comparator<K>> comparator() const override {
        return std::dynamic_pointer_cast<Comparator<K>>(comparator_);
    }

    virtual jxx::lang::jbool containsKey(const jxx::Ptr<jxx::lang::Object>& key) override {
        auto castKey = jxx::CAST<K, jxx::lang::Object>(key);
        if (castKey == nullptr) return false;
        return map_.find(castKey) != map_.end();
    }

    virtual jxx::lang::jbool containsValue(const jxx::Ptr<jxx::lang::Object>& value) override {
        if (value == nullptr) {
            for (const auto& kv : map_) if (kv.second == nullptr) return true;
        } else {
            for (const auto& kv : map_) {
                if (kv.second != nullptr && value->equals(::jxx::CAST<::jxx::lang::Object>(kv.second))) return true;
            }
        }
        return false;
    }

    virtual jxx::Ptr<V> get(const jxx::Ptr<jxx::lang::Object>& key) override {
        auto castKey = jxx::CAST<K, jxx::lang::Object>(key);
        if (castKey == nullptr) return nullptr;
        auto it = map_.find(castKey);
        if (it == map_.end()) return nullptr;
        return it->second;
    }

    virtual jxx::Ptr<V> put(const jxx::Ptr<K>& key, const jxx::Ptr<V>& value) override {
        if (key == nullptr) throw ::jxx::lang::NullPointerException();
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

    virtual jxx::Ptr<V> remove(const jxx::Ptr<jxx::lang::Object>& key) override {
        auto castKey = jxx::CAST<K, jxx::lang::Object>(key);
        if (castKey == nullptr) return nullptr;
        auto it = map_.find(castKey);
        if (it == map_.end()) return nullptr;
        jxx::Ptr<V> oldValue = it->second;
        map_.erase(it);
        ++modCount_;
        return oldValue;
    }

    virtual void putAll(const jxx::Ptr<Map<K, V>>& m) override {
        if (m == nullptr) throw ::jxx::lang::NullPointerException();
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

    virtual jxx::Ptr<K> firstKey() const override {
        if (map_.empty()) throw ::jxx::util::NoSuchElementException();
        return map_.begin()->first;
    }

    virtual jxx::Ptr<K> lastKey() const override {
        if (map_.empty()) throw ::jxx::util::NoSuchElementException();
        auto it = map_.end();
        --it;
        return it->first;
    }

    virtual jxx::Ptr<MapEntry<K, V>> firstEntry() const override {
        if (map_.empty()) return nullptr;
        return makeEntryView(map_.begin()->first);
    }

    virtual jxx::Ptr<MapEntry<K, V>> lastEntry() const override {
        if (map_.empty()) return nullptr;
        auto it = map_.end();
        --it;
        return makeEntryView(it->first);
    }

    virtual jxx::Ptr<MapEntry<K, V>> pollFirstEntry() override {
        if (map_.empty()) return nullptr;
        auto key = map_.begin()->first;
        auto entry = makeEntryView(key);
        map_.erase(map_.begin());
        ++modCount_;
        return entry;
    }

    virtual jxx::Ptr<MapEntry<K, V>> pollLastEntry() override {
        if (map_.empty()) return nullptr;
        auto it = map_.end();
        --it;
        auto key = it->first;
        auto entry = makeEntryView(key);
        map_.erase(it);
        ++modCount_;
        return entry;
    }

    virtual jxx::Ptr<MapEntry<K, V>> lowerEntry(const jxx::Ptr<K>& key) const override {
        if (key == nullptr) throw ::jxx::lang::NullPointerException();
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

    virtual jxx::Ptr<K> lowerKey(const jxx::Ptr<K>& key) const override {
        auto e = lowerEntry(key);
        return e == nullptr ? nullptr : e->getKey();
    }

    virtual jxx::Ptr<MapEntry<K, V>> floorEntry(const jxx::Ptr<K>& key) const override {
        if (key == nullptr) throw ::jxx::lang::NullPointerException();
        auto it = map_.upper_bound(key);
        if (it == map_.begin()) return nullptr;
        --it;
        return makeEntryView(it->first);
    }

    virtual jxx::Ptr<K> floorKey(const jxx::Ptr<K>& key) const override {
        auto e = floorEntry(key);
        return e == nullptr ? nullptr : e->getKey();
    }

    virtual jxx::Ptr<MapEntry<K, V>> ceilingEntry(const jxx::Ptr<K>& key) const override {
        if (key == nullptr) throw ::jxx::lang::NullPointerException();
        auto it = map_.lower_bound(key);
        if (it == map_.end()) return nullptr;
        return makeEntryView(it->first);
    }

    virtual jxx::Ptr<K> ceilingKey(const jxx::Ptr<K>& key) const override {
        auto e = ceilingEntry(key);
        return e == nullptr ? nullptr : e->getKey();
    }

    virtual jxx::Ptr<MapEntry<K, V>> higherEntry(const jxx::Ptr<K>& key) const override {
        if (key == nullptr) throw ::jxx::lang::NullPointerException();
        auto it = map_.upper_bound(key);
        if (it == map_.end()) return nullptr;
        return makeEntryView(it->first);
    }

    virtual jxx::Ptr<K> higherKey(const jxx::Ptr<K>& key) const override {
        auto e = higherEntry(key);
        return e == nullptr ? nullptr : e->getKey();
    }

    ::jxx::Ptr<NavigableMap<K, V>> descendingMap() const override {
        auto result = ::jxx::NEW<TreeMap<K, V>>(comparator_);
        for (auto iterator = map_.rbegin(); iterator != map_.rend(); ++iterator)
            result->put(iterator->first, iterator->second);
        return ::jxx::CAST<NavigableMap<K, V>>(result);
    }

    ::jxx::Ptr<NavigableSet<K>> navigableKeySet() const override {
        auto result = ::jxx::NEW<TreeSet<K>>(comparator());
        for (const auto& entry : map_) result->add(entry.first);
        return ::jxx::CAST<NavigableSet<K>>(result);
    }

    ::jxx::Ptr<NavigableSet<K>> descendingKeySet() const override {
        auto result = ::jxx::NEW<TreeSet<K>>(comparator());
        for (auto iterator = map_.rbegin(); iterator != map_.rend(); ++iterator)
            result->add(iterator->first);
        return ::jxx::CAST<NavigableSet<K>>(result);
    }

    ::jxx::Ptr<NavigableMap<K, V>> subMap(
        const ::jxx::Ptr<K>& fromKey, ::jxx::lang::jbool fromInclusive,
        const ::jxx::Ptr<K>& toKey, ::jxx::lang::jbool toInclusive) const override {
        if (fromKey == nullptr || toKey == nullptr) throw ::jxx::lang::NullPointerException();
        KeyLess less{comparator_};
        if (less(toKey, fromKey)) throw ::jxx::lang::IllegalArgumentException();
        auto result = ::jxx::NEW<TreeMap<K, V>>(comparator_);
        for (const auto& entry : map_) {
            const bool above = fromInclusive ? !less(entry.first, fromKey) : less(fromKey, entry.first);
            const bool below = toInclusive ? !less(toKey, entry.first) : less(entry.first, toKey);
            if (above && below) result->put(entry.first, entry.second);
        }
        return ::jxx::CAST<NavigableMap<K, V>>(result);
    }

    ::jxx::Ptr<NavigableMap<K, V>> headMap(
        const ::jxx::Ptr<K>& toKey, ::jxx::lang::jbool inclusive) const override {
        if (toKey == nullptr) throw ::jxx::lang::NullPointerException();
        KeyLess less{comparator_}; auto result = ::jxx::NEW<TreeMap<K, V>>(comparator_);
        for (const auto& entry : map_)
            if (inclusive ? !less(toKey, entry.first) : less(entry.first, toKey)) result->put(entry.first, entry.second);
        return ::jxx::CAST<NavigableMap<K, V>>(result);
    }

    ::jxx::Ptr<NavigableMap<K, V>> tailMap(
        const ::jxx::Ptr<K>& fromKey, ::jxx::lang::jbool inclusive) const override {
        if (fromKey == nullptr) throw ::jxx::lang::NullPointerException();
        KeyLess less{comparator_}; auto result = ::jxx::NEW<TreeMap<K, V>>(comparator_);
        for (const auto& entry : map_)
            if (inclusive ? !less(entry.first, fromKey) : less(fromKey, entry.first)) result->put(entry.first, entry.second);
        return ::jxx::CAST<NavigableMap<K, V>>(result);
    }

    ::jxx::Ptr<SortedMap<K, V>> subMap(
        const ::jxx::Ptr<K>& fromKey, const ::jxx::Ptr<K>& toKey) const override {
        return ::jxx::CAST<SortedMap<K, V>>(subMap(fromKey, true, toKey, false));
    }
    ::jxx::Ptr<SortedMap<K, V>> headMap(const ::jxx::Ptr<K>& toKey) const override {
        return ::jxx::CAST<SortedMap<K, V>>(headMap(toKey, false));
    }
    ::jxx::Ptr<SortedMap<K, V>> tailMap(const ::jxx::Ptr<K>& fromKey) const override {
        return ::jxx::CAST<SortedMap<K, V>>(tailMap(fromKey, true));
    }

    virtual jxx::Ptr<Set<K>> keySet() override { return AbstractMap<K, V>::keySet(); }
    virtual jxx::Ptr<Collection<V>> values() override { return AbstractMap<K, V>::values(); }

    virtual jxx::Ptr<Set<MapEntry<K, V>>> entrySet() override {
        if (entrySetView_ == nullptr) entrySetView_ = jxx::CAST<Set<MapEntry<K, V>>>(jxx::NEW<EntrySet>(this));
        return entrySetView_;
    }

    virtual jxx::Ptr<jxx::lang::Object> clone() {
        auto cloned = jxx::NEW<TreeMap<K, V>>(comparator_);
        for (const auto& kv : map_) {
            cloned->map_.emplace(kv.first, kv.second);
        }
        return cloned;
    }

private:
    jxx::lang::jbool keysEqual(const jxx::Ptr<K>& a, const jxx::Ptr<K>& b) const {
        if (a == nullptr) return b == nullptr;
        if (b == nullptr) return false;
        if (comparator_ != nullptr) {
            return comparator_->compareSuper(a, b) == 0;
        }
        auto comparable = jxx::CAST<jxx::lang::Comparable<K>, jxx::lang::Object>(a);
        if (comparable == nullptr) throw ::jxx::lang::ClassCastException();
        return comparable->compareTo(b) == 0;
    }

protected:
    class EntryView : public virtual MapEntry<K, V> {
    private:
        TreeMap<K, V>* map_;
        jxx::Ptr<K> key_;
    public:
        EntryView(TreeMap<K, V>* map, const jxx::Ptr<K>& key) : map_(map), key_(key) {}
        virtual ~EntryView() = default;
        virtual jxx::Ptr<K> getKey() override { return key_; }
        virtual jxx::Ptr<V> getValue() override {
            auto it = map_->map_.find(key_);
            return it == map_->map_.end() ? nullptr : it->second;
        }
        virtual jxx::Ptr<V> setValue(const jxx::Ptr<V>& value) override { return map_->put(key_, value); }
        virtual jxx::lang::jbool equals(const jxx::Ptr<jxx::lang::Object>& o) const override {
            auto other = jxx::CAST<MapEntry<K, V>, jxx::lang::Object>(o);
            if (other == nullptr) return false;
            const auto& k1 = key_;
            auto v1 = map_->get(::jxx::CAST<::jxx::lang::Object>(key_));
            auto k2 = other->getKey();
            auto v2 = other->getValue();
            jxx::lang::jbool keyEqual = (k1 == nullptr) ? (k2 == nullptr) : k1->equals(k2);
            jxx::lang::jbool valueEqual = (v1 == nullptr) ? (v2 == nullptr) : v1->equals(v2);
            return keyEqual && valueEqual;
        }
        virtual jxx::lang::jint hashCode() const override {
            jxx::lang::jint kh = (key_ == nullptr) ? 0 : key_->hashCode();
            auto value = map_->get(::jxx::CAST<::jxx::lang::Object>(key_));
            jxx::lang::jint vh = (value == nullptr) ? 0 : value->hashCode();
            return kh ^ vh;
        }
    };

    jxx::Ptr<MapEntry<K, V>> makeEntryView(const jxx::Ptr<K>& key) const {
        return jxx::CAST<MapEntry<K, V>>(jxx::NEW<EntryView>(const_cast<TreeMap<K, V>*>(this), key));
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
            if (current_ == end_) throw ::jxx::util::NoSuchElementException();
            lastReturnedKey_ = current_->first;
            ++current_;
            canRemove_ = true;
            return map_->makeEntryView(lastReturnedKey_);
        }
        virtual void remove() override {
            if (!canRemove_) throw ::jxx::lang::IllegalStateException();
            checkForComodification();
            map_->remove(lastReturnedKey_);
            expectedModCount_ = map_->modCount_;
            canRemove_ = false;
            lastReturnedKey_ = nullptr;
        }
    private:
        void checkForComodification() {
            if (map_->modCount_ != expectedModCount_) throw ::jxx::util::ConcurrentModificationException();
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
        virtual jxx::lang::jbool contains(const jxx::Ptr<jxx::lang::Object>& o) override {
            auto e = jxx::CAST<MapEntry<K, V>, jxx::lang::Object>(o);
            if (e == nullptr) return false;
            auto value = map_->get(e->getKey());
            if (value == nullptr) return e->getValue() == nullptr && map_->containsKey(e->getKey());
            return value->equals(e->getValue());
        }
        virtual jxx::Ptr<Iterator<MapEntry<K, V>>> iterator() override {
            return jxx::CAST<Iterator<MapEntry<K, V>>>(jxx::NEW<EntryIterator>(map_));
        }
        virtual ::jxx::lang::ObjectArray toArray() override {
            return AbstractCollection<MapEntry<K, V>>::toArray();
        }
        virtual jxx::lang::jbool add(const jxx::Ptr<MapEntry<K, V>>& /*e*/) override { throw ::jxx::lang::UnsupportedOperationException(); }
        virtual jxx::lang::jbool remove(const jxx::Ptr<jxx::lang::Object>& o) override {
            auto e = jxx::CAST<MapEntry<K, V>, jxx::lang::Object>(o);
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
        virtual jxx::lang::jbool containsAll(const jxx::Ptr<wildcard::CollectionAny>& c) override { return AbstractCollection<MapEntry<K, V>>::containsAll(c); }
        virtual jxx::lang::jbool addAll(const jxx::Ptr<wildcard::CollectionExtends<MapEntry<K, V>>>& /*c*/) override { throw ::jxx::lang::UnsupportedOperationException(); }
        virtual jxx::lang::jbool removeAll(const jxx::Ptr<wildcard::CollectionAny>& c) override { return AbstractSet<MapEntry<K, V>>::removeAll(c); }
        virtual jxx::lang::jbool retainAll(const jxx::Ptr<wildcard::CollectionAny>& c) override { return AbstractCollection<MapEntry<K, V>>::retainAll(c); }
        virtual void clear() override { map_->clear(); }
    };
};

} // namespace util
} // namespace jxx
