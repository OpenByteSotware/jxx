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
    virtual jxx::lang::jint size() = 0;
    virtual jxx::lang::jbool isEmpty() { return size() == 0; }
    virtual jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> o) = 0;
    virtual jxx::Ptr<Iterator<E>> iterator() override = 0;
    virtual jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>> toArray() = 0;

    template <typename T>
    jxx::Ptr<JxxArray<T, 1U>> toArray(jxx::Ptr<JxxArray<T, 1U>> a) {
        const jxx::lang::jint sz = size();
        jxx::Ptr<JxxArray<T, 1U>> result = a;
        if (result == nullptr || result->length() < sz) {
            result = jxx::Ptr<JxxArray<T, 1U>>(jxx::NEW<JxxArray<T, 1U>>(sz));
        }
        auto it = iterator();
        jxx::lang::jint i = 0;
        while (it->hasNext()) {
            result->set(i++, static_cast<T>(it->next()));
        }
        if (result->length() > sz) {
            result->set(sz, T());
        }
        return result;
    }

    virtual jxx::lang::jbool add(jxx::Ptr<E> e) = 0;
    virtual jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> o) = 0;
    virtual jxx::lang::jbool containsAll(jxx::Ptr<wildcard::CollectionAny> c) = 0;
    virtual jxx::lang::jbool addAll(jxx::Ptr<wildcard::CollectionExtends<E>> c) = 0;
    virtual jxx::lang::jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) = 0;
    virtual jxx::lang::jbool retainAll(jxx::Ptr<wildcard::CollectionAny> c) = 0;
    virtual void clear() = 0;

    virtual jxx::lang::jbool removeIf(jxx::Ptr<function::PredicateSuper<E>> /*filter*/) {
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
