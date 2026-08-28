#pragma once

#include <cstddef>
#include <memory>
#include <set>
#include <vector>

#include "io/jxx.io.SerializableI.h"

#include "lang/jxx.lang.ClassCastException.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"

#include "util/jxx.util.AbstractSet.h"
#include "util/jxx.util.Collection.h"
#include "util/jxx.util.ComparatorSuper.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.NoSuchElementException.h"

namespace jxx {
    namespace util {

        /*
         * Forward-declare TreeSet before the external iterator references it.
         */
        template <typename E>
        class TreeSet;

        /*
         * Comparator used privately by std::set.
         */
        template <typename E>
        class TreeSetElemLess final {
        private:
            jxx::Ptr<ComparatorSuper<E>> comparator_;

        public:
            TreeSetElemLess()
                : comparator_(nullptr) {}

            explicit TreeSetElemLess(
                jxx::Ptr<ComparatorSuper<E>> comparator)
                : comparator_(comparator) {}

            bool operator()(
                const jxx::Ptr<E>& left,
                const jxx::Ptr<E>& right) const {

                if (left == nullptr || right == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                if (comparator_ != nullptr) {
                    return comparator_->compare(left, right) < 0;
                }

                auto comparable =
                    jxx::CAST<jxx::lang::Comparable<E>>(left);

                if (comparable == nullptr) {
                    throw jxx::lang::ClassCastException();
                }

                return comparable->compareTo(right) < 0;
            }
        };

        /*
         * Forward-declared external TreeSet iterator.
         */
        template <typename E>
        class TreeSetIterator final
            : public virtual Iterator<E> {
        private:
            TreeSet<E>* owner_;
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
            explicit TreeSetIterator(TreeSet<E>* owner)
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

            virtual ~TreeSetIterator() = default;

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
        class TreeSet
            : public virtual AbstractSet<E>
            , public virtual jxx::lang::Cloneable
            , public virtual jxx::io::SerializableI {
        private:
            template <typename T>
            friend class TreeSetIterator;

            using InternalSet =
                std::set<
                jxx::Ptr<E>,
                TreeSetElemLess<E>>;

            InternalSet set_;
            jxx::Ptr<ComparatorSuper<E>> comparator_;
            jxx::lang::jint modCount_;

        private:
            static jxx::Ptr<E> castObjectToElement(
                jxx::Ptr<jxx::lang::Object> object) {

                return jxx::CAST<E>(object);
            }

        public:
            TreeSet()
                : set_(TreeSetElemLess<E>())
                , comparator_(nullptr)
                , modCount_(0) {}

            explicit TreeSet(
                jxx::Ptr<ComparatorSuper<E>> comparator)
                : set_(TreeSetElemLess<E>(comparator))
                , comparator_(comparator)
                , modCount_(0) {}

            explicit TreeSet(
                jxx::Ptr<wildcard::CollectionExtends<E>>
                collection)
                : TreeSet() {

                if (collection == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                addAll(collection);
            }

            virtual ~TreeSet() = default;

            /*
             * Required by your pure-virtual Serializable interface.
             */
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

            virtual jxx::lang::jint size() override {
                return static_cast<jxx::lang::jint>(
                    set_.size());
            }

            virtual jxx::lang::jbool isEmpty() override {
                return static_cast<jxx::lang::jbool>(
                    set_.empty());
            }

            virtual jxx::Ptr<ComparatorSuper<E>>
                comparator() {

                return comparator_;
            }

            virtual jxx::lang::jbool contains(
                jxx::Ptr<jxx::lang::Object> object)
                override {

                if (object == nullptr) {
                    throw jxx::lang::NullPointerException();
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

            virtual jxx::Ptr<Iterator<E>>
                iterator() override {

                return jxx::Ptr<Iterator<E>>(
                    new TreeSetIterator<E>(this));
            }

            virtual jxx::lang::jbool add(
                jxx::Ptr<E> element) override {

                if (element == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                const auto result =
                    set_.insert(element);

                if (!result.second) {
                    return static_cast<jxx::lang::jbool>(
                        false);
                }

                ++modCount_;

                return static_cast<jxx::lang::jbool>(
                    true);
            }

            virtual jxx::lang::jbool remove(
                jxx::Ptr<jxx::lang::Object> object)
                override {

                if (object == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                auto element =
                    castObjectToElement(object);

                if (element == nullptr) {
                    return static_cast<jxx::lang::jbool>(
                        false);
                }

                auto found = set_.find(element);

                if (found == set_.end()) {
                    return static_cast<jxx::lang::jbool>(
                        false);
                }

                set_.erase(found);
                ++modCount_;

                return static_cast<jxx::lang::jbool>(
                    true);
            }

            virtual void clear() override {
                if (set_.empty()) {
                    return;
                }

                set_.clear();
                ++modCount_;
            }

            virtual jxx::lang::jbool containsAll(
                jxx::Ptr<wildcard::CollectionAny>
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

                return static_cast<jxx::lang::jbool>(
                    true);
            }

            virtual jxx::lang::jbool addAll(
                jxx::Ptr<wildcard::CollectionExtends<E>>
                collection) override {

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

            virtual jxx::lang::jbool removeAll(
                jxx::Ptr<wildcard::CollectionAny>
                collection) override {

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

            virtual jxx::lang::jbool retainAll(
                jxx::Ptr<wildcard::CollectionAny>
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

            virtual jxx::Ptr<E> first() {
                if (set_.empty()) {
                    throw jxx::util::NoSuchElementException();
                }

                return *set_.begin();
            }

            virtual jxx::Ptr<E> last() {
                if (set_.empty()) {
                    throw jxx::util::NoSuchElementException();
                }

                auto iterator = set_.end();
                --iterator;

                return *iterator;
            }

            virtual jxx::Ptr<E> lower(
                jxx::Ptr<E> element) {

                if (element == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                auto iterator =
                    set_.lower_bound(element);

                if (iterator == set_.begin()) {
                    return nullptr;
                }

                --iterator;
                return *iterator;
            }

            virtual jxx::Ptr<E> floor(
                jxx::Ptr<E> element) {

                if (element == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                auto iterator =
                    set_.upper_bound(element);

                if (iterator == set_.begin()) {
                    return nullptr;
                }

                --iterator;
                return *iterator;
            }

            virtual jxx::Ptr<E> ceiling(
                jxx::Ptr<E> element) {

                if (element == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                auto iterator =
                    set_.lower_bound(element);

                return iterator == set_.end()
                    ? nullptr
                    : *iterator;
            }

            virtual jxx::Ptr<E> higher(
                jxx::Ptr<E> element) {

                if (element == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                auto iterator =
                    set_.upper_bound(element);

                return iterator == set_.end()
                    ? nullptr
                    : *iterator;
            }

            virtual jxx::Ptr<E> pollFirst() {
                if (set_.empty()) {
                    return nullptr;
                }

                auto iterator = set_.begin();
                auto value = *iterator;

                set_.erase(iterator);
                ++modCount_;

                return value;
            }

            virtual jxx::Ptr<E> pollLast() {
                if (set_.empty()) {
                    return nullptr;
                }

                auto iterator = set_.end();
                --iterator;

                auto value = *iterator;

                set_.erase(iterator);
                ++modCount_;

                return value;
            }

            virtual jxx::Ptr<jxx::lang::Object>
                clone() {

                auto cloned =
                    jxx::Ptr<TreeSet<E>>(
                        new TreeSet<E>(comparator_));

                for (const auto& element : set_) {
                    cloned->set_.insert(element);
                }

                cloned->modCount_ = 0;

                return jxx::CAST<jxx::lang::Object>(
                    cloned);
            }
        };

    } // namespace util
} // namespace jxx
