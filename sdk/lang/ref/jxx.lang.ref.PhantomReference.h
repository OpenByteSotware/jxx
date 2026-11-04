#pragma once

#include "lang/ref/jxx.lang.ref.Reference.h"

namespace jxx::lang::ref {

template <typename T>
class PhantomReference final
    : public ::jxx::lang::ClassBase<PhantomReference<T>, Reference<T>> {
public:
    using JxxSuper = Reference<T>;
    using Super = ::jxx::lang::ClassBase<PhantomReference<T>, JxxSuper>;

    PhantomReference(const ::jxx::Ptr<T>& referent,
        const ::jxx::Ptr<ReferenceQueue<T>>& queue)
        : Super(referent, queue, JxxSuper::Strength::Phantom) {
    }

    ::jxx::Ptr<T> get() const override {
        return nullptr;
    }
};

} // namespace jxx::lang::ref
