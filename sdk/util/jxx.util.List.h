#pragma once

#include "util/jxx.util.Collection.h"
#include "util/jxx.util.ListIterator.h"
#include "util/jxx.util.ComparatorSuper.h"

namespace jxx {
namespace util {

template <typename E> class Spliterator;

namespace function {
template <typename T> class UnaryOperator;
}

template <typename E>
class List : virtual public Collection<E> {
public:
    virtual ~List() = default;

    // ===== Collection methods redeclared for Java 8 List parity =====
    virtual jint size() override = 0;

    virtual jbool isEmpty() override {
        return size() == 0;
    }

    virtual jbool contains(jxx::Ptr<jxx::lang::Object> o) override = 0;

    virtual jxx::Ptr<ListIterator<E>> listIterator() = 0;
    virtual jxx::Ptr<ListIterator<E>> listIterator(jint index) = 0;
    virtual jxx::Ptr<Iterator<E>> iterator() override = 0;
    virtual jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>>> toArray() override = 0;
    virtual jbool add(jxx::Ptr<E> e) override = 0;
    virtual jbool remove(jxx::Ptr<jxx::lang::Object> o) override = 0;
    virtual jbool containsAll(jxx::Ptr<wildcard::CollectionAny> c) override = 0;
    virtual jbool addAll(jxx::Ptr<wildcard::CollectionExtends<E>> c) override = 0;
    virtual jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) override = 0;
    virtual jbool retainAll(jxx::Ptr<wildcard::CollectionAny> c) override = 0;
    virtual void clear() override = 0;

    // ===== List-specific methods =====
    virtual jxx::Ptr<E> get(jint index) = 0;
    virtual jxx::Ptr<E> set(jint index, jxx::Ptr<E> element) = 0;
    virtual void add(jint index, jxx::Ptr<E> element) = 0;
    virtual jxx::Ptr<E> remove(jint index) = 0;

    // Java: int indexOf(Object o), lastIndexOf(Object o)
    virtual jint indexOf(jxx::Ptr<jxx::lang::Object> o) = 0;
    virtual jint lastIndexOf(jxx::Ptr<jxx::lang::Object> o) = 0;

    // Java: boolean addAll(int, Collection<? extends E>)
    virtual jbool addAll(jint index, jxx::Ptr<wildcard::CollectionExtends<E>> c) = 0;

    virtual jxx::Ptr<List<E>> subList(jint fromIndex, jint toIndex) = 0;

    // Java 8 default methods
    virtual void replaceAll(jxx::Ptr<function::UnaryOperator<E>> /*op*/) {
        throw UnsupportedOperationException();
    }

    // Java: sort(Comparator<? super E>)
    virtual void sort(jxx::Ptr<ComparatorSuper<E>> /*c*/) {
        throw UnsupportedOperationException();
    }

    virtual jxx::Ptr<Spliterator<E>> spliterator() override {
        throw UnsupportedOperationException();
    }

    // Java List contract redeclarations
    virtual jbool equals(jxx::Ptr<jxx::lang::Object> o) override = 0;
    virtual jint hashCode() override = 0;
};

} // namespace util
} // namespace jxx
