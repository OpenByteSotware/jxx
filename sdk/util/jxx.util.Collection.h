#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Iterable.h"

namespace jxx::util {

template <class E>
struct Collection : public virtual Iterable<E> {
    virtual ~Collection() = default;

    virtual jxx::lang::jint size() const = 0;
    virtual jxx::lang::jbool isEmpty() const = 0;
    virtual jxx::lang::jbool contains(const E& e) const = 0;

    virtual jxx::Ptr<Iterator<E>> iterator() = 0;

    virtual jxx::lang::jbool add(const E& e) = 0;
    virtual jxx::lang::jbool remove(const E& e) = 0;
    virtual void clear() = 0;

    virtual jxx::lang::jbool addAll(jxx::Ptr<Collection<E>> c) = 0;
    virtual jxx::lang::jbool containsAll(jxx::Ptr<Collection<E>> c) const = 0;
    virtual jxx::lang::jbool removeAll(jxx::Ptr<Collection<E>> c) = 0;
    virtual jxx::lang::jbool retainAll(jxx::Ptr<Collection<E>> c) = 0;
};

} // namespace jxx::util
