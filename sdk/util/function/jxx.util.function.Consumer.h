#pragma once

#include "lang/jxx.lang.Object.h"
#include "util/function/jxx.util.function.ConsumerSuper.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx {
namespace util {
namespace function {

template <typename T>
class Consumer : public virtual ConsumerSuper<T> {
public:
    virtual ~Consumer() = default;
    
    virtual void acceptSuper(jxx::Ptr<T> value) {
		(void)value; // Suppress unused parameter warning
    }
    virtual void accept(jxx::Ptr<T> value) {
        acceptSuper(value);
    }

    virtual jxx::Ptr<Consumer<T>> andThen(jxx::Ptr<ConsumerSuper<T>> after) {
        if (after == nullptr) {
            throw jxx::lang::NullPointerException();
        }

  
        class AndThenConsumer : public virtual Consumer<T> {
        private:
            jxx::Ptr<ConsumerSuper<T>> first_;
            jxx::Ptr<ConsumerSuper<T>> second_;
        public:
            AndThenConsumer(jxx::Ptr<ConsumerSuper<T>> first, jxx::Ptr<ConsumerSuper<T>> second)
                : first_(first), second_(second) {}
            virtual ~AndThenConsumer() = default;
            virtual void acceptSuper(jxx::Ptr<T> value) override {
                first_->acceptSuper(value);
                second_->acceptSuper(value);
            }
            virtual void accept(jxx::Ptr<T> value) {
                acceptSuper(value);
            }
        };

        return jxx::Ptr<Consumer<T>>(new AndThenConsumer(jxx::Ptr<ConsumerSuper<T>>(this), after));
    }
};

} // namespace function
} // namespace util
} // namespace jxx
