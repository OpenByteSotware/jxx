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
    virtual jxx::lang::jint size() = 0;

    virtual jxx::lang::jbool isEmpty() override {
        return size() == 0;
    }

    virtual jxx::lang::jbool contains(const jxx::Ptr<jxx::lang::Object> o) = 0;
    virtual jxx::Ptr<ListIterator<E>> listIterator() = 0;
    virtual jxx::Ptr<ListIterator<E>> listIterator(jxx::lang::jint index) = 0;
    virtual jxx::Ptr<Iterator<E>> iterator() = 0;
    virtual jxx::lang::ObjectArray toArray() = 0;
    virtual jxx::lang::jbool add(const jxx::Ptr<E> e) = 0;
    virtual jxx::lang::jbool remove(const jxx::Ptr<jxx::lang::Object>& o) = 0;
    virtual jxx::lang::jbool containsAll(const jxx::Ptr<wildcard::CollectionAny> c) = 0;
    virtual jxx::lang::jbool addAll(const jxx::Ptr<wildcard::CollectionExtends<E>> c) = 0;
    virtual jxx::lang::jbool removeAll(const jxx::Ptr<wildcard::CollectionAny> c) = 0;
    virtual jxx::lang::jbool retainAll(const jxx::Ptr<wildcard::CollectionAny> c) = 0;
    virtual void clear() = 0;

    // ===== List-specific methods =====
    virtual jxx::Ptr<E> get(jxx::lang::jint index) = 0;
    virtual jxx::Ptr<E> set(jxx::lang::jint index, jxx::Ptr<E> element) = 0;
    virtual void add(jxx::lang::jint index, jxx::Ptr<E> element) = 0;
    virtual jxx::Ptr<E> remove(jxx::lang::jint index) = 0;

    // Java: int indexOf(Object o), lastIndexOf(Object o)
    virtual jxx::lang::jint indexOf(const jxx::Ptr<jxx::lang::Object> o) = 0;
    virtual jxx::lang::jint lastIndexOf(const jxx::Ptr<jxx::lang::Object> o) = 0;
    // Java: boolean addAll(int, Collection<? extends E>)
    virtual jxx::lang::jbool addAll(jxx::lang::jint index, jxx::Ptr<wildcard::CollectionExtends<E>> c) = 0;

    virtual jxx::Ptr<List<E>> subList(jxx::lang::jint fromIndex, jxx::lang::jint toIndex) = 0;

    // Java 8 default methods
    virtual void replaceAll(const jxx::Ptr<function::UnaryOperator<E>> /*op*/) = 0;
        
    // Java: sort(Comparator<? super E>)
    virtual void sort(const jxx::Ptr<ComparatorSuper<E>> /*c*/) = 0;
    
    virtual jxx::Ptr<Spliterator<E>> spliterator() = 0;

    // Java List contract redeclarations
    virtual jxx::lang::jbool equals(const jxx::Ptr<jxx::lang::Object>& o) = 0;
    virtual jxx::lang::jint hashCode() = 0;
};

} // namespace util
} // namespace jxx
