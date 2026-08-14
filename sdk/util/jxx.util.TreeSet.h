#pragma once

#include <cstddef>
#include <memory>
#include <set>
#include <vector>

#include "io/jxx.io.Serializable.h"

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

        template <typename E>
        class TreeSet
            : public virtual AbstractSet<E>
            , public virtual jxx::lang::Cloneable
            , public virtual jxx::io::Serializable {
        private:
            struct ElemLess {
                jxx::Ptr<ComparatorSuper<E>> comparator;

                bool operator()(
                    const jxx::Ptr<E>& left,
                    const jxx::Ptr<E>& right) const {

                    if (left == nullptr || right == nullptr) {
                        throw jxx::lang::NullPointerException();
                    }

                    if (comparator != nullptr) {
                        return comparator->compare(left, right) < 0;
                    }

                    auto comparable =
                        jxx::CAST<jxx::lang::Comparable<E>>(left);

                    if (comparable == nullptr) {
                        throw jxx::lang::ClassCastException();
                    }

                    return comparable->compareTo(right) < 0;
                }
            };

            using InternalSet =
                std::set<jxx::Ptr<E>, ElemLess>;

            InternalSet set_;
            jxx::Ptr<ComparatorSuper<E>> comparator_;
            jxx::lang::jint modCount_;

            static jxx::Ptr<E> castObjectToElement(
                jxx::Ptr<jxx::lang::Object> object) {

                return jxx::CAST<E>(object);
            }

            jxx::lang::jint compareElements(
                jxx::Ptr<E> left,
                jxx::Ptr<E> right) const {

                if (left == nullptr || right == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                if (comparator_ != nullptr) {
                    return comparator_->compare(left, right);
                }

                auto comparable =
                    jxx::CAST<jxx::lang::Comparable<E>>(left);

                if (comparable == nullptr) {
                    throw jxx::lang::ClassCastException();
                }

                return comparable->compareTo(right);
            }

        public:
            class TreeSetIterator final
                : public virtual Iterator<E> {
            private:
                TreeSet<E>* owner_;

                /*
                 * Snapshot references are internal only. This prevents the iterator
                 * from retaining std::set iterators across structural mutations.
                 */
                std::vector<jxx::Ptr<E>> snapshot_;
                std::size_t cursor_;

                jxx::Ptr<E> lastReturned_;
                jxx::lang::jbool canRemove_;
                jxx::lang::jint expectedModCount_;

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
