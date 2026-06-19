#pragma once

#include "lang/jxx.lang.UnsupportedOperationException.h"
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
class List : public virtual Collection<E> {
public:
    virtual ~List() = default;

    // ===== Collection methods redeclared for Java 8 List parity =====
    virtual jxx::lang::jint size() override = 0;

    virtual jxx::lang::jbool isEmpty() override {
        return size() == 0;
    }

    virtual jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> o) override = 0;
    virtual jxx::Ptr<ListIterator<E>> listIterator() = 0;
    virtual jxx::Ptr<ListIterator<E>> listIterator(jxx::lang::jint index) = 0;
    virtual jxx::Ptr<Iterator<E>> iterator() override = 0;
    virtual jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>> toArray() override = 0;
    virtual jxx::lang::jbool add(jxx::Ptr<E> e) override = 0;
    virtual jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> o) override = 0;
    virtual jxx::lang::jbool containsAll(jxx::Ptr<wildcard::CollectionAny> c) override = 0;
    virtual jxx::lang::jbool addAll(jxx::Ptr<wildcard::CollectionExtends<E>> c) override = 0;
    virtual jxx::lang::jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) override = 0;
    virtual jxx::lang::jbool retainAll(jxx::Ptr<wildcard::CollectionAny> c) override = 0;
    virtual void clear() override = 0;

    // ===== List-specific methods =====
    virtual jxx::Ptr<E> get(jxx::lang::jint index) = 0;
    virtual jxx::Ptr<E> set(jxx::lang::jint index, jxx::Ptr<E> element) = 0;
    virtual void add(jxx::lang::jint index, jxx::Ptr<E> element) = 0;
    virtual jxx::Ptr<E> remove(jxx::lang::jint index) = 0;

    // Java: int indexOf(Object o), lastIndexOf(Object o)
    virtual jxx::lang::jint indexOf(jxx::Ptr<jxx::lang::Object> o) = 0;
    virtual jxx::lang::jint lastIndexOf(jxx::Ptr<jxx::lang::Object> o) = 0;
    // Java: boolean addAll(int, Collection<? extends E>)
    virtual jxx::lang::jbool addAll(jxx::lang::jint index, jxx::Ptr<wildcard::CollectionExtends<E>> c) = 0;

    virtual jxx::Ptr<List<E>> subList(jxx::lang::jint fromIndex, jxx::lang::jint toIndex) = 0;

    // Java 8 default methods
    virtual void replaceAll(jxx::Ptr<function::UnaryOperator<E>> /*op*/) {
        throw jxx::lang::UnsupportedOperationException();
    }

    // Java: sort(Comparator<? super E>)
    virtual void sort(jxx::Ptr<ComparatorSuper<E>> /*c*/) {
        throw jxx::lang::UnsupportedOperationException();
    }

    virtual jxx::Ptr<Spliterator<E>> spliterator() override {
        throw jxx::lang::UnsupportedOperationException();
    }

    // Java List contract redeclarations
    virtual jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> o) override = 0;
    virtual jxx::lang::jint hashCode() override = 0;
};

} // namespace util
} // namespace jxx
