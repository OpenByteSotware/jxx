#pragma once

#include <cstddef>
#include <list>
#include <memory>
#include <unordered_map>
#include <vector>

#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"

#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.HashMap.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.Map.h"
#include "util/jxx.util.MapEntry.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.Set.h"
#include "lang/jxx.lang.ClassInfo.h"

namespace jxx {
    namespace util {

        template <typename K, typename V>
		class LinkedHashMap : public jxx::lang::ClassBase <LinkedHashMap<K, V>, HashMap<K, V>>
        {
        private:
            struct OrderKeyHash {
                std::size_t operator()(
                    const jxx::Ptr<K>& key) const {

                    if (key == nullptr) {
                        return 0u;
                    }

                    auto object =
                        jxx::CAST<jxx::lang::Object>(key);

                    if (object == nullptr) {
                        return reinterpret_cast<std::size_t>(
                            key.get());
                    }

                    return static_cast<std::size_t>(
                        object->hashCode());
                }
            };

            struct OrderKeyEqual {
                bool operator()(
                    const jxx::Ptr<K>& left,
                    const jxx::Ptr<K>& right) const {

                    if (left == nullptr || right == nullptr) {
                        return left == right;
                    }

                    auto leftObject =
                        jxx::CAST<jxx::lang::Object>(left);

                    if (leftObject == nullptr) {
                        return left.get() == right.get();
                    }

                    return leftObject->equals(
                        jxx::CAST<jxx::lang::Object>(
                            right));
                }
            };

            using OrderList =
                std::list<jxx::Ptr<K>>;

            using OrderIterator =
                typename OrderList::iterator;

            using OrderIndexMap =
                std::unordered_map<
                jxx::Ptr<K>,
                OrderIterator,
                OrderKeyHash,
                OrderKeyEqual>;

            OrderList order_;
            OrderIndexMap orderIndex_;
            jxx::lang::jbool accessOrder_;

            void appendOrderKey_(const jxx::Ptr<K> key) {
                order_.push_back(key);

                auto tail = order_.end();
                --tail;

                orderIndex_.emplace(key, tail);
            }

            void removeOrderKey_(const jxx::Ptr<K> key) {
                auto found = orderIndex_.find(key);

                if (found == orderIndex_.end()) {
                    return;
                }

                order_.erase(found->second);
                orderIndex_.erase(found);
            }

            jxx::lang::jbool moveOrderKeyToEnd_(
                jxx::Ptr<K> key) {

                auto found = orderIndex_.find(key);

                if (found == orderIndex_.end()) {
                    return static_cast<jxx::lang::jbool>(
                        false);
                }

                if (order_.empty()) {
                    return static_cast<jxx::lang::jbool>(
                        false);
                }

                auto tail = order_.end();
                --tail;

                if (found->second == tail) {
                    return static_cast<jxx::lang::jbool>(
                        false);
                }

                /*
                 * splice moves the list node without copying the key.
                 * Iterators to the moved element remain valid.
                 */
                order_.splice(
                    order_.end(),
                    order_,
                    found->second);

                auto newTail = order_.end();
                --newTail;

                found->second = newTail;

                return static_cast<jxx::lang::jbool>(
                    true);
            }

        public:
            LinkedHashMap()
                : HashMap<K, V>()
                , order_()
                , orderIndex_()
                , accessOrder_(
                    static_cast<jxx::lang::jbool>(
                        false)) {}

            explicit LinkedHashMap(
                jxx::lang::jint initialCapacity)
                : HashMap<K, V>(initialCapacity)
                , order_()
                , orderIndex_()
                , accessOrder_(
                    static_cast<jxx::lang::jbool>(
                        false)) {

                if (initialCapacity > 0) {
                    orderIndex_.reserve(
                        static_cast<std::size_t>(
                            initialCapacity));
                }
            }

            LinkedHashMap(
                jxx::lang::jint initialCapacity,
                jxx::lang::jfloat loadFactor)
                : HashMap<K, V>(
                    initialCapacity,
                    loadFactor)
                , order_()
                , orderIndex_()
                , accessOrder_(
                    static_cast<jxx::lang::jbool>(
                        false)) {

                if (initialCapacity > 0) {
                    orderIndex_.reserve(
                        static_cast<std::size_t>(
                            initialCapacity));
                }
            }

            LinkedHashMap(
                jxx::lang::jint initialCapacity,
                jxx::lang::jfloat loadFactor,
                jxx::lang::jbool accessOrder)
                : HashMap<K, V>(
                    initialCapacity,
                    loadFactor)
                , order_()
                , orderIndex_()
                , accessOrder_(accessOrder) {

                if (initialCapacity > 0) {
                    orderIndex_.reserve(
                        static_cast<std::size_t>(
                            initialCapacity));
                }
            }

            explicit LinkedHashMap(
                jxx::Ptr<Map<K, V>> source)
                : LinkedHashMap() {

                if (source == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                this->putAll(source);
            }

            virtual ~LinkedHashMap() = default;

        protected:
            /*
             * Java 8:
             * protected boolean removeEldestEntry(
             *     Map.Entry<K,V> eldest)
             */
            virtual jxx::lang::jbool removeEldestEntry(
                jxx::Ptr<MapEntry<K, V>>
            /* eldest */) {

                return static_cast<jxx::lang::jbool>(
                    false);
            }

            virtual void afterNodeAccess(
                jxx::Ptr<K> key) override {

                if (!accessOrder_) {
                    return;
                }

                if (moveOrderKeyToEnd_(key)) {
                    /*
                     * Java LinkedHashMap considers an access-order
                     * reordering a structural modification.
                     */
                    this->incrementModificationCount_();
                }
            }

            virtual void afterNodeInsertion(const jxx::Ptr<K>& key,
                jxx::lang::jbool isNewKey) override {

                if (!isNewKey) {
                    return;
                }

                appendOrderKey_(key);

                if (order_.empty()) {
                    return;
                }

                auto eldestKey = order_.front();

                auto eldestEntry =
                    this->makeEntryView(eldestKey);

                if (removeEldestEntry(eldestEntry)) {
                    this->remove(
                        jxx::CAST<jxx::lang::Object>(
                            eldestKey));
                }
            }

            virtual void afterNodeRemoval(
                jxx::Ptr<K> key) override {

                removeOrderKey_(key);
            }

            virtual void afterClear() override {
                order_.clear();
                orderIndex_.clear();
            }

            class LinkedEntryIterator final
                : public virtual Iterator<MapEntry<K, V>> {
            private:
                LinkedHashMap<K, V>* owner_;

                /*
                 * Keep a snapshot of Java/JXX key references.
                 * No STL type is exposed publicly.
                 */
                std::vector<jxx::Ptr<K>> keys_;
                std::size_t cursor_;

                jxx::Ptr<K> lastReturnedKey_;
                jxx::lang::jbool canRemove_;
                jxx::lang::jint expectedModCount_;

                void checkForComodification() const {
                    if (owner_->modificationCount_() !=
                        expectedModCount_) {

                        throw jxx::util::
                            ConcurrentModificationException();
                    }
                }

            public:
                explicit LinkedEntryIterator(
                    LinkedHashMap<K, V>* owner)
                    : owner_(owner)
                    , keys_()
                    , cursor_(0)
                    , lastReturnedKey_(nullptr)
                    , canRemove_(
                        static_cast<jxx::lang::jbool>(
                            false))
                    , expectedModCount_(
                        owner->modificationCount_()) {

                    keys_.reserve(owner_->order_.size());

                    for (const auto& key : owner_->order_) {
                        keys_.push_back(key);
                    }
                }

                virtual ~LinkedEntryIterator() = default;

                virtual jxx::lang::jbool hasNext()
                    override {

                    return static_cast<jxx::lang::jbool>(
                        cursor_ < keys_.size());
                }

                virtual jxx::Ptr<MapEntry<K, V>>
                    next() override {

                    checkForComodification();

                    if (cursor_ >= keys_.size()) {
                        throw jxx::util::
                            NoSuchElementException();
                    }

                    lastReturnedKey_ =
                        keys_[cursor_++];

                    canRemove_ =
                        static_cast<jxx::lang::jbool>(
                            true);

                    return owner_->makeEntryView(
                        lastReturnedKey_);
                }

                virtual void remove() override {
                    if (!canRemove_) {
                        throw jxx::lang::
                            IllegalStateException();
                    }

                    checkForComodification();

                    owner_->remove(
                        jxx::CAST<jxx::lang::Object>(
                            lastReturnedKey_));

                    expectedModCount_ =
                        owner_->modificationCount_();

                    lastReturnedKey_ = nullptr;

                    canRemove_ =
                        static_cast<jxx::lang::jbool>(
                            false);
                }
            };

            class LinkedEntrySet
                : public HashMap<K, V>::EntrySet {
            private:
                LinkedHashMap<K, V>* owner_;

            public:
                explicit LinkedEntrySet(
                    LinkedHashMap<K, V>* owner)
                    : HashMap<K, V>::EntrySet(owner)
                    , owner_(owner) {}

                virtual ~LinkedEntrySet() = default;

                virtual jxx::Ptr<
                    Iterator<MapEntry<K, V>>>
                    iterator() override {

                    return jxx::Ptr<
                        Iterator<MapEntry<K, V>>>(
                            new LinkedEntryIterator(
                                owner_));
                }
            };

            virtual jxx::Ptr<
                Set<MapEntry<K, V>>>
                createEntrySetView() override {

                return jxx::Ptr<
                    Set<MapEntry<K, V>>>(
                        new LinkedEntrySet(this));
            }

        public:
            virtual jxx::Ptr<jxx::lang::Object>
                clone() {

                auto cloned =
                    jxx::Ptr<LinkedHashMap<K, V>>(
                        new LinkedHashMap<K, V>(
                            this->size(),
                            this->loadFactorValue_(),
                            accessOrder_));

                /*
                 * Use the base public API instead of directly
                 * accessing HashMap's private unordered_map.
                 *
                 * Temporarily disable access-order behavior while
                 * reading values from this map so clone() does not
                 * reorder the source map.
                 */
                const jxx::lang::jbool savedAccessOrder =
                    accessOrder_;

                accessOrder_ =
                    static_cast<jxx::lang::jbool>(false);

                for (const auto& key : order_) {
                    auto value = this->get(
                        jxx::CAST<jxx::lang::Object>(
                            key));

                    cloned->put(key, value);
                }

                accessOrder_ = savedAccessOrder;

                return jxx::CAST<jxx::lang::Object>(
                    cloned);
            }
        };

    } // namespace util
} // namespace jxx