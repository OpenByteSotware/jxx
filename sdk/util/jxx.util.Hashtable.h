#pragma once

#include <algorithm>
#include <cmath>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include "lang/jxx.lang.Object.h"
#include "io/jxx.io.Serializable.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.Collection.h"
#include "util/jxx.util.Dictionary.h"
#include "util/jxx.util.Enumeration.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.Map.h"
#include "util/jxx.util.MapEntry.h"
#include "util/jxx.util.Set.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"

namespace jxx {
namespace util {

template <typename K, typename V>
class Hashtable
    : public Dictionary<K, V>
    , public virtual Map<K, V>
    , public virtual jxx::lang::Cloneable
    , public virtual jxx::io::Serializable {
private:
    struct EntryRecord {
        jxx::lang::jint hash;
        jxx::Ptr<K> key;
        jxx::Ptr<V> value;
    };

    std::vector<std::vector<EntryRecord>> buckets_;
    jxx::lang::jint count_;
    jxx::lang::jint threshold_;
    float loadFactor_;

    static constexpr jxx::lang::jint DEFAULT_INITIAL_CAPACITY = 11;
    static constexpr float DEFAULT_LOAD_FACTOR = 0.75f;

    static inline std::size_t bucketIndexFor(jxx::lang::jint hash, std::size_t cap) {
        const long long h = static_cast<long long>(hash);
        const long long c = static_cast<long long>(cap);
        const long long idx = ((h % c) + c) % c;
        return static_cast<std::size_t>(idx);
    }

    static inline jxx::lang::jbool objEquals(jxx::Ptr<jxx::lang::Object> a, jxx::Ptr<jxx::lang::Object> b) {
        if (a == nullptr) return static_cast<jxx::lang::jbool>(b == nullptr);
        return static_cast<jxx::lang::jbool>(a->equals(b));
    }

    static inline jxx::lang::jint objectHash(jxx::Ptr<jxx::lang::Object> o) {
        return (o == nullptr) ? static_cast<jxx::lang::jint>(0) : o->hashCode();
    }

    inline void recomputeThreshold() {
        threshold_ = static_cast<jxx::lang::jint>(static_cast<float>(buckets_.size()) * loadFactor_);
        if (threshold_ <= 0) threshold_ = 1;
    }

    inline EntryRecord* findEntryUnlocked(jxx::Ptr<jxx::lang::Object> keyObj) {
        if (keyObj == nullptr) return nullptr;
        const auto h = objectHash(keyObj);
        auto& bucket = buckets_[bucketIndexFor(h, buckets_.size())];
        for (auto& entry : bucket) {
            if (entry.hash == h && objEquals(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(entry.key), keyObj)) return &entry;
        }
        return nullptr;
    }

    inline const EntryRecord* findEntryUnlocked(jxx::Ptr<jxx::lang::Object> keyObj) const {
        if (keyObj == nullptr) return nullptr;
        const auto h = objectHash(keyObj);
        const auto& bucket = buckets_[bucketIndexFor(h, buckets_.size())];
        for (const auto& entry : bucket) {
            if (entry.hash == h && objEquals(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(entry.key), keyObj)) return &entry;
        }
        return nullptr;
    }

    inline void insertNewUnlocked(jxx::Ptr<K> key, jxx::Ptr<V> value) {
        const auto h = objectHash(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(key));
        buckets_[bucketIndexFor(h, buckets_.size())].push_back(EntryRecord{h, key, value});
        ++count_;
    }

    inline void ensureCapacityForInsertUnlocked() {
        if (count_ < threshold_) return;
        rehash();
    }

    template <typename E>
    class SnapshotIterator : public virtual Iterator<E> {
    private:
        std::vector<jxx::Ptr<E>> snapshot_;
        std::size_t index_;
    public:
        explicit SnapshotIterator(std::vector<jxx::Ptr<E>> snapshot)
            : snapshot_(std::move(snapshot)), index_(0) {}
        virtual ~SnapshotIterator() = default;
        virtual jxx::lang::jbool hasNext() override {
            return static_cast<jxx::lang::jbool>(index_ < snapshot_.size());
        }
        virtual jxx::Ptr<E> next() override {
            if (index_ >= snapshot_.size()) throw jxx::util::NoSuchElementException();
            return snapshot_[index_++];
        }

        virtual void remove() override {
            throw jxx::lang::UnsupportedOperationException();
        }

    };

    template <typename E>
    class SnapshotEnumeration : public virtual Enumeration<E> {
    private:
        std::vector<jxx::Ptr<E>> snapshot_;
        std::size_t index_;
    public:
        explicit SnapshotEnumeration(std::vector<jxx::Ptr<E>> snapshot)
            : snapshot_(std::move(snapshot)), index_(0) {}
        virtual ~SnapshotEnumeration() = default;
        virtual jxx::lang::jbool hasMoreElements() override {
            return static_cast<jxx::lang::jbool>(index_ < snapshot_.size());
        }
        virtual jxx::Ptr<E> nextElement() override {
            if (index_ >= snapshot_.size()) throw NoSuchElementException();
            return snapshot_[index_++];
        }
    };

    class EntryView : public virtual MapEntry<K, V> {
    private:
        Hashtable<K, V>* owner_;
        jxx::Ptr<K> key_;
    public:
        EntryView(Hashtable<K, V>* owner, jxx::Ptr<K> key)
            : owner_(owner), key_(key) {}
        virtual ~EntryView() = default;
        virtual jxx::Ptr<K> getKey() override { return key_; }
        virtual jxx::Ptr<V> getValue() override {
            return owner_->get(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(key_));
        }
        virtual jxx::Ptr<V> setValue(jxx::Ptr<V> value) override {
            return owner_->put(key_, value);
        }

        virtual jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> o) override {
            if (o == nullptr) return static_cast<jxx::lang::jbool>(false);
            auto e = jxx::CAST<MapEntry<K, V>, jxx::lang::Object>(o);
            if (e == nullptr) return static_cast<jxx::lang::jbool>(false);
            return static_cast<jxx::lang::jbool>(objEquals(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(key_), jxx::CAST<jxx::lang::Object, jxx::lang::Object>(e->getKey())) &&
                objEquals(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(getValue()), jxx::CAST<jxx::lang::Object, jxx::lang::Object>(e->getValue())));
		}

        virtual jxx::lang::jint hashCode() override {
            return objectHash(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(key_)) ^
                objectHash(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(getValue()));
		}
    };

    class KeySetView : public virtual Set<K> {
    private:
        Hashtable<K, V>* owner_;
    public:
        explicit KeySetView(Hashtable<K, V>* owner) : owner_(owner) {}
        virtual ~KeySetView() = default;
        virtual jxx::lang::jint size() override { return owner_->size(); }
        virtual jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> o) override { return owner_->containsKey(o); }
        virtual jxx::Ptr<Iterator<K>> iterator() override { return owner_->snapshotKeyIterator(); }
        virtual jxx::lang::jbool add(jxx::Ptr<K> /*e*/) override { throw jxx::lang::UnsupportedOperationException(); }
        virtual jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> o) override {
            if (!owner_->containsKey(o)) return static_cast<jxx::lang::jbool>(false);
            owner_->remove(o);
            return static_cast<jxx::lang::jbool>(true);
        }
        virtual void clear() override { owner_->clear(); }

        virtual jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>> toArray() override {
            return nullptr;
        }

        virtual jxx::lang::jbool containsAll(jxx::Ptr<wildcard::CollectionAny> c) override {
            if (c == nullptr) throw jxx::lang::NullPointerException();
            auto it = c->iteratorObject();
            while (it->hasNext()) {
                auto e = jxx::CAST<MapEntry<K, V>, jxx::lang::Object>(it->next());
                if (!contains(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(e))) return static_cast<jxx::lang::jbool>(false);
            }
            return static_cast<jxx::lang::jbool>(true);
        }
        virtual jxx::lang::jbool addAll(jxx::Ptr<wildcard::CollectionExtends<K>> c) override {
            throw jxx::lang::UnsupportedOperationException();
        }
        virtual jxx::lang::jbool retainAll(jxx::Ptr<wildcard::CollectionAny> c) override {
            throw jxx::lang::UnsupportedOperationException();
        }
        virtual jxx::lang::jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) override {
            throw jxx::lang::UnsupportedOperationException();
        }
        virtual jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> o) override {
            throw jxx::lang::UnsupportedOperationException();
        }
        virtual jxx::lang::jint hashCode() override {
            throw jxx::lang::UnsupportedOperationException();
        }
        virtual jxx::Ptr<Spliterator<K>> spliterator() override {
            throw jxx::lang::UnsupportedOperationException();
        }
    };

    class ValuesView : public virtual Collection<V> {
    private:
        Hashtable<K, V>* owner_;
    public:
        explicit ValuesView(Hashtable<K, V>* owner) : owner_(owner) {}
        virtual ~ValuesView() = default;
        virtual jxx::lang::jint size() override { return owner_->size(); }
        virtual jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> o) override { return owner_->containsValue(o); }
        virtual jxx::Ptr<Iterator<V>> iterator() override { return owner_->snapshotValueIterator(); }
        virtual jxx::lang::jbool add(jxx::Ptr<V> /*e*/) override { throw jxx::lang::UnsupportedOperationException(); }
        virtual jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> o) override {
            return owner_->removeValueFirst(o);
        }
        virtual void clear() override { owner_->clear(); }
        virtual jxx::Ptr<JxxArray<jxx::Ptr<V>, 1U>> toArray() override {
            const jxx::lang::jint sz = this->size();
            auto result = jxx::Ptr<JxxArray<jxx::Ptr<V>, 1U>>(jxx::NEW<JxxArray<jxx::Ptr<V>, 1U>>(sz));
            auto it = this->iterator();
            jxx::lang::jint i = 0;
            while (it->hasNext()) {
                (*result)(i++) = it->next();
            }
            return result;
        }
               
        virtual jxx::lang::jbool containsAll(jxx::Ptr<wildcard::CollectionAny> c) override {
            if (c == nullptr) throw jxx::lang::NullPointerException();
            auto it = c->iteratorObject();
            while (it->hasNext()) {
                auto e = jxx::CAST<jxx::lang::Object, jxx::lang::Object>(it->next());
                if (!contains(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(e))) return
                    static_cast<jxx::lang::jbool>(false);
            }
            return static_cast<jxx::lang::jbool>(true);
		}
        virtual jxx::lang::jbool addAll(jxx::Ptr<wildcard::CollectionExtends<V>> c) override {
			throw jxx::lang::UnsupportedOperationException();
        }
        virtual jxx::lang::jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) override {
            throw jxx::lang::UnsupportedOperationException();
        }
        virtual jxx::lang::jbool retainAll(jxx::Ptr<wildcard::CollectionAny> c) override {
            throw jxx::lang::UnsupportedOperationException(); }
       
    };

    class EntrySetView : public virtual Set<MapEntry<K, V>> {
    private:
        Hashtable<K, V>* owner_;
    public:
        explicit EntrySetView(Hashtable<K, V>* owner) : owner_(owner) {}
        virtual ~EntrySetView() = default;
        virtual jxx::lang::jint size() override { return owner_->size(); }
        virtual jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> o) override {
            auto e = jxx::CAST<MapEntry<K, V>, jxx::lang::Object>(o);
            if (e == nullptr) return static_cast<jxx::lang::jbool>(false);
            auto v = owner_->get(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(e->getKey()));
            if (v == nullptr && !owner_->containsKey(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(e->getKey()))) {
                return static_cast<jxx::lang::jbool>(false);
            }
            return objEquals(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(v),
                jxx::CAST<jxx::lang::Object, jxx::lang::Object>(e->getValue()));
        }
        virtual jxx::Ptr<Iterator<MapEntry<K, V>>> iterator() override { return owner_->snapshotEntryIterator(); }
        virtual jxx::lang::jbool add(jxx::Ptr<MapEntry<K, V>> /*e*/) override { throw jxx::lang::UnsupportedOperationException(); }
        virtual jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> o) override {
            auto e = jxx::CAST<MapEntry<K, V>, jxx::lang::Object>(o);
            if (e == nullptr) return static_cast<jxx::lang::jbool>(false);
            return owner_->removeEq(e->getKey(), e->getValue());
        }
        virtual void clear() override { owner_->clear(); }
         
        virtual jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>> toArray() override {
            return nullptr;
		}        
       
        virtual jxx::lang::jbool containsAll(jxx::Ptr<wildcard::CollectionAny> c) override {
            if (c == nullptr) throw jxx::lang::NullPointerException();
            auto it = c->iteratorObject();
            while (it->hasNext()) {
                auto e = jxx::CAST<MapEntry<K, V>, jxx::lang::Object>(it->next());
                if (!contains(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(e))) return static_cast<jxx::lang::jbool>(false);
            }
            return static_cast<jxx::lang::jbool>(true);
		}
        virtual jxx::lang::jbool addAll(jxx::Ptr<wildcard::CollectionExtends<MapEntry<K, V>>> c) override {
            throw jxx::lang::UnsupportedOperationException();
		}
        virtual jxx::lang::jbool retainAll(jxx::Ptr<wildcard::CollectionAny> c) override {
            throw jxx::lang::UnsupportedOperationException();
        }
        virtual jxx::lang::jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) override {
            throw jxx::lang::UnsupportedOperationException();
        }
        virtual jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> o) override {
            throw jxx::lang::UnsupportedOperationException();
        }
        virtual jxx::lang::jint hashCode() override {
            throw jxx::lang::UnsupportedOperationException();
        }
        virtual jxx::Ptr<Spliterator<MapEntry<K, V>>> spliterator() override {
            throw jxx::lang::UnsupportedOperationException();
        }
    };

public:
    Hashtable()
        : buckets_(static_cast<std::size_t>(DEFAULT_INITIAL_CAPACITY))
        , count_(0)
        , threshold_(0)
        , loadFactor_(DEFAULT_LOAD_FACTOR) {
        recomputeThreshold();
    }

    explicit Hashtable(jxx::lang::jint initialCapacity)
        : buckets_(static_cast<std::size_t>(initialCapacity > 0 ? initialCapacity : 1))
        , count_(0)
        , threshold_(0)
        , loadFactor_(DEFAULT_LOAD_FACTOR) {
        if (initialCapacity < 0) throw jxx::lang::IllegalArgumentException();
        recomputeThreshold();
    }

    Hashtable(jxx::lang::jint initialCapacity, float loadFactor)
        : buckets_(static_cast<std::size_t>(initialCapacity > 0 ? initialCapacity : 1))
        , count_(0)
        , threshold_(0)
        , loadFactor_(loadFactor) {
        if (initialCapacity < 0 || !(loadFactor > 0.0f)) throw jxx::lang::IllegalArgumentException();
        recomputeThreshold();
    }

    explicit Hashtable(jxx::Ptr<Map<K, V>> t)
        : buckets_(static_cast<std::size_t>(DEFAULT_INITIAL_CAPACITY))
        , count_(0)
        , threshold_(0)
        , loadFactor_(DEFAULT_LOAD_FACTOR) {
        if (t == nullptr) throw jxx::lang::NullPointerException();
        recomputeThreshold();
        putAll(t);
    }

    virtual ~Hashtable() = default;
    
    virtual jxx::lang::jint size() override {
        return this->synchronized([&]() -> jxx::lang::jint { return count_; });
    }

    virtual jxx::lang::jint hashCode() override {
        return (jxx::lang::jint)this;
	}

    virtual void writeObject(jxx::Ptr<jxx::io::ObjectOutputStream> out)override {
        if (out == nullptr) throw jxx::lang::NullPointerException();
        this->synchronized([&]() {
            out->writeInt(count_);
            for (const auto& bucket : buckets_) {
                for (const auto& entry : bucket) {
                    out->writeObject(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(entry.key));
                    out->writeObject(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(entry.value));
                }
            }
        });
	}
    virtual void readObject(jxx::Ptr<jxx::io::ObjectInputStream> in) override {
        if (in == nullptr) throw jxx::lang::NullPointerException();
        this->synchronized([&]() {
            count_ = in->readInt();
            for (jxx::lang::jint i = 0; i < count_; ++i) {
                auto key = jxx::CAST<K, jxx::lang::Object>(in->readObject());
                auto value = jxx::CAST<V, jxx::lang::Object>(in->readObject());
                put(key, value);
            }
			});

    }

    virtual void readObjectNoData() override {
        this->synchronized([&]() {
            buckets_.clear();
            buckets_.resize(static_cast<std::size_t>(DEFAULT_INITIAL_CAPACITY));
            count_ = 0;
            loadFactor_ = DEFAULT_LOAD_FACTOR;
            recomputeThreshold();
        });
	}

    virtual jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> o) override {
        if (o == nullptr) return static_cast<jxx::lang::jbool>(false);
        if (this == o.get()) return static_cast<jxx::lang::jbool>(true);
        auto m = jxx::CAST<Map<K, V>, jxx::lang::Object>(o);
        if (m == nullptr) return static_cast<jxx::lang::jbool>(false);
        return this->synchronized([&]() -> jxx::lang::jbool {
            if (count_ != m->size()) return static_cast<jxx::lang::jbool>(false);
            auto it = m->entrySet()->iterator();
            while (it->hasNext()) {
                auto e = it->next();
                auto key = e->getKey();
                auto value = e->getValue();
                auto v = get(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(key));
                if (!objEquals(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(v),
                    jxx::CAST<jxx::lang::Object, jxx::lang::Object>(value))) {
                    return static_cast<jxx::lang::jbool>(false);
                }
            }
            return static_cast<jxx::lang::jbool>(true);
        });
	}

    virtual jxx::lang::jbool isEmpty() override {
        return this->synchronized([&]() -> jxx::lang::jbool { return static_cast<jxx::lang::jbool>(count_ == 0); });
    }

    virtual jxx::Ptr<Enumeration<K>> keys() override {
        return snapshotKeyEnumeration();
    }

    virtual jxx::Ptr<Enumeration<V>> elements() override {
        return snapshotValueEnumeration();
    }

    virtual jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> value) {
        return containsValue(value);
    }

    virtual jxx::lang::jbool containsValue(jxx::Ptr<jxx::lang::Object> value) override {
        if (value == nullptr) throw jxx::lang::NullPointerException();
        return this->synchronized([&]() -> jxx::lang::jbool {
            for (const auto& bucket : buckets_) {
                for (const auto& entry : bucket) {
                    if (objEquals(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(entry.value), value)) {
                        return static_cast<jxx::lang::jbool>(true);
                    }
                }
            }
            return static_cast<jxx::lang::jbool>(false);
        });
    }

    virtual jxx::lang::jbool containsKey(jxx::Ptr<jxx::lang::Object> key) override {
        return this->synchronized([&]() -> jxx::lang::jbool {
            return static_cast<jxx::lang::jbool>(findEntryUnlocked(key) != nullptr);
        });
    }

    virtual jxx::Ptr<V> get(jxx::Ptr<jxx::lang::Object> key) override {
        return this->synchronized([&]() -> jxx::Ptr<V> {
            auto* entry = findEntryUnlocked(key);
            return entry == nullptr ? nullptr : entry->value;
        });
    }

    virtual jxx::Ptr<V> put(jxx::Ptr<K> key, jxx::Ptr<V> value) override {
        if (key == nullptr || value == nullptr) throw jxx::lang::NullPointerException();
        return this->synchronized([&]() -> jxx::Ptr<V> {
            auto* entry = findEntryUnlocked(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(key));
            if (entry != nullptr) {
                auto old = entry->value;
                entry->value = value;
                return old;
            }
            ensureCapacityForInsertUnlocked();
            insertNewUnlocked(key, value);
            return nullptr;
        });
    }

    virtual jxx::Ptr<V> remove(jxx::Ptr<jxx::lang::Object> key) override {
        return this->synchronized([&]() -> jxx::Ptr<V> {
            if (key == nullptr) return nullptr;
            const auto h = objectHash(key);
            auto& bucket = buckets_[bucketIndexFor(h, buckets_.size())];
            for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                if (it->hash == h && objEquals(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(it->key), key)) {
                    auto old = it->value;
                    bucket.erase(it);
                    --count_;
                    return old;
                }
            }
            return nullptr;
        });
    }

    virtual void putAll(jxx::Ptr<Map<K, V>> t) override {
        if (t == nullptr) throw jxx::lang::NullPointerException();
        auto it = t->entrySet()->iterator();
        while (it->hasNext()) {
            auto e = it->next();
            put(e->getKey(), e->getValue());
        }
    }

    virtual void clear() override {
        this->synchronized([&]() -> void {
            for (auto& bucket : buckets_) bucket.clear();
            count_ = 0;
        });
    }

    virtual jxx::Ptr<jxx::lang::Object> clone() {
        auto copy = jxx::Ptr<Hashtable<K, V>>(jxx::NEW<Hashtable<K, V>>(static_cast<jxx::lang::jint>(buckets_.size()), 
            loadFactor_));
        this->synchronized([&]() -> void {
            for (const auto& bucket : buckets_) {
                for (const auto& entry : bucket) {
                    copy->put(entry.key, entry.value);
                }
            }
        });
        return jxx::Ptr<jxx::lang::Object>(copy);
    }

    virtual jxx::Ptr<Set<K>> keySet() override {
        return jxx::Ptr<Set<K>>(jxx::NEW<KeySetView>(this));
    }

    virtual jxx::Ptr<Collection<V>> values() override {
        return jxx::Ptr<Collection<V>>(jxx::NEW<ValuesView>(this));
    }

    virtual jxx::Ptr<Set<MapEntry<K, V>>> entrySet() override {
        return jxx::Ptr<Set<MapEntry<K, V>>>(jxx::NEW<EntrySetView>(this));
    }

    virtual jxx::Ptr<V> getOrDefault(jxx::Ptr<jxx::lang::Object> key, jxx::Ptr<V> defaultValue) {
        auto v = get(key);
        return v == nullptr ? defaultValue : v;
    }

    virtual jxx::Ptr<V> putIfAbsent(jxx::Ptr<K> key, jxx::Ptr<V> value) {
        if (key == nullptr || value == nullptr) throw jxx::lang::NullPointerException();
        return this->synchronized([&]() -> jxx::Ptr<V> {
            auto* entry = findEntryUnlocked(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(key));
            if (entry != nullptr) return entry->value;
            ensureCapacityForInsertUnlocked();
            insertNewUnlocked(key, value);
            return nullptr;
        });
    }

    virtual jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> key, jxx::Ptr<jxx::lang::Object> value) {
        if (key == nullptr || value == nullptr) return static_cast<jxx::lang::jbool>(false);
        return this->synchronized([&]() -> jxx::lang::jbool {
            const auto h = objectHash(key);
            auto& bucket = buckets_[bucketIndexFor(h, buckets_.size())];
            for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                if (it->hash == h && objEquals(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(it->key), key)
                    && objEquals(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(it->value), value)) {
                    bucket.erase(it);
                    --count_;
                    return static_cast<jxx::lang::jbool>(true);
                }
            }
            return static_cast<jxx::lang::jbool>(false);
        });
    }

    virtual jxx::lang::jbool replace(jxx::Ptr<K> key, jxx::Ptr<V> oldValue, jxx::Ptr<V> newValue) {
        if (key == nullptr || oldValue == nullptr || newValue == nullptr) throw jxx::lang::NullPointerException();
        return this->synchronized([&]() -> jxx::lang::jbool {
            auto* entry = findEntryUnlocked(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(key));
            if (entry == nullptr) return static_cast<jxx::lang::jbool>(false);
            if (!objEquals(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(entry->value), 
                jxx::CAST<jxx::lang::Object, jxx::lang::Object>(oldValue))) {
                return static_cast<jxx::lang::jbool>(false);
            }
            entry->value = newValue;
            return static_cast<jxx::lang::jbool>(true);
        });
    }

    virtual jxx::Ptr<V> replace(jxx::Ptr<K> key, jxx::Ptr<V> value) {
        if (key == nullptr || value == nullptr) throw jxx::lang::NullPointerException();
        return this->synchronized([&]() -> jxx::Ptr<V> {
            auto* entry = findEntryUnlocked(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(key));
            if (entry == nullptr) return nullptr;
            auto old = entry->value;
            entry->value = value;
            return old;
        });
    }

protected:
    virtual jxx::Ptr<jxx::lang::Object> cloneImpl() const override { throw jxx::lang::UnsupportedOperationException(); }

    virtual void rehash() {
        const std::size_t newCap = buckets_.size() * 2U + 1U;
        std::vector<std::vector<EntryRecord>> newBuckets(newCap);
        for (const auto& bucket : buckets_) {
            for (const auto& entry : bucket) {
                newBuckets[bucketIndexFor(entry.hash, newCap)].push_back(entry);
            }
        }
        buckets_.swap(newBuckets);
        recomputeThreshold();
    }

private:
    jxx::Ptr<Iterator<K>> snapshotKeyIterator() {
        auto snapshot = this->synchronized([&]() -> std::vector<jxx::Ptr<K>> {
            std::vector<jxx::Ptr<K>> out;
            out.reserve(static_cast<std::size_t>(count_));
            for (const auto& bucket : buckets_) for (const auto& entry : bucket) out.push_back(entry.key);
            return out;
        });
        return jxx::Ptr<Iterator<K>>(new SnapshotIterator<K>(std::move(snapshot)));
    }

    jxx::Ptr<Iterator<V>> snapshotValueIterator() {
        auto snapshot = this->synchronized([&]() -> std::vector<jxx::Ptr<V>> {
            std::vector<jxx::Ptr<V>> out;
            out.reserve(static_cast<std::size_t>(count_));
            for (const auto& bucket : buckets_) for (const auto& entry : bucket) out.push_back(entry.value);
            return out;
        });
        return jxx::Ptr<Iterator<V>>(new SnapshotIterator<V>(std::move(snapshot)));
    }

    jxx::Ptr<Iterator<MapEntry<K, V>>> snapshotEntryIterator() {
        auto snapshot = this->synchronized([&]() -> std::vector<jxx::Ptr<MapEntry<K, V>>> {
            std::vector<jxx::Ptr<MapEntry<K, V>>> out;
            out.reserve(static_cast<std::size_t>(count_));
            for (const auto& bucket : buckets_) {
                for (const auto& entry : bucket) {
                    out.push_back(jxx::Ptr<MapEntry<K, V>>(jxx::NEW<EntryView>(this, entry.key)));
                }
            }
            return out;
        });
        return jxx::Ptr<Iterator<MapEntry<K, V>>>(new SnapshotIterator<MapEntry<K, V>>(std::move(snapshot)));
    }

    jxx::Ptr<Enumeration<K>> snapshotKeyEnumeration() {
        auto snapshot = this->synchronized([&]() -> std::vector<jxx::Ptr<K>> {
            std::vector<jxx::Ptr<K>> out;
            out.reserve(static_cast<std::size_t>(count_));
            for (const auto& bucket : buckets_) for (const auto& entry : bucket) out.push_back(entry.key);
            return out;
        });
        return jxx::Ptr<Enumeration<K>>(new SnapshotEnumeration<K>(std::move(snapshot)));
    }

    jxx::Ptr<Enumeration<V>> snapshotValueEnumeration() {
        auto snapshot = this->synchronized([&]() -> std::vector<jxx::Ptr<V>> {
            std::vector<jxx::Ptr<V>> out;
            out.reserve(static_cast<std::size_t>(count_));
            for (const auto& bucket : buckets_) for (const auto& entry : bucket) out.push_back(entry.value);
            return out;
        });
        return jxx::Ptr<Enumeration<V>>(new SnapshotEnumeration<V>(std::move(snapshot)));
    }

    jxx::lang::jbool removeValueFirst(jxx::Ptr<jxx::lang::Object> value) {
        if (value == nullptr) return static_cast<jxx::lang::jbool>(false);
        return this->synchronized([&]() -> jxx::lang::jbool {
            for (auto& bucket : buckets_) {
                for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                    if (objEquals(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(it->value), value)) {
                        bucket.erase(it);
                        --count_;
                        return static_cast<jxx::lang::jbool>(true);
                    }
                }
            }
            return static_cast<jxx::lang::jbool>(false);
        });
    }

    jxx::lang::jbool removeEq(jxx::Ptr<K> key, jxx::Ptr<V> value) {
        if (key == nullptr || value == nullptr) return static_cast<jxx::lang::jbool>(false);
        return remove(jxx::CAST<jxx::lang::Object, jxx::lang::Object>(key), 
            jxx::CAST<jxx::lang::Object, jxx::lang::Object>(value));
    }
};

} // namespace util
} // namespace jxx
