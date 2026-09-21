#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "util/function/jxx.util.function.Consumer.h"

namespace jxx::util {
template <typename E>
class Iterator : public jxx::lang::InterfaceBase<Iterator<E>> {
public:
    virtual ~Iterator() = default;
    virtual jxx::lang::jbool hasNext() = 0;
    virtual jxx::Ptr<E> next() = 0;

    virtual void remove() {
        throw jxx::lang::UnsupportedOperationException();
    }

    virtual void forEachRemaining(
        const jxx::Ptr<jxx::util::function::Consumer<E>>& action) {
        if (action == nullptr) throw jxx::lang::NullPointerException();
        while (hasNext()) action->accept(next());
    }
};
} // namespace jxx::util
