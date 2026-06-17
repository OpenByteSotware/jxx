#pragma once

#include <map>
#include <memory>
#include <shared_mutex>
#include <stdexcept>
#include <utility>
#include <vector>

#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Comparable.h"
#include "io/jxx.io.Serializable.h"
#include "util/jxx.util.AbstractMap.h"
#include "util/jxx.util.Comparator.h"
#include "util/jxx.util.NavigableMap.h"
#include "util/jxx.util.NavigableSet.h"
#include "util/jxx.util.ArrayList.h"
#include "lang/jxx.lang.String.h"

namespace jxx::util
{
    template<typename K, typename V>
    class TreeMap final
        : public AbstractMap<K, V>
        , public NavigableMap<K, V>
        , public jxx::lang::Cloneable
        , public jxx::io::Serializable
    {
    private:
        struct KeyOrder
        {
            explicit KeyOrder(const TreeMap<K, V>* owner = nullptr)
                : owner_(owner)
            {
            }

            bool operator()(const jxx::Ptr<K>& a,
                            const jxx::Ptr<K>& b) const
            {
                return owner_->compareKeys_(a, b) < 0;
            }

        private:
            const TreeMap<K, V>* owner_ = nullptr;
        };

        using NativeMap = std::map<jxx::Ptr<K>, jxx::Ptr<V>, KeyOrder>;

        class SnapshotKeySet final : public jxx::lang::Object, public NavigableSet<K>
        {
        public:
            SnapshotKeySet(std::vector<jxx::Ptr<K>> items,
                           jxx::Ptr<Comparator<K>> c,
                           const TreeMap<K, V>* owner)
                : items_(std::move(items)),
                  comparator_(std::move(c)),
                  owner_(owner)
            {
            }

            ~SnapshotKeySet() override = default;

        public:
            jxx::lang::jint size() const override
            {
                return static_cast<jxx::lang::jint>(items_.size());
            }

            jxx::lang::jbool isEmpty() const override
            {
                return items_.empty();
            }

            jxx::Ptr<K> get(jxx::lang::jint index) const override
            {
                if (index < 0 || static_cast<std::size_t>(index) >= items_.size())
                    return nullptr;
                return items_[static_cast<std::size_t>(index)];
            }

            void add(const jxx::Ptr<K>&) override { throw std::runtime_error("read-only snapshot set"); }
            void clear() override { throw std::runtime_error("read-only snapshot set"); }
            jxx::lang::jbool contains(jxx::Ptr<K> e) const override
            {
                for (const auto& x : items_) if (internal::ptrEquals(x, e)) return true;
                return false;
            }
            jxx::lang::jbool addElement(jxx::Ptr<K>) override { throw std::runtime_error("read-only snapshot set"); }
            jxx::lang::jbool removeElement(jxx::Ptr<K>) override { throw std::runtime_error("read-only snapshot set"); }

            jxx::Ptr<Comparator<K>> comparator() const override { return comparator_; }
            jxx::Ptr<K> first() const override { return items_.empty() ? nullptr : items_.front(); }
            jxx::Ptr<K> last() const override { return items_.empty() ? nullptr : items_.back(); }

            jxx::Ptr<K> lower(jxx::Ptr<K> e) const override
            {
                jxx::Ptr<K> best = nullptr;
                for (const auto& x : items_)
                {
                    if (owner_->compareKeys_(x, e) < 0) best = x;
                    else break;
                }
                return best;
            }

            jxx::Ptr<K> floor(jxx::Ptr<K> e) const override
            {
                jxx::Ptr<K> best = nullptr;
                for (const auto& x : items_)
                {
                    if (owner_->compareKeys_(x, e) <= 0) best = x;
                    else break;
                }
                return best;
            }

            jxx::Ptr<K> ceiling(jxx::Ptr<K> e) const override
            {
                for (const auto& x : items_) if (owner_->compareKeys_(x, e) >= 0) return x;
                return nullptr;
            }

            jxx::Ptr<K> higher(jxx::Ptr<K> e) const override
            {
                for (const auto& x : items_) if (owner_->compareKeys_(x, e) > 0) return x;
                return nullptr;
            }

            jxx::Ptr<K> pollFirst() override { throw std::runtime_error("read-only snapshot set"); }
            jxx::Ptr<K> pollLast() override { throw std::runtime_error("read-only snapshot set"); }

        private:
            std::vector<jxx::Ptr<K>> items_;
            jxx::Ptr<Comparator<K>> comparator_;
            const TreeMap<K, V>* owner_ = nullptr;
        };

        class SnapshotEntrySet final : public jxx::lang::Object, public Set<MapEntry<K, V>>
        {
        public:
            explicit SnapshotEntrySet(std::vector<jxx::Ptr<MapEntry<K, V>>> entries)
                : entries_(std::move(entries))
            {
            }
            ~SnapshotEntrySet() override = default;

        public:
            jxx::lang::jint size() const override { return static_cast<jxx::lang::jint>(entries_.size()); }
            jxx::lang::jbool isEmpty() const override { return entries_.empty(); }
            jxx::Ptr<MapEntry<K, V>> get(jxx::lang::jint index) const override
            {
                if (index < 0 || static_cast<std::size_t>(index) >= entries_.size()) return nullptr;
                return entries_[static_cast<std::size_t>(index)];
            }
            void add(const jxx::Ptr<MapEntry<K, V>>&) override { throw std::runtime_error("read-only snapshot set"); }
            void clear() override { throw std::runtime_error("read-only snapshot set"); }
            jxx::lang::jbool contains(jxx::Ptr<MapEntry<K, V>> e) const override
            {
                if (!e) return false;
                for (const auto& x : entries_)
                    if (x && internal::ptrEquals(x->getKey(), e->getKey()) && internal::ptrEquals(x->getValue(), e->getValue()))
                        return true;
                return false;
            }
            jxx::lang::jbool addElement(jxx::Ptr<MapEntry<K, V>>) override { throw std::runtime_error("read-only snapshot set"); }
            jxx::lang::jbool removeElement(jxx::Ptr<MapEntry<K, V>>) override { throw std::runtime_error("read-only snapshot set"); }

        private:
            std::vector<jxx::Ptr<MapEntry<K, V>>> entries_;
        };

    public:
        TreeMap()
            : comparator_(nullptr),
              map_(KeyOrder(this))
        {
        }

        explicit TreeMap(jxx::Ptr<Comparator<K>> comparator)
            : comparator_(std::move(comparator)),
              map_(KeyOrder(this))
        {
        }

        explicit TreeMap(jxx::Ptr<Map<K, V>> m)
            : TreeMap()
        {
            if (m) this->putAll(m);
        }

        explicit TreeMap(jxx::Ptr<SortedMap<K, V>> m)
            : TreeMap(m ? m->comparator() : nullptr)
        {
            if (m) this->putAll(std::static_pointer_cast<Map<K, V>>(m));
        }

        ~TreeMap() override = default;

    public:
        jxx::lang::jint size() const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            return static_cast<jxx::lang::jint>(map_.size());
        }

        jxx::lang::jbool isEmpty() const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            return map_.empty();
        }

        jxx::lang::jbool containsKey(jxx::Ptr<K> key) const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            return map_.find(key) != map_.end();
        }

        jxx::lang::jbool containsValue(jxx::Ptr<V> value) const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            for (const auto& kv : map_) if (internal::ptrEquals(kv.second, value)) return true;
            return false;
        }

        jxx::Ptr<V> get(jxx::Ptr<K> key) const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            auto it = map_.find(key);
            return it == map_.end() ? nullptr : it->second;
        }

        jxx::Ptr<V> put(jxx::Ptr<K> key, jxx::Ptr<V> value) override
        {
            std::unique_lock<std::shared_mutex> lock(m_);
            auto it = map_.find(key);
            if (it == map_.end())
            {
                map_.emplace(std::move(key), std::move(value));
                return nullptr;
            }
            auto old = it->second;
            it->second = std::move(value);
            return old;
        }

        void putAll(jxx::Ptr<Map<K, V>> m) override
        {
            AbstractMap<K, V>::putAll(std::move(m));
        }

        jxx::Ptr<V> remove(jxx::Ptr<K> key) override
        {
            std::unique_lock<std::shared_mutex> lock(m_);
            auto it = map_.find(key);
            if (it == map_.end()) return nullptr;
            auto old = it->second;
            map_.erase(it);
            return old;
        }

        void clear() override
        {
            std::unique_lock<std::shared_mutex> lock(m_);
            map_.clear();
        }

        jxx::Ptr<Comparator<K>> comparator() const override { return comparator_; }
        jxx::Ptr<K> firstKey() const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            return map_.empty() ? nullptr : map_.begin()->first;
        }
        jxx::Ptr<K> lastKey() const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            return map_.empty() ? nullptr : map_.rbegin()->first;
        }

        jxx::Ptr<MapEntry<K, V>> firstEntry() const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            return map_.empty() ? nullptr : std::make_shared<BasicMapEntry<K, V>>(map_.begin()->first, map_.begin()->second);
        }

        jxx::Ptr<MapEntry<K, V>> lastEntry() const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            return map_.empty() ? nullptr : std::make_shared<BasicMapEntry<K, V>>(map_.rbegin()->first, map_.rbegin()->second);
        }

        jxx::Ptr<MapEntry<K, V>> lowerEntry(jxx::Ptr<K> key) const override
        {
            auto k = lowerKey(key);
            return k ? std::make_shared<BasicMapEntry<K, V>>(k, get(k)) : nullptr;
        }

        jxx::Ptr<K> lowerKey(jxx::Ptr<K> key) const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            auto it = map_.lower_bound(key);
            if (it == map_.begin()) return nullptr;
            --it;
            return it->first;
        }

        jxx::Ptr<MapEntry<K, V>> floorEntry(jxx::Ptr<K> key) const override
        {
            auto k = floorKey(key);
            return k ? std::make_shared<BasicMapEntry<K, V>>(k, get(k)) : nullptr;
        }

        jxx::Ptr<K> floorKey(jxx::Ptr<K> key) const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            auto it = map_.upper_bound(key);
            if (it == map_.begin()) return nullptr;
            --it;
            return it->first;
        }

        jxx::Ptr<MapEntry<K, V>> ceilingEntry(jxx::Ptr<K> key) const override
        {
            auto k = ceilingKey(key);
            return k ? std::make_shared<BasicMapEntry<K, V>>(k, get(k)) : nullptr;
        }

        jxx::Ptr<K> ceilingKey(jxx::Ptr<K> key) const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            auto it = map_.lower_bound(key);
            return it == map_.end() ? nullptr : it->first;
        }

        jxx::Ptr<MapEntry<K, V>> higherEntry(jxx::Ptr<K> key) const override
        {
            auto k = higherKey(key);
            return k ? std::make_shared<BasicMapEntry<K, V>>(k, get(k)) : nullptr;
        }

        jxx::Ptr<K> higherKey(jxx::Ptr<K> key) const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            auto it = map_.upper_bound(key);
            return it == map_.end() ? nullptr : it->first;
        }

        jxx::Ptr<MapEntry<K, V>> pollFirstEntry() override
        {
            std::unique_lock<std::shared_mutex> lock(m_);
            if (map_.empty()) return nullptr;
            auto it = map_.begin();
            auto out = std::make_shared<BasicMapEntry<K, V>>(it->first, it->second);
            map_.erase(it);
            return out;
        }

        jxx::Ptr<MapEntry<K, V>> pollLastEntry() override
        {
            std::unique_lock<std::shared_mutex> lock(m_);
            if (map_.empty()) return nullptr;
            auto it = std::prev(map_.end());
            auto out = std::make_shared<BasicMapEntry<K, V>>(it->first, it->second);
            map_.erase(it);
            return out;
        }

        jxx::Ptr<NavigableMap<K, V>> descendingMap() const override
        {
            auto out = std::make_shared<TreeMap<K, V>>(comparator_);
            std::shared_lock<std::shared_mutex> lock(m_);
            for (auto it = map_.rbegin(); it != map_.rend(); ++it)
                out->put(it->first, it->second);
            return out;
        }

        jxx::Ptr<NavigableSet<K>> navigableKeySet() const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            std::vector<jxx::Ptr<K>> keys;
            keys.reserve(map_.size());
            for (const auto& kv : map_) keys.push_back(kv.first);
            return std::make_shared<SnapshotKeySet>(std::move(keys), comparator_, this);
        }

        jxx::Ptr<NavigableSet<K>> descendingKeySet() const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            std::vector<jxx::Ptr<K>> keys;
            keys.reserve(map_.size());
            for (auto it = map_.rbegin(); it != map_.rend(); ++it) keys.push_back(it->first);
            return std::make_shared<SnapshotKeySet>(std::move(keys), comparator_, this);
        }

        jxx::Ptr<SortedMap<K, V>> headMap(jxx::Ptr<K> toKey) const override
        {
            return std::static_pointer_cast<SortedMap<K, V>>(headMap(toKey, false));
        }

        jxx::Ptr<SortedMap<K, V>> subMap(jxx::Ptr<K> fromKey, jxx::Ptr<K> toKey) const override
        {
            return std::static_pointer_cast<SortedMap<K, V>>(subMap(fromKey, true, toKey, false));
        }

        jxx::Ptr<SortedMap<K, V>> tailMap(jxx::Ptr<K> fromKey) const override
        {
            return std::static_pointer_cast<SortedMap<K, V>>(tailMap(fromKey, true));
        }

        jxx::Ptr<NavigableMap<K, V>> headMap(jxx::Ptr<K> toKey, jxx::lang::jbool inclusive) const override
        {
            auto out = std::make_shared<TreeMap<K, V>>(comparator_);
            std::shared_lock<std::shared_mutex> lock(m_);
            for (const auto& kv : map_)
            {
                const auto cmp = compareKeys_(kv.first, toKey);
                if (cmp < 0 || (inclusive && cmp == 0)) out->put(kv.first, kv.second);
                else break;
            }
            return out;
        }

        jxx::Ptr<NavigableMap<K, V>> tailMap(jxx::Ptr<K> fromKey, jxx::lang::jbool inclusive) const override
        {
            auto out = std::make_shared<TreeMap<K, V>>(comparator_);
            std::shared_lock<std::shared_mutex> lock(m_);
            for (const auto& kv : map_)
            {
                const auto cmp = compareKeys_(kv.first, fromKey);
                if (cmp > 0 || (inclusive && cmp == 0)) out->put(kv.first, kv.second);
            }
            return out;
        }

        jxx::Ptr<NavigableMap<K, V>> subMap(jxx::Ptr<K> fromKey,
                                            jxx::lang::jbool fromInclusive,
                                            jxx::Ptr<K> toKey,
                                            jxx::lang::jbool toInclusive) const override
        {
            auto out = std::make_shared<TreeMap<K, V>>(comparator_);
            std::shared_lock<std::shared_mutex> lock(m_);
            for (const auto& kv : map_)
            {
                const auto c1 = compareKeys_(kv.first, fromKey);
                const auto c2 = compareKeys_(kv.first, toKey);
                const auto lowerOk = (c1 > 0) || (fromInclusive && c1 == 0);
                const auto upperOk = (c2 < 0) || (toInclusive && c2 == 0);
                if (lowerOk && upperOk) out->put(kv.first, kv.second);
            }
            return out;
        }

        jxx::Ptr<Set<K>> keySet() const override
        {
            return std::static_pointer_cast<Set<K>>(navigableKeySet());
        }

        jxx::Ptr<Collection<V>> values() const override
        {
            auto out = std::make_shared<ArrayList<V>>();
            std::shared_lock<std::shared_mutex> lock(m_);
            for (const auto& kv : map_) out->add(kv.second);
            return out;
        }

        jxx::Ptr<Set<MapEntry<K, V>>> entrySet() const override
        {
            std::shared_lock<std::shared_mutex> lock(m_);
            std::vector<jxx::Ptr<MapEntry<K, V>>> entries;
            entries.reserve(map_.size());
            for (const auto& kv : map_)
                entries.push_back(std::make_shared<BasicMapEntry<K, V>>(kv.first, kv.second));
            return std::make_shared<SnapshotEntrySet>(std::move(entries));
        }

        jxx::Ptr<jxx::lang::Object> clone() const
        {
            auto out = std::make_shared<TreeMap<K, V>>(comparator_);
            std::shared_lock<std::shared_mutex> lock(m_);
            for (const auto& kv : map_) out->put(kv.first, kv.second);
            return out;
        }

        jxx::Ptr<jxx::lang::String> toString() const override
        {
            return std::make_shared<jxx::lang::String>(
                std::string("TreeMap[size=") + std::to_string(size()) + "]");
        }

    private:
        jxx::lang::jint compareKeys_(jxx::Ptr<K> a, jxx::Ptr<K> b) const
        {
            if (!a && !b) return 0;
            if (!a) return -1;
            if (!b) return 1;
            if (comparator_) return comparator_->compare(a, b);
            auto ca = std::dynamic_pointer_cast<jxx::lang::Comparable<K>>(a);
            if (!ca)
                throw std::runtime_error("TreeMap key is not Comparable and no Comparator was supplied");
            return ca->compareTo(b);
        }

    private:
        mutable std::shared_mutex m_;
        jxx::Ptr<Comparator<K>> comparator_;
        NativeMap map_;
    };
}
