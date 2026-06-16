#pragma once

#include "util/function/jxx.util.function.ConsumerSuper.h"

namespace jxx {
    template <typename T> class Ptr;
}

namespace jxx {
    namespace util {
        namespace function {

            template <typename T>
            class Consumer : public virtual ConsumerSuper<T> {
            public:
                virtual ~Consumer() = default;

                // Inherits:
                //   virtual void accept(Ptr<T> value) = 0;

                // Java 8 default method:
                //   Consumer<T> andThen(Consumer<? super T> after)
                virtual jxx::Ptr<Consumer<T>> andThen(jxx::Ptr<ConsumerSuper<T>> const after) {
                    if (after == nullptr) {
                        throw NullPointerException();
                    }

                    class AndThenConsumer : public virtual Consumer<T> {
                    private:
                        jxx::Ptr<ConsumerSuper<T>> first_;
                        jxx::Ptr<ConsumerSuper<T>> second_;

                    public:
                        AndThenConsumer(
                            jxx::Ptr<ConsumerSuper<T>> first,
                            jxx::Ptr<ConsumerSuper<T>> second)
                            : first_(first), second_(second) {}

                        virtual ~AndThenConsumer() = default;

                        virtual void accept(jxx::Ptr<T> value) override {
                            first_->accept(value);
                            second_->accept(value);
                        }
                    };

                    return jxx::Ptr<Consumer<T>>(
                        new AndThenConsumer(
                            jxx::runtime::SelfRef<ConsumerSuper<T>, Consumer<T>>::get(this), after));
                }
            };

        } // namespace function
    } // namespace util
} // namespace jxx