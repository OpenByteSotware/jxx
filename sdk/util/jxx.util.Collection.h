#pragma once

#include "jxx_types.h"
#include "jxx.util.Iterable.h"

namespace jxx::util {

template <class E>
struct Collection : public virtual Iterable<E> {
    virtual ~Collection() = default;

    virtual jint size() const = 0;
    virtual jbool isEmpty() const = 0;
    virtual jbool contains(const E& e) const = 0;

    virtual jxx::Ptr<Iterator<E>> iterator() = 0;

    virtual jbool add(const E& e) = 0;
    virtual jbool remove(const E& e) = 0;
    virtual void clear() = 0;

    virtual jbool addAll(jxx::Ptr<Collection<E>> c) = 0;
    virtual jbool containsAll(jxx::Ptr<Collection<E>> c) const = 0;
    virtual jbool removeAll(jxx::Ptr<Collection<E>> c) = 0;
    virtual jbool retainAll(jxx::Ptr<Collection<E>> c) = 0;
};

} // namespace jxx::util
