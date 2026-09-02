#pragma once
#include "util/jxx.util.Iterator.h"
namespace jxx::util {
template <typename E>
class ListIterator : public virtual Iterator<E> {
public:
    virtual ~ListIterator() = default;
    virtual jxx::lang::jbool hasPrevious() = 0;
    virtual jxx::Ptr<E> previous() = 0;
    virtual jxx::lang::jint nextIndex() = 0;
    virtual jxx::lang::jint previousIndex() = 0;
    virtual void set(const jxx::Ptr<E>& element) = 0;
    virtual void add(const jxx::Ptr<E>& element) = 0;
};
} // namespace jxx::util
