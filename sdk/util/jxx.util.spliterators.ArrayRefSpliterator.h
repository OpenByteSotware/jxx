#pragma once

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"

#include "util/jxx.util.Spliterator.h"

namespace jxx {
    namespace util {
        namespace spliterators {

            template <typename E>
            class ArrayRefSpliterator final
                : public virtual jxx::util::Spliterator<E> {
            private:
                using ElementArray =
                    jxx::lang::JxxArray<jxx::Ptr<E>, 1U>;

                jxx::Ptr<ElementArray> array_;
                jxx::lang::jint index_;
                jxx::lang::jint fence_;
                jxx::lang::jint characteristics_;

                jxx::Ptr<jxx::util::ComparatorSuper<E>>
                    comparator_;

            public:
                ArrayRefSpliterator(
                    jxx::Ptr<ElementArray> array,
                    jxx::lang::jint origin,
                    jxx::lang::jint fence,
                    jxx::lang::jint characteristics,
                    jxx::Ptr<jxx::util::ComparatorSuper<E>>
                    comparator = nullptr)
                    : array_(array)
                    , index_(origin)
                    , fence_(fence)
                    , characteristics_(
                        characteristics |
                        jxx::util::Spliterator<E>::SIZED |
                        jxx::util::Spliterator<E>::SUBSIZED)
                    , comparator_(comparator) {

                    if (array_ == nullptr) {
                        throw jxx::lang::NullPointerException();
                    }

                    const jxx::lang::jint arrayLength =
                        static_cast<jxx::lang::jint>(
                            array_->length);

                    if (origin < 0 ||
                        fence < origin ||
                        fence > arrayLength) {

                        throw jxx::lang::IndexOutOfBoundsException();
                    }
                }

                virtual ~ArrayRefSpliterator() = default;

                virtual jxx::lang::jbool tryAdvance(
                    jxx::Ptr<
                    jxx::util::function::Consumer<E>>
                    action) override {

                    if (action == nullptr) {
                        throw jxx::lang::NullPointerException();
                    }

                    if (index_ >= fence_) {
                        return static_cast<jxx::lang::jbool>(
                            false);
                    }

                    action->accept((*array_)[index_]);
                    ++index_;

                    return static_cast<jxx::lang::jbool>(true);
                }

                virtual void forEachRemaining(
                    jxx::Ptr<
                    jxx::util::function::Consumer<E>>
                    action) override {

                    if (action == nullptr) {
                        throw jxx::lang::NullPointerException();
                    }

                    while (index_ < fence_) {
                        action->accept((*array_)[index_]);
                        ++index_;
                    }
                }

                virtual jxx::Ptr<jxx::util::Spliterator<E>>
                    trySplit() override {

                    const jxx::lang::jint low = index_;

                    const jxx::lang::jint middle =
                        low + ((fence_ - low) >> 1);

                    if (low >= middle) {
                        return nullptr;
                    }

                    index_ = middle;

                    return jxx::NEW<ArrayRefSpliterator<E>>(
                        array_,
                        low,
                        middle,
                        characteristics_,
                        comparator_);
                }

                virtual jxx::lang::jlong estimateSize()
                    override {

                    return static_cast<jxx::lang::jlong>(
                        fence_ - index_);
                }

                virtual jxx::lang::jint characteristics()
                    override {

                    return characteristics_;
                }

                virtual jxx::Ptr<
                    jxx::util::ComparatorSuper<E>>
                    getComparator() override {

                    if (!this->hasCharacteristics(
                        jxx::util::Spliterator<E>::SORTED)) {

                        throw jxx::lang::IllegalStateException();
                    }

                    return comparator_;
                }
            };

        } // namespace spliterators
    } // namespace util
} // namespace jxx