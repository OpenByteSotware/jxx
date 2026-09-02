#pragma once

#include <cstddef>
#include <list>
#include <unordered_map>
#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.HashMap.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.Map.h"
#include "util/jxx.util.MapEntry.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.Set.h"

namespace jxx::util {

template <typename K, typename V>
class LinkedHashMap
    : public jxx::lang::ClassBase<LinkedHashMap<K, V>, HashMap<K, V>> {
public:
    using JavaSuper = HashMap<K, V>;
    using Super =
        jxx::lang::ClassBase<
            LinkedHashMap<K, V>,
            JavaSuper>;

    struct OrderKeyHash {
        std::size_t operator()(const jxx::Ptr<K>& key) const {
            if (key == nullptr) {
                return 0U;
            }

            const auto object =
                jxx::CAST<jxx::lang::Object>(key);

            if (object == nullptr) {
                return reinterpret_cast<std::size_t>(key.get());
            }

            return static_cast<std::size_t>(object->hashCode());
        }
    };

    struct OrderKeyEqual {
        bool operator()(
            const jxx::Ptr<K>& left,
            const jxx::Ptr<K>& right) const {

            if (left == nullptr || right == nullptr) {
                return left == right;
            }

            const auto leftObject =
                jxx::CAST<jxx::lang::Object>(left);

            if (leftObject == nullptr) {
                return left.get() == right.get();
            }

            return leftObject->equals(
                jxx::CAST<jxx::lang::Object>(right));
        }
    };

    using OrderList = std::list<jxx::Ptr<K>>;
    using OrderIterator = typename OrderList::iterator;
    using OrderIndexMap =
        std::unordered_map<
            jxx::Ptr<K>,
            OrderIterator,
            OrderKeyHash,
            OrderKeyEqual>;

    OrderList order_;
    OrderIndexMap orderIndex_;
    jxx::lang::jbool accessOrder_ = false;

    void appendOrderKey_(const jxx::Ptr<K>& key) {
        order_.push_back(key);
        auto tail = order_.end();
        --tail;
        orderIndex_.emplace(key, tail);
    }

    void removeOrderKey_(const jxx::Ptr<K>& key) {
        const auto found = orderIndex_.find(key);
        if (found == orderIndex_.end()) {
            return;
        }

        order_.erase(found->second);
        orderIndex_.erase(found);
    }

    jxx::lang::jbool moveOrderKeyToEnd_(const jxx::Ptr<K>& key) {
        const auto found = orderIndex_.find(key);
        if (found == orderIndex_.end() || order_.empty()) {
            return false;
        }

        auto tail = order_.end();
        --tail;
        if (found->second == tail) {
            return false;
        }

        order_.splice(order_.end(), order_, found->second);
        auto newTail = order_.end();
        --newTail;
        found->second = newTail;
        return true;
    }

public:
    LinkedHashMap()
        : Super()
        , order_()
        , orderIndex_()
        , accessOrder_(false) {
    }

    explicit LinkedHashMap(jxx::lang::jint initialCapacity)
        : Super(initialCapacity)
        , order_()
        , orderIndex_()
        , accessOrder_(false) {

        if (initialCapacity > 0) {
            orderIndex_.reserve(
                static_cast<std::size_t>(initialCapacity));
        }
    }

    LinkedHashMap(
        jxx::lang::jint initialCapacity,
        jxx::lang::jfloat loadFactor)
        : Super(initialCapacity, loadFactor)
        , order_()
        , orderIndex_()
        , accessOrder_(false) {

        if (initialCapacity > 0) {
            orderIndex_.reserve(
                static_cast<std::size_t>(initialCapacity));
        }
    }

    LinkedHashMap(
        jxx::lang::jint initialCapacity,
        jxx::lang::jfloat loadFactor,
        jxx::lang::jbool accessOrder)
        : Super(initialCapacity, loadFactor)
        , order_()
        , orderIndex_()
        , accessOrder_(accessOrder) {

        if (initialCapacity > 0) {
            orderIndex_.reserve(
                static_cast<std::size_t>(initialCapacity));
        }
    }

    explicit LinkedHashMap(
        const jxx::Ptr<Map<K, V>>& source)
        : LinkedHashMap() {

        if (source == nullptr) {
            throw jxx::lang::NullPointerException();
        }

        this->putAll(source);
    }

    ~LinkedHashMap() override = default;

protected:
    virtual jxx::lang::jbool removeEldestEntry(
        const jxx::Ptr<MapEntry<K, V>>& /* eldest */) {
        return false;
    }

    void afterNodeAccess(const jxx::Ptr<K>& key) override {
        if (accessOrder_ && moveOrderKeyToEnd_(key)) {
            this->incrementModificationCount_();
        }
    }

    void afterNodeInsertion(
        const jxx::Ptr<K>& key,
        jxx::lang::jbool isNewKey) override {

        if (!isNewKey) {
            return;
        }

        appendOrderKey_(key);
        if (order_.empty()) {
            return;
        }

        const auto eldestKey = order_.front();
        const auto eldestEntry = this->makeEntryView(eldestKey);

        if (removeEldestEntry(eldestEntry)) {
            this->remove(
                jxx::CAST<jxx::lang::Object>(eldestKey));
        }
    }

    void afterNodeRemoval(const jxx::Ptr<K>& key) override {
        removeOrderKey_(key);
    }

    void afterClear() override {
        order_.clear();
        orderIndex_.clear();
    }

    class LinkedEntryIterator final
        : public jxx::lang::ClassBase<
              LinkedEntryIterator,
              jxx::lang::Object,
              Iterator<MapEntry<K, V>>> {
    private:
        jxx::Ptr<LinkedHashMap<K, V>> owner_;
        std::vector<jxx::Ptr<K>> keys_;
        std::size_t cursor_ = 0;
        jxx::Ptr<K> lastReturnedKey_;
        jxx::lang::jbool canRemove_ = false;
        jxx::lang::jint expectedModCount_ = 0;

        void checkForComodification_() const {
            if (owner_->modificationCount_() != expectedModCount_) {
                throw ConcurrentModificationException();
            }
        }

    public:
        explicit LinkedEntryIterator(
            const jxx::Ptr<LinkedHashMap<K, V>>& owner)
            : owner_(owner) {

            if (owner_ == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            expectedModCount_ = owner_->modificationCount_();
            keys_.reserve(owner_->order_.size());
            for (const auto& key : owner_->order_) {
                keys_.push_back(key);
            }
        }

        ~LinkedEntryIterator() override = default;

        jxx::lang::jbool hasNext() override {
            return cursor_ < keys_.size();
        }

        jxx::Ptr<MapEntry<K, V>> next() override {
            checkForComodification_();
            if (cursor_ >= keys_.size()) {
                throw NoSuchElementException();
            }

            lastReturnedKey_ = keys_[cursor_++];
            canRemove_ = true;
            return owner_->makeEntryView(lastReturnedKey_);
        }

        void remove() override {
            if (!canRemove_) {
                throw jxx::lang::IllegalStateException();
            }

            checkForComodification_();
            owner_->remove(
                jxx::CAST<jxx::lang::Object>(lastReturnedKey_));
            expectedModCount_ = owner_->modificationCount_();
            lastReturnedKey_ = nullptr;
            canRemove_ = false;
        }
    };

    using HashMapEntrySet = typename HashMap<K, V>::EntrySet;

    class LinkedEntrySet final
        : public HashMapEntrySet {
    private:
        jxx::Ptr<LinkedHashMap<K, V>> owner_;

    public:
        explicit LinkedEntrySet(
            const jxx::Ptr<LinkedHashMap<K, V>>& owner)
            : HashMapEntrySet(
                  jxx::CAST<HashMap<K, V>>(owner))
            , owner_(owner) {

            if (owner_ == nullptr) {
                throw jxx::lang::NullPointerException();
            }
        }

        ~LinkedEntrySet() override = default;

        jxx::Ptr<Iterator<MapEntry<K, V>>> iterator() override {
            auto iteratorValue =
                jxx::NEW<LinkedEntryIterator>(owner_);

            return jxx::CAST<Iterator<MapEntry<K, V>>>(
                iteratorValue);
        }
    };

    jxx::Ptr<Set<MapEntry<K, V>>>
    createEntrySetView() override {
        auto owner =
            jxx::CAST<LinkedHashMap<K, V>>(this->thisPtr);

        if (owner == nullptr) {
            throw jxx::lang::IllegalStateException();
        }

        auto view = jxx::NEW<LinkedEntrySet>(owner);
        return jxx::CAST<Set<MapEntry<K, V>>>(view);
    }

public:
    jxx::Ptr<jxx::lang::Object> clone() const override {
        auto cloned =
            jxx::NEW<LinkedHashMap<K, V>>(
                static_cast<jxx::lang::jint>(order_.size()),
                this->loadFactorValue_(),
                accessOrder_);

        for (const auto& key : order_) {
            cloned->put(key, this->getWithoutAccess_(key));
        }

        return jxx::CAST<jxx::lang::Object>(cloned);
    }
};

} // namespace jxx::util
