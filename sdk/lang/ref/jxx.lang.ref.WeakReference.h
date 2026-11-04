#pragma once

#include "lang/ref/jxx.lang.ref.Reference.h"

namespace jxx::lang::ref {

template <typename T>
class WeakReference final
    : public ::jxx::lang::ClassBase<WeakReference<T>, Reference<T>> {
public:
    using JxxSuper = Reference<T>;
    using Super = ::jxx::lang::ClassBase<WeakReference<T>, JxxSuper>;

    explicit WeakReference(const ::jxx::Ptr<T>& referent)
        : Super(referent, nullptr, JxxSuper::Strength::Weak) {
    }

    WeakReference(const ::jxx::Ptr<T>& referent,
        const ::jxx::Ptr<ReferenceQueue<T>>& queue)
        : Super(referent, queue, JxxSuper::Strength::Weak) {
    }
};

} // namespace jxx::lang::ref
