#pragma once

#include <cmath>
#include <cstddef>
#include <memory>
#include <unordered_set>
#include <vector>

#include "io/jxx.io.Serializable.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.AbstractCollection.h"
#include "util/jxx.util.AbstractSet.h"
#include "util/jxx.util.Collection.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"

namespace jxx {
    namespace util {

        template <typename E>
        class ElemHash {
        public:
            std::size_t operator()(const jxx::Ptr<E>& elem) const {
                if (elem == nullptr) {
                    return 0u;
                }

                auto obj = jxx::CAST<jxx::lang::Object>(elem);
                if (obj == nullptr) {
                    return reinterpret_cast<std::size_t>(elem.get());
                }

                return static_cast<std::size_t>(obj->hashCode());
            }
        };

        template <typename E>
        class ElemEq {
        public:
            bool operator()(const jxx::Ptr<E>& a, const jxx::Ptr<E>& b) const {
                if (a == nullptr || b == nullptr) {
                    return a == b;
                }

                auto ao = jxx::CAST<jxx::lang::Object>(a);
                if (ao == nullptr) {
                    return a.get() == b.get();
                }

                return ao->equals(jxx::CAST<jxx::lang::Object>(b));
            }
        };

        template <typename E>
        class HashSetIterator final : public virtual Iterator<E> {
        public:
            HashSet<E>* owner_;
            std::vector<jxx::Ptr<E>> snapshot_;
            std::size_t cursor_;
            jxx::Ptr<E> lastReturned_;
            jxx::lang::jbool canRemove_;
            jxx::lang::jint expectedModCount_;

            void checkForComodification() const {
                if (owner_->modCount_ != expectedModCount_) {
                    throw jxx::util::ConcurrentModificationException();
                }
            }

            explicit HashSetIterator(HashSet<E>* owner)
                : owner_(owner)
                , snapshot_()
                , cursor_(0)
                , lastReturned_(nullptr)
                , canRemove_(static_cast<jxx::lang::jbool>(false))
                , expectedModCount_(owner->modCount_) {
                snapshot_.reserve(owner_->set_.size());
                for (const auto& e : owner_->set_) {
                    snapshot_.push_back(e);
                }
            }

            virtual ~HashSetIterator() = default;

            virtual jxx::lang::jbool hasNext() override {
                return static_cast<jxx::lang::jbool>(cursor_ < snapshot_.size());
            }

            virtual jxx::Ptr<E> next() override {
                checkForComodification();

                if (cursor_ >= snapshot_.size()) {
                    throw jxx::util::NoSuchElementException();
                }

                lastReturned_ = snapshot_[cursor_++];
                canRemove_ = static_cast<jxx::lang::jbool>(true);
                return lastReturned_;
            }

            virtual void remove() override {
                if (!canRemove_) {
                    throw jxx::lang::IllegalStateException();
                }

                checkForComodification();

                owner_->remove(jxx::CAST<jxx::lang::Object>(lastReturned_));

                expectedModCount_ = owner_->modCount_;
                lastReturned_ = nullptr;
                canRemove_ = static_cast<jxx::lang::jbool>(false);
            }
        };

        template <typename E>
        class HashSet
            : public virtual AbstractSet<E>
            , public virtual jxx::lang::Cloneable
            , public virtual jxx::io::Serializable {
        private:

            using InternalSet = std::unordered_set<jxx::Ptr<E>, ElemHash, ElemEq>;

            static constexpr jxx::lang::jint DEFAULT_INITIAL_CAPACITY = 16;
            static constexpr jxx::lang::jfloat DEFAULT_LOAD_FACTOR = 0.75f;

            InternalSet set_;
            jxx::lang::jfloat loadFactor_;
            jxx::lang::jint modCount_;

            static jxx::Ptr<E> castObjectToElement(jxx::Ptr<jxx::lang::Object> o) {
                return jxx::CAST<E>(o);
            }

        public:
            HashSet()
                : set_()
                , loadFactor_(DEFAULT_LOAD_FACTOR)
                , modCount_(0) {
                set_.max_load_factor(static_cast<float>(loadFactor_));
                set_.reserve(static_cast<std::size_t>(DEFAULT_INITIAL_CAPACITY));
            }

            explicit HashSet(jxx::lang::jint initialCapacity)
                : HashSet(initialCapacity, DEFAULT_LOAD_FACTOR) {}

            HashSet(jxx::lang::jint initialCapacity, jxx::lang::jfloat loadFactor)
                : set_()
                , loadFactor_(loadFactor)
                , modCount_(0) {
                if (initialCapacity < 0) {
                    throw jxx::lang::IllegalArgumentException();
                }

                if (!(loadFactor > 0.0f) || std::isnan(static_cast<double>(loadFactor))) {
                    throw jxx::lang::IllegalArgumentException();
                }

                set_.max_load_factor(static_cast<float>(loadFactor_));
                set_.reserve(static_cast<std::size_t>(initialCapacity));
            }

            explicit HashSet(jxx::Ptr<wildcard::CollectionExtends<E>> c)
                : HashSet() {
                if (c == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                addAll(c);
            }

            virtual ~HashSet() = default;

            /*
             * Your Serializable interface is pure virtual, not marker-only.
             * These methods satisfy the interface without assuming stream APIs.
             */
            virtual void writeObject(jxx::Ptr<jxx::io::ObjectOutputStream> out) override {
                if (out == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                throw jxx::lang::UnsupportedOperationException();
            }

            virtual void readObject(jxx::Ptr<jxx::io::ObjectInputStream> in) override {
                if (in == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                throw jxx::lang::UnsupportedOperationException();
            }

            virtual void readObjectNoData() override {
                throw jxx::lang::UnsupportedOperationException();
            }

            virtual jxx::Ptr<Iterator<E>> iterator() override {
                return jxx::Ptr<Iterator<E>>(new HashSetIterator(this));
            }

            virtual jxx::lang::jint size() override {
                return static_cast<jxx::lang::jint>(set_.size());
            }

            virtual jxx::lang::jbool isEmpty() override {
                return static_cast<jxx::lang::jbool>(set_.empty());
            }

            virtual jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> o) override {
                if (o == nullptr) {
                    return static_cast<jxx::lang::jbool>(set_.find(nullptr) != set_.end());
                }

                auto casted = castObjectToElement(o);
                if (casted == nullptr) {
                    return static_cast<jxx::lang::jbool>(false);
                }

                return static_cast<jxx::lang::jbool>(set_.find(casted) != set_.end());
            }

            virtual jxx::lang::jbool add(jxx::Ptr<E> e) override {
                auto result = set_.insert(e);
                if (!result.second) {
                    return static_cast<jxx::lang::jbool>(false);
                }

                ++modCount_;
                return static_cast<jxx::lang::jbool>(true);
            }

            virtual jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> o) override {
                jxx::Ptr<E> casted = nullptr;

                if (o != nullptr) {
                    casted = castObjectToElement(o);
                    if (casted == nullptr) {
                        return static_cast<jxx::lang::jbool>(false);
                    }
                }

                auto it = set_.find(casted);
                if (it == set_.end()) {
                    return static_cast<jxx::lang::jbool>(false);
                }

                set_.erase(it);
                ++modCount_;
                return static_cast<jxx::lang::jbool>(true);
            }

            virtual void clear() override {
                if (!set_.empty()) {
                    set_.clear();
                    ++modCount_;
                }
            }

            virtual jxx::lang::jbool containsAll(jxx::Ptr<wildcard::CollectionAny> c) override {
                if (c == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                auto it = c->iteratorObject();
                while (it->hasNext()) {
                    if (!contains(it->next())) {
                        return static_cast<jxx::lang::jbool>(false);
                    }
                }

                return static_cast<jxx::lang::jbool>(true);
            }

            virtual jxx::lang::jbool addAll(jxx::Ptr<wildcard::CollectionExtends<E>> c) override {
                if (c == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                jxx::lang::jbool modified = static_cast<jxx::lang::jbool>(false);

                auto it = c->iteratorExtends();
                while (it->hasNext()) {
                    if (add(it->next())) {
                        modified = static_cast<jxx::lang::jbool>(true);
                    }
                }

                return modified;
            }

            virtual jxx::lang::jbool retainAll(jxx::Ptr<wildcard::CollectionAny> c) override {
                if (c == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                jxx::lang::jbool modified = static_cast<jxx::lang::jbool>(false);

                auto it = iterator();
                while (it->hasNext()) {
                    auto e = it->next();
                    auto obj = jxx::CAST<jxx::lang::Object>(e);

                    if (!c->containsObject(obj)) {
                        it->remove();
                        modified = static_cast<jxx::lang::jbool>(true);
                    }
                }

                return modified;
            }

            virtual jxx::lang::jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) override {
                if (c == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                jxx::lang::jbool modified = static_cast<jxx::lang::jbool>(false);

                auto it = c->iteratorObject();
                while (it->hasNext()) {
                    if (remove(it->next())) {
                        modified = static_cast<jxx::lang::jbool>(true);
                    }
                }

                return modified;
            }

            virtual jxx::Ptr<jxx::lang::Object> clone() {
                auto cloned = jxx::Ptr<HashSet<E>>(
                    new HashSet<E>(
                        static_cast<jxx::lang::jint>(set_.bucket_count()),
                        loadFactor_
                    )
                );

                for (const auto& e : set_) {
                    cloned->set_.insert(e);
                }

                cloned->modCount_ = 0;

                return jxx::CAST<jxx::lang::Object>(cloned);
            }
        };
    }
}