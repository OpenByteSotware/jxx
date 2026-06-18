#pragma once

#include <unordered_map>
#include <utility>

#include "util/jxx.util.AbstractMap.h"
#include "lang/jxx.lang.Cloneable.h"
#include "io/jxx.io.Serializable.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx {
namespace util {

template <typename K, typename V>
class HashMap
    : public AbstractMap<K, V>
    , public virtual jxx::lang::Cloneable
    , public virtual jxx::io::Serializable {
protected:
    struct KeyHash {
        std::size_t operator()(const jxx::Ptr<K>& key) const {
            if (key == nullptr) return 0u;
            return static_cast<std::size_t>(key->hashCode());
        }
    };

    struct KeyEq {
        bool operator()(const jxx::Ptr<K>& a, const jxx::Ptr<K>& b) const {
            if (a == nullptr) return b == nullptr;
            if (b == nullptr) return false;
            return a->equals(b);
        }
    };

    using InternalMap = std::unordered_map<jxx::Ptr<K>, jxx::Ptr<V>, KeyHash, KeyEq>;

    static constexpr jint DEFAULT_INITIAL_CAPACITY = 16;
    static constexpr jfloat DEFAULT_LOAD_FACTOR = 0.75f;

    InternalMap map_;
    jfloat loadFactor_;
    jint threshold_;
    jint modCount_;
    jxx::Ptr<Set<MapEntry<K, V>>> entrySetView_;

public:
    HashMap()
        : map_()
        , loadFactor_(DEFAULT_LOAD_FACTOR)
        , threshold_(DEFAULT_INITIAL_CAPACITY)
        , modCount_(0)
        , entrySetView_(nullptr) {
        map_.reserve(static_cast<std::size_t>(DEFAULT_INITIAL_CAPACITY));
    }

    explicit HashMap(jint initialCapacity)
        : map_()
        , loadFactor_(DEFAULT_LOAD_FACTOR)
        , threshold_(initialCapacity > 0 ? initialCapacity : 0)
        , modCount_(0)
        , entrySetView_(nullptr) {
        if (initialCapacity < 0) throw jxx::lang::IllegalArgumentException("cannot be less than zero");
        map_.reserve(static_cast<std::size_t>(initialCapacity));
    }

    HashMap(jint initialCapacity, jfloat loadFactor)
        : map_()
        , loadFactor_(loadFactor)
        , threshold_(initialCapacity > 0 ? initialCapacity : 0)
        , modCount_(0)
        , entrySetView_(nullptr) {
        if (initialCapacity < 0) throw jxx::lang::IllegalArgumentException("cannot be less than zero");
        if (!(loadFactor > 0.0f) || loadFactor != loadFactor) throw IllegalArgumentException();
        map_.reserve(static_cast<std::size_t>(initialCapacity));
    }

    explicit HashMap(jxx::Ptr<Map<K, V>> m)
        : HashMap() {
        if (m == nullptr) throw jxx::lang::NullPointerException();
        putAll(m);
    }

    virtual ~HashMap() = default;

    virtual jxx::lang::jint size() override { return static_cast<jxx::lang::jint>(map_.size()); }
    virtual jxx::lang::jbool isEmpty() override { return map_.empty(); }

    virtual jxx::lang::jbool containsKey(jxx::Ptr<jxx::lang::Object> key) override {
        return map_.find(jxx::lang::ptr_checked_cast<K>(key)) != map_.end();
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
        auto it = map_.find(castKey);
        if (it == map_.end()) return nullptr;
        afterNodeAccess(castKey);
        return it->second;
    }

    virtual jxx::Ptr<V> put(jxx::Ptr<K> key, jxx::Ptr<V> value) override {
        auto it = map_.find(key);
        if (it == map_.end()) {
            map_.emplace(key, value);
            ++modCount_;
            afterNodeInsertion(key, true);
            resizeIfNeeded();
            return nullptr;
        }
        jxx::Ptr<V> oldValue = it->second;
        it->second = value;
        afterNodeAccess(key);
        return oldValue;
    }

    virtual jxx::Ptr<V> remove(jxx::Ptr<jxx::lang::Object> key) override {
        auto castKey = jxx::lang::ptr_checked_cast<K>(key);
        auto it = map_.find(castKey);
        if (it == map_.end()) return nullptr;
        jxx::Ptr<V> oldValue = it->second;
        map_.erase(it);
        ++modCount_;
        afterNodeRemoval(castKey);
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
            afterClear();
        }
    }

    virtual jxx::Ptr<Set<MapEntry<K, V>>> entrySet() override {
        if (entrySetView_ == nullptr) entrySetView_ = createEntrySetView();
        return entrySetView_;
    }

    virtual jxx::Ptr<jxx::lang::Object> clone() {
        jxx::Ptr<HashMap<K, V>> cloned(new HashMap<K, V>(static_cast<jint>(map_.size()), loadFactor_));
        for (const auto& kv : map_) cloned->map_.emplace(kv.first, kv.second);
        cloned->threshold_ = threshold_;
        return cloned;
    }

protected:
    virtual void afterNodeAccess(jxx::Ptr<K> /*key*/) {}
    virtual void afterNodeInsertion(jxx::Ptr<K> /*key*/, jbool /*isNewKey*/) {}
    virtual void afterNodeRemoval(jxx::Ptr<K> /*key*/) {}
    virtual void afterClear() {}

    void resizeIfNeeded() {
        if (size() > threshold_) {
            jint newThreshold = threshold_ <= 0 ? DEFAULT_INITIAL_CAPACITY : threshold_ << 1;
            threshold_ = newThreshold;
            map_.reserve(static_cast<std::size_t>(newThreshold));
        }
    }

    class EntryView : public virtual MapEntry<K, V> {
    private:
        HashMap<K, V>* map_;
        jxx::Ptr<K> key_;
    public:
        EntryView(HashMap<K, V>* map, jxx::Ptr<K> key) : map_(map), key_(key) {}
        virtual ~EntryView() = default;
        virtual jxx::Ptr<K> getKey() override { return key_; }
        virtual jxx::Ptr<V> getValue() override { return map_->get(key_); }
        virtual jxx::Ptr<V> setValue(jxx::Ptr<V> value) override { return map_->put(key_, value); }
        virtual jbool equals(jxx::Ptr<jxx::lang::Object> o) override {
            auto other = jxx::lang::ptr_checked_cast<MapEntry<K, V>>(o);
            if (other == nullptr) return false;
            auto k1 = getKey();
            auto v1 = getValue();
            auto k2 = other->getKey();
            auto v2 = other->getValue();
            jbool keyEqual = (k1 == nullptr) ? (k2 == nullptr) : k1->equals(k2);
            jbool valueEqual = (v1 == nullptr) ? (v2 == nullptr) : v1->equals(v2);
            return keyEqual && valueEqual;
        }
        virtual jint hashCode() override {
            jint kh = (key_ == nullptr) ? 0 : key_->hashCode();
            jxx::Ptr<V> value = getValue();
            jint vh = (value == nullptr) ? 0 : value->hashCode();
            return kh ^ vh;
        }
    };

    virtual jxx::Ptr<MapEntry<K, V>> makeEntryView(jxx::Ptr<K> key) {
        return jxx::Ptr<MapEntry<K, V>>(new EntryView(this, key));
    }

    class EntryIterator : public virtual Iterator<MapEntry<K, V>> {
    protected:
        HashMap<K, V>* map_;
        typename InternalMap::iterator current_;
        typename InternalMap::iterator end_;
        jxx::Ptr<K> lastReturnedKey_;
        jbool canRemove_;
        jint expectedModCount_;
    public:
        explicit EntryIterator(HashMap<K, V>* map)
            : map_(map)
            , current_(map->map_.begin())
            , end_(map->map_.end())
            , lastReturnedKey_(nullptr)
            , canRemove_(false)
            , expectedModCount_(map->modCount_) {}
        virtual ~EntryIterator() = default;
        virtual jbool hasNext() override { return current_ != end_; }
        virtual jxx::Ptr<MapEntry<K, V>> next() override {
            checkForComodification();
            if (current_ == end_) throw NoSuchElementException();
            jxx::Ptr<K> key = current_->first;
            ++current_;
            lastReturnedKey_ = key;
            canRemove_ = true;
            return map_->makeEntryView(key);
        }
        virtual void remove() override {
            if (!canRemove_) throw IllegalStateException();
            checkForComodification();
            map_->remove(lastReturnedKey_);
            expectedModCount_ = map_->modCount_;
            canRemove_ = false;
            lastReturnedKey_ = nullptr;
        }
    protected:
        void checkForComodification() {
            if (map_->modCount_ != expectedModCount_) throw ConcurrentModificationException();
        }
    };

    class EntrySet : public AbstractSet<MapEntry<K, V>> {
    protected:
        HashMap<K, V>* map_;
    public:
        explicit EntrySet(HashMap<K, V>* map) : map_(map) {}
        virtual ~EntrySet() = default;
        virtual jint size() override { return map_->size(); }
        virtual jbool isEmpty() override { return map_->isEmpty(); }
        virtual jbool contains(jxx::Ptr<jxx::lang::Object> o) override {
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
        virtual jbool add(jxx::Ptr<MapEntry<K, V>> /*e*/) override { throw UnsupportedOperationException(); }
        virtual jbool remove(jxx::Ptr<jxx::lang::Object> o) override {
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
        virtual jbool containsAll(jxx::Ptr<wildcard::CollectionAny> c) override { return AbstractCollection<MapEntry<K, V>>::containsAll(c); }
        virtual jbool addAll(jxx::Ptr<wildcard::CollectionExtends<MapEntry<K, V>>> /*c*/) override { throw UnsupportedOperationException(); }
        virtual jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) override { return AbstractSet<MapEntry<K, V>>::removeAll(c); }
        virtual jbool retainAll(jxx::Ptr<wildcard::CollectionAny> c) override { return AbstractCollection<MapEntry<K, V>>::retainAll(c); }
        virtual void clear() override { map_->clear(); }
    };

    virtual jxx::Ptr<Set<MapEntry<K, V>>> createEntrySetView() {
        return jxx::Ptr<Set<MapEntry<K, V>>>(new EntrySet(this));
    }
};

} // namespace util
} // namespace jxx
