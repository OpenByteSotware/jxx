#pragma once

#include <cmath>
#include <cstddef>
#include <memory>
#include <unordered_set>
#include <vector>

#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.AbstractSet.h"
#include "util/jxx.util.Collection.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.NoSuchElementException.h"

namespace jxx {
    namespace util {

        /*
         * HashSet must be declared before HashSetIterator references it.
         */
        template <typename E>
        class HashSet;

        template <typename E>
        class ElemHash final {
        public:
            std::size_t operator()(
                const jxx::Ptr<E>& element) const {

                if (element == nullptr) {
                    return 0u;
                }

                auto object =
                    jxx::CAST<jxx::lang::Object>(element);

                if (object == nullptr) {
                    return reinterpret_cast<std::size_t>(
                        element.get());
                }

                return static_cast<std::size_t>(
                    object->hashCode());
            }
        };

        template <typename E>
        class ElemEq final {
        public:
            bool operator()(
                const jxx::Ptr<E>& left,
                const jxx::Ptr<E>& right) const {

                if (left == nullptr || right == nullptr) {
                    return left == right;
                }

                auto leftObject =
                    jxx::CAST<jxx::lang::Object>(left);

                auto rightObject =
                    jxx::CAST<jxx::lang::Object>(right);

                if (leftObject == nullptr ||
                    rightObject == nullptr) {

                    return left.get() == right.get();
                }

                return leftObject->equals(rightObject);
            }
        };

        template <typename E>
        class HashSetIterator final
            : public virtual Iterator<E> {
        private:
            HashSet<E>* owner_;
            std::vector<jxx::Ptr<E>> snapshot_;
            std::size_t cursor_;
            jxx::Ptr<E> lastReturned_;
            jxx::lang::jbool canRemove_;
            jxx::lang::jint expectedModCount_;

        private:
            void checkForComodification() const {
                if (owner_->modCount_ != expectedModCount_) {
                    throw jxx::util::
                        ConcurrentModificationException();
                }
            }

        public:
            explicit HashSetIterator(HashSet<E>* owner)
                : owner_(owner)
                , snapshot_()
                , cursor_(0)
                , lastReturned_(nullptr)
                , canRemove_(
                    static_cast<jxx::lang::jbool>(false))
                , expectedModCount_(owner->modCount_) {

                snapshot_.reserve(owner_->set_.size());

                for (const auto& element : owner_->set_) {
                    snapshot_.push_back(element);
                }
            }

            virtual ~HashSetIterator() = default;

            virtual jxx::lang::jbool hasNext() override {
                return static_cast<jxx::lang::jbool>(
                    cursor_ < snapshot_.size());
            }

            virtual jxx::Ptr<E> next() override {
                checkForComodification();

                if (cursor_ >= snapshot_.size()) {
                    throw jxx::util::NoSuchElementException();
                }

                lastReturned_ = snapshot_[cursor_++];

                canRemove_ =
                    static_cast<jxx::lang::jbool>(true);

                return lastReturned_;
            }

            virtual void remove() override {
                if (!canRemove_) {
                    throw jxx::lang::IllegalStateException();
                }

                checkForComodification();

                owner_->remove(
                    jxx::CAST<jxx::lang::Object>(
                        lastReturned_));

                expectedModCount_ = owner_->modCount_;
                lastReturned_ = nullptr;

                canRemove_ =
                    static_cast<jxx::lang::jbool>(false);
            }
        };

        template <typename E>
        class HashSet
            : public AbstractSet<E>
            , public virtual jxx::lang::Cloneable
            , public virtual jxx::io::SerializableI {
        private:
            /*
             * Allow the external iterator implementation to access set_
             * and modCount_.
             */
            template <typename T>
            friend class HashSetIterator;

            /*
             * ElemHash and ElemEq require their template arguments.
             */
            using InternalSet =
                std::unordered_set<
                jxx::Ptr<E>,
                ElemHash<E>,
                ElemEq<E>>;

            static constexpr jxx::lang::jint
                DEFAULT_INITIAL_CAPACITY = 16;

            static constexpr jxx::lang::jfloat
                DEFAULT_LOAD_FACTOR = 0.75f;

            InternalSet set_;
            jxx::lang::jfloat loadFactor_;
            jxx::lang::jint modCount_;

        private:
            static jxx::Ptr<E> castObjectToElement(
                jxx::Ptr<jxx::lang::Object> object) {

                return jxx::CAST<E>(object);
            }

        public:
            HashSet()
                : set_()
                , loadFactor_(DEFAULT_LOAD_FACTOR)
                , modCount_(0) {

                set_.max_load_factor(
                    static_cast<float>(loadFactor_));

                set_.reserve(
                    static_cast<std::size_t>(
                        DEFAULT_INITIAL_CAPACITY));
            }

            explicit HashSet(
                jxx::lang::jint initialCapacity)
                : HashSet(
                    initialCapacity,
                    DEFAULT_LOAD_FACTOR) {}

            HashSet(
                jxx::lang::jint initialCapacity,
                jxx::lang::jfloat loadFactor)
                : set_()
                , loadFactor_(loadFactor)
                , modCount_(0) {

                if (initialCapacity < 0) {
                    throw jxx::lang::IllegalArgumentException();
                }

                if (!(loadFactor > 0.0f) ||
                    std::isnan(
                        static_cast<double>(loadFactor))) {

                    throw jxx::lang::IllegalArgumentException();
                }

                set_.max_load_factor(
                    static_cast<float>(loadFactor_));

                set_.reserve(
                    static_cast<std::size_t>(
                        initialCapacity));
            }

            explicit HashSet(
                jxx::Ptr<wildcard::CollectionExtends<E>>
                collection)
                : HashSet() {

                if (collection == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                addAll(collection);
            }

            virtual ~HashSet() = default;

            virtual void writeObject(
                jxx::Ptr<jxx::io::ObjectOutputStream> out)
                override {

                if (out == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                throw jxx::lang::UnsupportedOperationException();
            }

            virtual void readObject(
                jxx::Ptr<jxx::io::ObjectInputStream> in)
                override {

                if (in == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                throw jxx::lang::UnsupportedOperationException();
            }

            virtual void readObjectNoData() override {
                throw jxx::lang::UnsupportedOperationException();
            }

            virtual jxx::Ptr<Iterator<E>>
                iterator() override {

                /*
                 * The template argument is explicit.
                 */
                return jxx::Ptr<Iterator<E>>(
                    new HashSetIterator<E>(this));
            }

            virtual jxx::lang::jint size() override {
                return static_cast<jxx::lang::jint>(
                    set_.size());
            }

            virtual jxx::lang::jbool isEmpty() override {
                return static_cast<jxx::lang::jbool>(
                    set_.empty());
            }

            virtual jxx::lang::jbool contains(const jxx::Ptr<jxx::lang::Object>& object)
                override {

                if (object == nullptr) {
                    return static_cast<jxx::lang::jbool>(
                        set_.find(nullptr) != set_.end());
                }

                auto element =
                    castObjectToElement(object);

                if (element == nullptr) {
                    return static_cast<jxx::lang::jbool>(
                        false);
                }

                return static_cast<jxx::lang::jbool>(
                    set_.find(element) != set_.end());
            }

            virtual jxx::lang::jbool add(
                const jxx::Ptr<E>& element) override {

                const auto result =
                    set_.insert(element);

                if (!result.second) {
                    return static_cast<jxx::lang::jbool>(
                        false);
                }

                ++modCount_;

                return static_cast<jxx::lang::jbool>(true);
            }

            virtual jxx::lang::jbool remove(
                jxx::Ptr<jxx::lang::Object> object)
                override {

                jxx::Ptr<E> element = nullptr;

                if (object != nullptr) {
                    element = castObjectToElement(object);

                    if (element == nullptr) {
                        return static_cast<jxx::lang::jbool>(
                            false);
                    }
                }

                auto found = set_.find(element);

                if (found == set_.end()) {
                    return static_cast<jxx::lang::jbool>(
                        false);
                }

                set_.erase(found);
                ++modCount_;

                return static_cast<jxx::lang::jbool>(true);
            }

            virtual void clear() override {
                if (set_.empty()) {
                    return;
                }

                set_.clear();
                ++modCount_;
            }

            virtual jxx::lang::jbool containsAll(
                const jxx::Ptr<wildcard::CollectionAny>&
                collection) override {

                if (collection == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                auto iterator =
                    collection->iteratorObject();

                while (iterator->hasNext()) {
                    if (!contains(iterator->next())) {
                        return static_cast<jxx::lang::jbool>(
                            false);
                    }
                }

                return static_cast<jxx::lang::jbool>(true);
            }

            virtual jxx::lang::jbool addAll(const jxx::Ptr<wildcard::CollectionExtends<E>>& collection) override {

                if (collection == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                jxx::lang::jbool modified =
                    static_cast<jxx::lang::jbool>(false);

                auto iterator =
                    collection->iteratorExtends();

                while (iterator->hasNext()) {
                    if (add(iterator->next())) {
                        modified =
                            static_cast<jxx::lang::jbool>(
                                true);
                    }
                }

                return modified;
            }

            virtual jxx::lang::jbool retainAll(const
                jxx::Ptr<wildcard::CollectionAny>&
                collection) override {

                if (collection == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                jxx::lang::jbool modified =
                    static_cast<jxx::lang::jbool>(false);

                auto iterator = this->iterator();

                while (iterator->hasNext()) {
                    auto element = iterator->next();

                    auto object =
                        jxx::CAST<jxx::lang::Object>(
                            element);

                    if (!collection->containsObject(object)) {
                        iterator->remove();

                        modified =
                            static_cast<jxx::lang::jbool>(
                                true);
                    }
                }

                return modified;
            }

            virtual jxx::lang::jbool removeAll(const jxx::Ptr<wildcard::CollectionAny>& collection) override {

                if (collection == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                jxx::lang::jbool modified =
                    static_cast<jxx::lang::jbool>(false);

                auto iterator =
                    collection->iteratorObject();

                while (iterator->hasNext()) {
                    if (remove(iterator->next())) {
                        modified =
                            static_cast<jxx::lang::jbool>(
                                true);
                    }
                }

                return modified;
            }

            virtual jxx::Ptr<jxx::lang::Object> clone()
            {
                auto cloned = jxx::NEW<HashSet<E>>(static_cast<jxx::lang::jint>(set_.size()), loadFactor_);

                    for (const auto& element : set_)
                    {
                        cloned->set_.insert(element);
                    }

                    cloned->modCount_ = 0;
                    return jxx::CAST<jxx::lang::Object>(cloned);
            }
        };
    }
}