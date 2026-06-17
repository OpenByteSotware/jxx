#pragma once

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx {
namespace util {
namespace function {

template <typename T, typename U>
class BiConsumer {
public:
    virtual ~BiConsumer() = default;
    virtual void accept(jxx::Ptr<T> t, jxx::Ptr<U> u) = 0;

    virtual jxx::Ptr<BiConsumer<T, U>> andThen(jxx::Ptr<BiConsumer<T, U>> after) {
        if (after == nullptr) throw jxx::lang::NullPointerException();

        class AndThenBiConsumer : public virtual BiConsumer<T, U> {
        private:
            jxx::Ptr<BiConsumer<T, U>> first_;
            jxx::Ptr<BiConsumer<T, U>> second_;
        public:
            AndThenBiConsumer(jxx::Ptr<BiConsumer<T, U>> first, jxx::Ptr<BiConsumer<T, U>> second)
                : first_(first), second_(second) {}
            virtual ~AndThenBiConsumer() = default;
            virtual void accept(jxx::Ptr<T> t, jxx::Ptr<U> u) override {
                first_->accept(t, u);
                second_->accept(t, u);
            }
        };

        return jxx::Ptr<BiConsumer<T, U>>(new AndThenBiConsumer(jxx::Ptr<BiConsumer<T, U>>(this), after));
    }
};

} // namespace function
} // namespace util
} // namespace jxx
