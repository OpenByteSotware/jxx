#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "util/jxx.util.SortedSet.h"

namespace jxx::util {

template <typename E>
class NavigableSet
    : public ::jxx::lang::InterfaceBase<NavigableSet<E>, SortedSet<E>> {
public:
    ~NavigableSet() override = default;

    virtual ::jxx::Ptr<E> lower(const ::jxx::Ptr<E>& element) const = 0;
    virtual ::jxx::Ptr<E> floor(const ::jxx::Ptr<E>& element) const = 0;
    virtual ::jxx::Ptr<E> ceiling(const ::jxx::Ptr<E>& element) const = 0;
    virtual ::jxx::Ptr<E> higher(const ::jxx::Ptr<E>& element) const = 0;
    virtual ::jxx::Ptr<E> pollFirst() = 0;
    virtual ::jxx::Ptr<E> pollLast() = 0;
    virtual ::jxx::Ptr<Iterator<E>> descendingIterator() = 0;
    virtual ::jxx::Ptr<NavigableSet<E>> descendingSet() const = 0;
    virtual ::jxx::Ptr<NavigableSet<E>> subSet(
        const ::jxx::Ptr<E>& fromElement,
        ::jxx::lang::jbool fromInclusive,
        const ::jxx::Ptr<E>& toElement,
        ::jxx::lang::jbool toInclusive) const = 0;
    virtual ::jxx::Ptr<NavigableSet<E>> headSet(
        const ::jxx::Ptr<E>& toElement,
        ::jxx::lang::jbool inclusive) const = 0;
    virtual ::jxx::Ptr<NavigableSet<E>> tailSet(
        const ::jxx::Ptr<E>& fromElement,
        ::jxx::lang::jbool inclusive) const = 0;
};

} // namespace jxx::util
