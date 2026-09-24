#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "util/jxx.util.Comparator.h"
#include "util/jxx.util.Set.h"

namespace jxx::util {

template <typename E>
class SortedSet
    : public ::jxx::lang::InterfaceBase<SortedSet<E>, Set<E>> {
public:
    ~SortedSet() override = default;

    virtual ::jxx::Ptr<Comparator<E>> comparator() const = 0;
    virtual ::jxx::Ptr<E> first() const = 0;
    virtual ::jxx::Ptr<E> last() const = 0;
    virtual ::jxx::Ptr<SortedSet<E>> subSet(
        const ::jxx::Ptr<E>& fromElement,
        const ::jxx::Ptr<E>& toElement) const = 0;
    virtual ::jxx::Ptr<SortedSet<E>> headSet(
        const ::jxx::Ptr<E>& toElement) const = 0;
    virtual ::jxx::Ptr<SortedSet<E>> tailSet(
        const ::jxx::Ptr<E>& fromElement) const = 0;
};

} // namespace jxx::util
