#pragma once

#include "lang/jxx.lang.Iterable.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.wildcard.CollectionAny.h"
#include "util/jxx.util.wildcard.CollectionExtends.h"
#include "util/function/jxx.util.function.PredicateSuper.h"

namespace jxx {
namespace util {

template <typename E> class Spliterator;
namespace stream { template <typename T> class Stream; }

template <typename E>
class Collection : virtual public jxx::lang::Iterable<E> {
public:
    virtual ~Collection() = default;
    virtual jint size() = 0;
    virtual jbool isEmpty() { return size() == 0; }
    virtual jbool contains(jxx::Ptr<jxx::lang::Object> o) = 0;
    virtual jxx::Ptr<Iterator<E>> iterator() override = 0;
    virtual jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>>> toArray() = 0;

    template <typename T>
    jxx::Ptr<JxxArray<T>> toArray(jxx::Ptr<JxxArray<T>> a) {
        const jint sz = size();
        jxx::Ptr<JxxArray<T>> result = a;
        if (result == nullptr || result->length() < sz) {
            result = jxx::Ptr<JxxArray<T>>(new JxxArray<T>(sz));
        }
        auto it = iterator();
        jint i = 0;
        while (it->hasNext()) {
            result->set(i++, static_cast<T>(it->next()));
        }
        if (result->length() > sz) {
            result->set(sz, T());
        }
        return result;
    }

    virtual jbool add(jxx::Ptr<E> e) = 0;
    virtual jbool remove(jxx::Ptr<jxx::lang::Object> o) = 0;
    virtual jbool containsAll(jxx::Ptr<wildcard::CollectionAny> c) = 0;
    virtual jbool addAll(jxx::Ptr<wildcard::CollectionExtends<E>> c) = 0;
    virtual jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) = 0;
    virtual jbool retainAll(jxx::Ptr<wildcard::CollectionAny> c) = 0;
    virtual void clear() = 0;

    virtual jbool removeIf(jxx::Ptr<function::PredicateSuper<E>> /*filter*/) {
        throw UnsupportedOperationException();
    }
    virtual jxx::Ptr<Spliterator<E>> spliterator() override {
        throw UnsupportedOperationException();
    }
    virtual jxx::Ptr<stream::Stream<E>> stream() {
        throw UnsupportedOperationException();
    }
    virtual jxx::Ptr<stream::Stream<E>> parallelStream() {
        throw UnsupportedOperationException();
    }
};

} // namespace util
} // namespace jxx
