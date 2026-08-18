#pragma once

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

#include "io/jxx.io.Serializable.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.AbstractCollection.h"
#include "util/jxx.util.AbstractSet.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.Dictionary.h"
#include "util/jxx.util.Enumeration.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.Map.h"
#include "util/jxx.util.MapEntry.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.Spliterator.h"
#include "util/jxx.util.wildcard.CollectionAny.h"
#include "util/jxx.util.wildcard.CollectionExtends.h"

namespace jxx::util {

/**
 * Java 8 style Hashtable<K,V> for JXX/C++17.
 *
 * - Dictionary<K,V> is the concrete Object-bearing class branch.
 * - Map<K,V>, Cloneable and Serializable are interfaces.
 * - Keys and values must be non-null.
 * - All public object parameters use jxx::Ptr<T>.
 * - STL storage is private.
 * - Public operations use Object::synchronized(...).
 */
template <typename K, typename V>
class Hashtable
    : public Dictionary<K, V>
    , public virtual Map<K, V>
    , public virtual jxx::lang::Cloneable
    , public virtual jxx::io::Serializable {
private:
    struct Record {
        jxx::lang::jint hash;
        jxx::Ptr<K> key;
        jxx::Ptr<V> value;
    };

    static constexpr jxx::lang::jint DEFAULT_INITIAL_CAPACITY = 11;
    static constexpr jxx::lang::jfloat DEFAULT_LOAD_FACTOR = 0.75f;

    std::vector<std::vector<Record>> buckets_;
    jxx::lang::jint size_ = 0;
    jxx::lang::jint threshold_ = 0;
    jxx::lang::jfloat loadFactor_ = DEFAULT_LOAD_FACTOR;
    jxx::lang::jint modCount_ = 0;

    jxx::Ptr<Set<K>> keySet_;
    jxx::Ptr<Collection<V>> values_;
    jxx::Ptr<Set<MapEntry<K, V>>> entrySet_;

    static jxx::Ptr<jxx::lang::Object> object_(const jxx::Ptr<K>& value) {
        return jxx::CAST<jxx::lang::Object>(value);
    }

    static jxx::Ptr<jxx::lang::Object> valueObject_(const jxx::Ptr<V>& value) {
        return jxx::CAST<jxx::lang::Object>(value);
    }

    static jxx::lang::jint hash_(const jxx::Ptr<jxx::lang::Object>& value) {
        return value == nullptr ? 0 : value->hashCode();
    }

    static jxx::lang::jbool equalObjects_(
        const jxx::Ptr<jxx::lang::Object>& left,
        const jxx::Ptr<jxx::lang::Object>& right) {
        if (left == nullptr || right == nullptr) return left == right;
        return left->equals(right);
    }

    static std::size_t index_(jxx::lang::jint hash, std::size_t capacity) {
        const auto unsignedHash = static_cast<std::uint32_t>(hash);
        return static_cast<std::size_t>(unsignedHash % capacity);
    }

    void updateThreshold_() {
        const auto value = static_cast<jxx::lang::jint>(
            static_cast<jxx::lang::jfloat>(buckets_.size()) * loadFactor_);
        threshold_ = value > 0 ? value : 1;
    }

    Record* find_(const jxx::Ptr<jxx::lang::Object>& key) {
        if (key == nullptr) return nullptr;
        const auto hash = hash_(key);
        auto& bucket = buckets_[index_(hash, buckets_.size())];
        for (auto& record : bucket) {
            if (record.hash == hash && equalObjects_(object_(record.key), key)) return &record;
        }
        return nullptr;
    }

    const Record* find_(const jxx::Ptr<jxx::lang::Object>& key) const {
        if (key == nullptr) return nullptr;
        const auto hash = hash_(key);
        const auto& bucket = buckets_[index_(hash, buckets_.size())];
        for (const auto& record : bucket) {
            if (record.hash == hash && equalObjects_(object_(record.key), key)) return &record;
        }
        return nullptr;
    }

    void insertNew_(jxx::Ptr<K> key, jxx::Ptr<V> value) {
        if (size_ >= threshold_) rehash();
        const auto hash = hash_(object_(key));
        buckets_[index_(hash, buckets_.size())].push_back(Record{hash, key, value});
        ++size_;
        ++modCount_;
    }

    template <typename E>
    class SnapshotEnumeration final : public virtual Enumeration<E> {
        std::vector<jxx::Ptr<E>> values_;
        std::size_t index_ = 0;
    public:
        explicit SnapshotEnumeration(std::vector<jxx::Ptr<E>> values)
            : values_(std::move(values)) {}
        jxx::lang::jbool hasMoreElements() override { return index_ < values_.size(); }
        jxx::Ptr<E> nextElement() override {
            if (index_ >= values_.size()) throw NoSuchElementException();
            return values_[index_++];
        }
    };

    enum class IterationKind { Key, Value, Entry };

    template <typename E>
    class ViewIterator final : public virtual Iterator<E> {
        Hashtable<K,V>* owner_;
        std::vector<jxx::Ptr<E>> snapshot_;
        std::size_t index_ = 0;
        jxx::Ptr<jxx::lang::Object> lastKey_;
        jxx::lang::jbool canRemove_ = false;
        jxx::lang::jint expectedModCount_;
    public:
        ViewIterator(Hashtable<K,V>* owner, std::vector<jxx::Ptr<E>> snapshot)
            : owner_(owner), snapshot_(std::move(snapshot)), expectedModCount_(owner->modCount_) {}
        jxx::lang::jbool hasNext() override { return index_ < snapshot_.size(); }
        jxx::Ptr<E> next() override {
            check_();
            if (index_ >= snapshot_.size()) throw NoSuchElementException();
            auto result = snapshot_[index_++];
            auto entry = jxx::CAST<MapEntry<K,V>>(jxx::CAST<jxx::lang::Object>(result));
            lastKey_ = entry == nullptr
                ? jxx::CAST<jxx::lang::Object>(result)
                : jxx::CAST<jxx::lang::Object>(entry->getKey());
            canRemove_ = true;
            return result;
        }
        void remove() override {
            if (!canRemove_) throw jxx::lang::IllegalStateException();
            check_();
            owner_->remove(lastKey_);
            expectedModCount_ = owner_->modCount_;
            canRemove_ = false;
            lastKey_ = nullptr;
        }
    private:
        void check_() const {
            if (owner_->modCount_ != expectedModCount_) throw ConcurrentModificationException();
        }
    };

    class EntryView final : public virtual MapEntry<K,V> {
        Hashtable<K,V>* owner_;
        jxx::Ptr<K> key_;
    public:
        EntryView(Hashtable<K,V>* owner, jxx::Ptr<K> key) : owner_(owner), key_(key) {}
        jxx::Ptr<K> getKey() override { return key_; }
        jxx::Ptr<V> getValue() override { return owner_->get(object_(key_)); }
        jxx::Ptr<V> setValue(jxx::Ptr<V> value) override {
            if (value == nullptr) throw jxx::lang::NullPointerException();
            return owner_->put(key_, value);
        }
        jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> value) override {
            auto other = jxx::CAST<MapEntry<K,V>>(value);
            return other != nullptr &&
                equalObjects_(object_(key_), jxx::CAST<jxx::lang::Object>(other->getKey())) &&
                equalObjects_(valueObject_(getValue()), jxx::CAST<jxx::lang::Object>(other->getValue()));
        }
        jxx::lang::jint hashCode() override {
            return hash_(object_(key_)) ^ hash_(valueObject_(getValue()));
        }
    };

    class KeySetView final : public virtual AbstractSet<K> {
        Hashtable<K,V>* owner_;
    public:
        explicit KeySetView(Hashtable<K,V>* owner) : owner_(owner) {
            if (owner_ == nullptr) throw jxx::lang::NullPointerException();
        }
        virtual ~KeySetView() = default;
        jxx::lang::jint size() override { return owner_->size(); }
        jxx::lang::jbool isEmpty() override { return owner_->isEmpty(); }
        jxx::Ptr<Iterator<K>> iterator() override { return owner_->keyIterator_(); }
        jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> o) override { return owner_->containsKey(o); }
        jxx::lang::ObjectArray toArray() override { return AbstractCollection<K>::toArray(); }
        jxx::lang::jbool add(jxx::Ptr<K>) override { throw jxx::lang::UnsupportedOperationException(); }
        jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> o) override {
            if (!owner_->containsKey(o)) return false;
            owner_->remove(o); return true;
        }
        jxx::lang::jbool containsAll(jxx::Ptr<wildcard::CollectionAny> c) override {
            return AbstractCollection<K>::containsAll(c);
        }
        jxx::lang::jbool addAll(jxx::Ptr<wildcard::CollectionExtends<K>>) override {
            throw jxx::lang::UnsupportedOperationException();
        }
        jxx::lang::jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) override {
            return AbstractSet<K>::removeAll(c);
        }
        jxx::lang::jbool retainAll(jxx::Ptr<wildcard::CollectionAny> c) override {
            return AbstractCollection<K>::retainAll(c);
        }
        void clear() override { owner_->clear(); }
        jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> o) override {
            return AbstractSet<K>::equals(o);
        }
        jxx::lang::jint hashCode() override { return AbstractSet<K>::hashCode(); }
        jxx::Ptr<Spliterator<K>> spliterator() override {
            throw jxx::lang::UnsupportedOperationException();
        }
    };

    class ValuesView final : public virtual AbstractCollection<V> {
        Hashtable<K,V>* owner_;
    public:
        explicit ValuesView(Hashtable<K,V>* owner) : owner_(owner) {
            if (owner_ == nullptr) throw jxx::lang::NullPointerException();
        }
        virtual ~ValuesView() = default;
        jxx::lang::jint size() override { return owner_->size(); }
        jxx::lang::jbool isEmpty() override { return owner_->isEmpty(); }
        jxx::Ptr<Iterator<V>> iterator() override { return owner_->valueIterator_(); }
        jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> o) override { return owner_->containsValue(o); }
        jxx::lang::ObjectArray toArray() override { return AbstractCollection<V>::toArray(); }
        jxx::lang::jbool add(jxx::Ptr<V>) override { throw jxx::lang::UnsupportedOperationException(); }
        jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> o) override { return owner_->removeFirstValue_(o); }
        jxx::lang::jbool containsAll(jxx::Ptr<wildcard::CollectionAny> c) override {
            return AbstractCollection<V>::containsAll(c);
        }
        jxx::lang::jbool addAll(jxx::Ptr<wildcard::CollectionExtends<V>>) override {
            throw jxx::lang::UnsupportedOperationException();
        }
        jxx::lang::jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) override {
            return AbstractCollection<V>::removeAll(c);
        }
        jxx::lang::jbool retainAll(jxx::Ptr<wildcard::CollectionAny> c) override {
            return AbstractCollection<V>::retainAll(c);
        }
        void clear() override { owner_->clear(); }
        jxx::Ptr<Spliterator<V>> spliterator() override {
            throw jxx::lang::UnsupportedOperationException();
        }
    };

    class EntrySetView final : public virtual AbstractSet<MapEntry<K,V>> {
        Hashtable<K,V>* owner_;
    public:
        explicit EntrySetView(Hashtable<K,V>* owner) : owner_(owner) {
            if (owner_ == nullptr) throw jxx::lang::NullPointerException();
        }
        virtual ~EntrySetView() = default;
        jxx::lang::jint size() override { return owner_->size(); }
        jxx::lang::jbool isEmpty() override { return owner_->isEmpty(); }
        jxx::Ptr<Iterator<MapEntry<K,V>>> iterator() override { return owner_->entryIterator_(); }
        jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> o) override {
            auto entry = jxx::CAST<MapEntry<K,V>>(o);
            if (entry == nullptr) return false;
            auto key = jxx::CAST<jxx::lang::Object>(entry->getKey());
            auto current = owner_->get(key);
            return current != nullptr && equalObjects_(valueObject_(current),
                jxx::CAST<jxx::lang::Object>(entry->getValue()));
        }
        jxx::lang::ObjectArray toArray() override {
            return AbstractCollection<MapEntry<K,V>>::toArray();
        }
        jxx::lang::jbool add(jxx::Ptr<MapEntry<K,V>>) override {
            throw jxx::lang::UnsupportedOperationException();
        }
        jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> o) override {
            auto entry = jxx::CAST<MapEntry<K,V>>(o);
            if (entry == nullptr) return false;
            return owner_->remove(jxx::CAST<jxx::lang::Object>(entry->getKey()),
                                  jxx::CAST<jxx::lang::Object>(entry->getValue()));
        }
        jxx::lang::jbool containsAll(jxx::Ptr<wildcard::CollectionAny> c) override {
            return AbstractCollection<MapEntry<K,V>>::containsAll(c);
        }
        jxx::lang::jbool addAll(
            jxx::Ptr<wildcard::CollectionExtends<MapEntry<K,V>>>) override {
            throw jxx::lang::UnsupportedOperationException();
        }
        jxx::lang::jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) override {
            return AbstractSet<MapEntry<K,V>>::removeAll(c);
        }
        jxx::lang::jbool retainAll(jxx::Ptr<wildcard::CollectionAny> c) override {
            return AbstractCollection<MapEntry<K,V>>::retainAll(c);
        }
        void clear() override { owner_->clear(); }
        jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> o) override {
            return AbstractSet<MapEntry<K,V>>::equals(o);
        }
        jxx::lang::jint hashCode() override {
            return AbstractSet<MapEntry<K,V>>::hashCode();
        }
        jxx::Ptr<Spliterator<MapEntry<K,V>>> spliterator() override {
            throw jxx::lang::UnsupportedOperationException();
        }
    };

public:
    Hashtable() : Hashtable(DEFAULT_INITIAL_CAPACITY, DEFAULT_LOAD_FACTOR) {}

    explicit Hashtable(jxx::lang::jint initialCapacity)
        : Hashtable(initialCapacity, DEFAULT_LOAD_FACTOR) {}

    Hashtable(jxx::lang::jint initialCapacity, jxx::lang::jfloat loadFactor)
        : buckets_(static_cast<std::size_t>(initialCapacity == 0 ? 1 : initialCapacity)),
          loadFactor_(loadFactor) {
        if (initialCapacity < 0 || !(loadFactor > 0.0f) || std::isnan(loadFactor))
            throw jxx::lang::IllegalArgumentException();
        updateThreshold_();
    }

    explicit Hashtable(jxx::Ptr<Map<K,V>> source) : Hashtable() {
        if (source == nullptr) throw jxx::lang::NullPointerException();
        putAll(source);
    }

    virtual ~Hashtable() = default;

    jxx::lang::jint size() override {
        return this->synchronized([&]{ return size_; });
    }

    jxx::lang::jbool isEmpty() override {
        return this->synchronized([&]{ return static_cast<jxx::lang::jbool>(size_ == 0); });
    }

    Enumeration<K>* unsupportedRawEnumeration_() = delete;

    jxx::Ptr<Enumeration<K>> keys() override {
        auto snapshot = this->synchronized([&]{
            std::vector<jxx::Ptr<K>> out; out.reserve(static_cast<std::size_t>(size_));
            for (const auto& b : buckets_) for (const auto& r : b) out.push_back(r.key);
            return out;
        });
        return jxx::NEW<SnapshotEnumeration<K>>(std::move(snapshot));
    }

    jxx::Ptr<Enumeration<V>> elements() override {
        auto snapshot = this->synchronized([&]{
            std::vector<jxx::Ptr<V>> out; out.reserve(static_cast<std::size_t>(size_));
            for (const auto& b : buckets_) for (const auto& r : b) out.push_back(r.value);
            return out;
        });
        return jxx::NEW<SnapshotEnumeration<V>>(std::move(snapshot));
    }

    jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> value) { return containsValue(value); }

    jxx::lang::jbool containsValue(jxx::Ptr<jxx::lang::Object> value) override {
        if (value == nullptr) throw jxx::lang::NullPointerException();
        return this->synchronized([&]{
            for (const auto& b : buckets_) for (const auto& r : b)
                if (equalObjects_(valueObject_(r.value), value)) return static_cast<jxx::lang::jbool>(true);
            return static_cast<jxx::lang::jbool>(false);
        });
    }

    jxx::lang::jbool containsKey(jxx::Ptr<jxx::lang::Object> key) override {
        return this->synchronized([&]{ return static_cast<jxx::lang::jbool>(find_(key) != nullptr); });
    }

    jxx::Ptr<V> get(jxx::Ptr<jxx::lang::Object> key) override {
        return this->synchronized([&]{ auto* r=find_(key); return r==nullptr?jxx::Ptr<V>(nullptr):r->value; });
    }

    jxx::Ptr<V> put(jxx::Ptr<K> key, jxx::Ptr<V> value) override {
        if (key == nullptr || value == nullptr) throw jxx::lang::NullPointerException();
        return this->synchronized([&]{
            auto* r=find_(object_(key));
            if(r!=nullptr){auto old=r->value;r->value=value;return old;}
            insertNew_(key,value);return jxx::Ptr<V>(nullptr);
        });
    }

    jxx::Ptr<V> remove(jxx::Ptr<jxx::lang::Object> key) override {
        return this->synchronized([&]{
            if(key==nullptr)return jxx::Ptr<V>(nullptr);
            const auto h=hash_(key);auto& b=buckets_[index_(h,buckets_.size())];
            for(auto i=b.begin();i!=b.end();++i)if(i->hash==h&&equalObjects_(object_(i->key),key)){
                auto old=i->value;b.erase(i);--size_;++modCount_;return old;}
            return jxx::Ptr<V>(nullptr);
        });
    }

    void putAll(jxx::Ptr<Map<K,V>> source) override {
        if(source==nullptr)throw jxx::lang::NullPointerException();
        auto it=source->entrySet()->iterator();while(it->hasNext()){auto e=it->next();put(e->getKey(),e->getValue());}
    }

    void clear() override {
        this->synchronized([&]{if(size_!=0){for(auto& b:buckets_)b.clear();size_=0;++modCount_;}});
    }

    jxx::Ptr<Set<K>> keySet() override {
        return this->synchronized([&]{if(keySet_==nullptr)keySet_=jxx::NEW<KeySetView>(this);return keySet_;});
    }

    jxx::Ptr<Collection<V>> values() override {
        return this->synchronized([&]{if(values_==nullptr)values_=jxx::NEW<ValuesView>(this);return values_;});
    }

    jxx::Ptr<Set<MapEntry<K,V>>> entrySet() override {
        return this->synchronized([&]{if(entrySet_==nullptr)entrySet_=jxx::NEW<EntrySetView>(this);return entrySet_;});
    }

    jxx::Ptr<V> getOrDefault(jxx::Ptr<jxx::lang::Object> key,jxx::Ptr<V> defaultValue) {
        auto value=get(key);return value==nullptr?defaultValue:value;
    }

    jxx::Ptr<V> putIfAbsent(jxx::Ptr<K> key,jxx::Ptr<V> value) {
        if(key==nullptr||value==nullptr)throw jxx::lang::NullPointerException();
        return this->synchronized([&]{auto* r=find_(object_(key));if(r!=nullptr)return r->value;insertNew_(key,value);return jxx::Ptr<V>(nullptr);});
    }

    jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> key,jxx::Ptr<jxx::lang::Object> value) {
        if(key==nullptr||value==nullptr)return false;
        return this->synchronized([&]{auto* r=find_(key);if(r==nullptr||!equalObjects_(valueObject_(r->value),value))return static_cast<jxx::lang::jbool>(false);remove(key);return static_cast<jxx::lang::jbool>(true);});
    }

    jxx::lang::jbool replace(jxx::Ptr<K> key,jxx::Ptr<V> oldValue,jxx::Ptr<V> newValue) {
        if(key==nullptr||oldValue==nullptr||newValue==nullptr)throw jxx::lang::NullPointerException();
        return this->synchronized([&]{auto* r=find_(object_(key));if(r==nullptr||!equalObjects_(valueObject_(r->value),valueObject_(oldValue)))return static_cast<jxx::lang::jbool>(false);r->value=newValue;return static_cast<jxx::lang::jbool>(true);});
    }

    jxx::Ptr<V> replace(jxx::Ptr<K> key,jxx::Ptr<V> value) {
        if(key==nullptr||value==nullptr)throw jxx::lang::NullPointerException();
        return this->synchronized([&]{auto* r=find_(object_(key));if(r==nullptr)return jxx::Ptr<V>(nullptr);auto old=r->value;r->value=value;return old;});
    }

    jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> object) override {
        if(object==nullptr)return false;if(object.get()==this)return true;
        auto other=jxx::CAST<Map<K,V>>(object);if(other==nullptr||other->size()!=size())return false;
        auto it=entrySet()->iterator();while(it->hasNext()){auto e=it->next();auto v=other->get(jxx::CAST<jxx::lang::Object>(e->getKey()));if(!equalObjects_(valueObject_(e->getValue()),valueObject_(v)))return false;}return true;
    }

    jxx::lang::jint hashCode() override {
        return this->synchronized([&]{jxx::lang::jint h=0;for(const auto& b:buckets_)for(const auto&r:b)h+=hash_(object_(r.key))^hash_(valueObject_(r.value));return h;});
    }

    jxx::Ptr<jxx::lang::Object> clone() {
        auto copy=jxx::NEW<Hashtable<K,V>>(static_cast<jxx::lang::jint>(buckets_.size()),loadFactor_);
        this->synchronized([&]{for(const auto&b:buckets_)for(const auto&r:b)copy->put(r.key,r.value);});
        return jxx::CAST<jxx::lang::Object>(copy);
    }

    void writeObject(jxx::Ptr<jxx::io::ObjectOutputStream> out) override {
        if(out==nullptr)throw jxx::lang::NullPointerException();
        throw jxx::lang::UnsupportedOperationException();
    }
    void readObject(jxx::Ptr<jxx::io::ObjectInputStream> in) override {
        if(in==nullptr)throw jxx::lang::NullPointerException();
        throw jxx::lang::UnsupportedOperationException();
    }
    void readObjectNoData() override { clear(); }

protected:
    virtual void rehash() {
        const auto newCapacity=buckets_.size()*2U+1U;
        std::vector<std::vector<Record>> replacement(newCapacity);
        for(const auto&b:buckets_)for(const auto&r:b)replacement[index_(r.hash,newCapacity)].push_back(r);
        buckets_.swap(replacement);updateThreshold_();
    }

    jxx::Ptr<jxx::lang::Object> cloneImpl() const override {
        throw jxx::lang::UnsupportedOperationException();
    }

private:
    jxx::Ptr<Iterator<K>> keyIterator_() {
        std::vector<jxx::Ptr<K>> out;out.reserve(static_cast<std::size_t>(size_));for(const auto&b:buckets_)for(const auto&r:b)out.push_back(r.key);
        return jxx::NEW<ViewIterator<K>>(this,std::move(out));
    }
    jxx::Ptr<Iterator<V>> valueIterator_() {
        std::vector<jxx::Ptr<V>> out;out.reserve(static_cast<std::size_t>(size_));for(const auto&b:buckets_)for(const auto&r:b)out.push_back(r.value);
        return jxx::NEW<ViewIterator<V>>(this,std::move(out));
    }
    jxx::Ptr<Iterator<MapEntry<K,V>>> entryIterator_() {
        std::vector<jxx::Ptr<MapEntry<K,V>>> out;out.reserve(static_cast<std::size_t>(size_));for(const auto&b:buckets_)for(const auto&r:b)out.push_back(jxx::NEW<EntryView>(this,r.key));
        return jxx::NEW<ViewIterator<MapEntry<K,V>>>(this,std::move(out));
    }
    jxx::lang::jbool removeFirstValue_(jxx::Ptr<jxx::lang::Object> value) {
        if(value==nullptr)return false;
        return this->synchronized([&]{for(auto&b:buckets_)for(auto i=b.begin();i!=b.end();++i)if(equalObjects_(valueObject_(i->value),value)){b.erase(i);--size_;++modCount_;return static_cast<jxx::lang::jbool>(true);}return static_cast<jxx::lang::jbool>(false);});
    }
};

} // namespace jxx::util
