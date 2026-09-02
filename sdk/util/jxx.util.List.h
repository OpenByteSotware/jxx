#pragma once
#include "util/jxx.util.Collection.h"
#include "util/jxx.util.ListIterator.h"
namespace jxx::util {
template <typename E>
class List : public virtual Collection<E> {
public:
    virtual ~List() = default;
    virtual jxx::lang::jbool addAll(jxx::lang::jint index, const jxx::Ptr<wildcard::CollectionExtends<E>>& collection) = 0;
    virtual jxx::Ptr<E> get(jxx::lang::jint index) const = 0;
    virtual jxx::Ptr<E> set(jxx::lang::jint index, const jxx::Ptr<E>& element) = 0;
    virtual void add(jxx::lang::jint index, const jxx::Ptr<E>& element) = 0;
    virtual jxx::Ptr<E> remove(jxx::lang::jint index) = 0;
    virtual jxx::lang::jint indexOf(const jxx::Ptr<jxx::lang::Object>& object) = 0;
    virtual jxx::lang::jint lastIndexOf(const jxx::Ptr<jxx::lang::Object>& object) = 0;
    virtual jxx::Ptr<ListIterator<E>> listIterator() = 0;
    virtual jxx::Ptr<ListIterator<E>> listIterator(jxx::lang::jint index) = 0;
    virtual jxx::Ptr<List<E>> subList(jxx::lang::jint fromIndex, jxx::lang::jint toIndex) = 0;
};
} // namespace jxx::util
