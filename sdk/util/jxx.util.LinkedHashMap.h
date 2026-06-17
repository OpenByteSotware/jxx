#pragma once

#include <list>
#include <unordered_map>

#include "util/jxx.util.HashMap.h"

namespace jxx { template <typename T> class Ptr; }

namespace jxx {
namespace util {

template <typename K, typename V>
class LinkedHashMap : public HashMap<K, V> {
private:
    using OrderList = std::list<jxx::Ptr<K>>;
    using OrderIndexMap = std::unordered_map<jxx::Ptr<K>, typename OrderList::iterator, typename HashMap<K, V>::KeyHash, typename HashMap<K, V>::KeyEq>;

    OrderList order_;
    OrderIndexMap orderIndex_;
    jbool accessOrder_;

public:
    LinkedHashMap()
        : HashMap<K, V>()
        , order_()
        , orderIndex_()
        , accessOrder_(false) {
    }

    explicit LinkedHashMap(jint initialCapacity)
        : HashMap<K, V>(initialCapacity)
        , order_()
        , orderIndex_()
        , accessOrder_(false) {
    }

    LinkedHashMap(jint initialCapacity, jfloat loadFactor)
        : HashMap<K, V>(initialCapacity, loadFactor)
        , order_()
        , orderIndex_()
        , accessOrder_(false) {
    }

    LinkedHashMap(jint initialCapacity, jfloat loadFactor, jbool accessOrder)
        : HashMap<K, V>(initialCapacity, loadFactor)
        , order_()
        , orderIndex_()
        , accessOrder_(accessOrder) {
    }

    explicit LinkedHashMap(jxx::Ptr<Map<K, V>> m)
        : LinkedHashMap() {
        if (m == nullptr) throw NullPointerException();
        this->putAll(m);
    }

    virtual ~LinkedHashMap() = default;

    // Java hook: protected boolean removeEldestEntry(Map.Entry<K,V> eldest)
    virtual jbool removeEldestEntry(jxx::Ptr<MapEntry<K, V>> /*eldest*/) {
        return false;
    }

    virtual jxx::Ptr<jxx::lang::Object> clone() {
        jxx::Ptr<LinkedHashMap<K, V>> cloned(new LinkedHashMap<K, V>(static_cast<jint>(this->map_.size()), this->loadFactor_, accessOrder_));
        for (const auto& key : order_) {
            cloned->put(key, this->map_.at(key));
        }
        return cloned;
    }

protected:
    virtual void afterNodeAccess(jxx::Ptr<K> key) override {
        if (!accessOrder_) return;
        auto idx = orderIndex_.find(key);
        if (idx == orderIndex_.end()) return;
        auto it = idx->second;
        auto backIt = order_.end();
        if (!order_.empty()) {
            --backIt;
            if (it == backIt) return;
        }
        order_.erase(it);
        order_.push_back(key);
        auto tailIt = order_.end();
        --tailIt;
        idx->second = tailIt;
        ++this->modCount_;
    }

    virtual void afterNodeInsertion(jxx::Ptr<K> key, jbool isNewKey) override {
        if (isNewKey) {
            order_.push_back(key);
            auto tailIt = order_.end();
            --tailIt;
            orderIndex_[key] = tailIt;

            if (!order_.empty()) {
                auto eldestKey = order_.front();
                auto eldest = this->makeEntryView(eldestKey);
                if (removeEldestEntry(eldest)) {
                    this->remove(eldestKey);
                }
            }
        }
    }

    virtual void afterNodeRemoval(jxx::Ptr<K> key) override {
        auto idx = orderIndex_.find(key);
        if (idx != orderIndex_.end()) {
            order_.erase(idx->second);
            orderIndex_.erase(idx);
        }
    }

    virtual void afterClear() override {
        order_.clear();
        orderIndex_.clear();
    }

    class LinkedEntryIterator : public virtual Iterator<MapEntry<K, V>> {
    private:
        LinkedHashMap<K, V>* map_;
        typename OrderList::iterator current_;
        typename OrderList::iterator end_;
        jxx::Ptr<K> lastReturnedKey_;
        jbool canRemove_;
        jint expectedModCount_;
    public:
        explicit LinkedEntryIterator(LinkedHashMap<K, V>* map)
            : map_(map)
            , current_(map->order_.begin())
            , end_(map->order_.end())
            , lastReturnedKey_(nullptr)
            , canRemove_(false)
            , expectedModCount_(map->modCount_) {
        }

        virtual ~LinkedEntryIterator() = default;

        virtual jbool hasNext() override {
            return current_ != end_;
        }

        virtual jxx::Ptr<MapEntry<K, V>> next() override {
            checkForComodification();
            if (current_ == end_) throw NoSuchElementException();
            lastReturnedKey_ = *current_;
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

    class LinkedEntrySet : public HashMap<K, V>::EntrySet {
    private:
        LinkedHashMap<K, V>* linkedMap_;
    public:
        explicit LinkedEntrySet(LinkedHashMap<K, V>* map)
            : HashMap<K, V>::EntrySet(map)
            , linkedMap_(map) {
        }

        virtual ~LinkedEntrySet() = default;

        virtual jxx::Ptr<Iterator<MapEntry<K, V>>> iterator() override {
            return jxx::Ptr<Iterator<MapEntry<K, V>>>(new LinkedEntryIterator(linkedMap_));
        }
    };

    virtual jxx::Ptr<Set<MapEntry<K, V>>> createEntrySetView() override {
        return jxx::Ptr<Set<MapEntry<K, V>>>(new LinkedEntrySet(this));
    }
};

} // namespace util
} // namespace jxx
