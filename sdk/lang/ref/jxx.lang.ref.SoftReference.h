#pragma once

#include "lang/ref/jxx.lang.ref.Reference.h"

namespace jxx::lang::ref {

template <typename T>
class SoftReference final
    : public ::jxx::lang::ClassBase<SoftReference<T>, Reference<T>> {
public:
    using JxxSuper = Reference<T>;
    using Super = ::jxx::lang::ClassBase<SoftReference<T>, JxxSuper>;

    explicit SoftReference(const ::jxx::Ptr<T>& referent)
        : Super(referent, nullptr, JxxSuper::Strength::Soft) {
    }

    SoftReference(const ::jxx::Ptr<T>& referent,
        const ::jxx::Ptr<ReferenceQueue<T>>& queue)
        : Super(referent, queue, JxxSuper::Strength::Soft) {
    }
};

} // namespace jxx::lang::ref
