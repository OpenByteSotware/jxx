#pragma once
#include "util/jxx.util.Queue.h"
#include "util/jxx.util.Iterator.h"
namespace jxx::util {
template <typename E>
class Deque : public virtual Queue<E> {
public:
    virtual ~Deque() = default;
    virtual void addFirst(const jxx::Ptr<E>& element) = 0;
    virtual void addLast(const jxx::Ptr<E>& element) = 0;
    virtual jxx::lang::jbool offerFirst(const jxx::Ptr<E>& element) = 0;
    virtual jxx::lang::jbool offerLast(const jxx::Ptr<E>& element) = 0;
    virtual jxx::Ptr<E> removeFirst() = 0;
    virtual jxx::Ptr<E> removeLast() = 0;
    virtual jxx::Ptr<E> pollFirst() = 0;
    virtual jxx::Ptr<E> pollLast() = 0;
    virtual jxx::Ptr<E> getFirst() = 0;
    virtual jxx::Ptr<E> getLast() = 0;
    virtual jxx::Ptr<E> peekFirst() = 0;
    virtual jxx::Ptr<E> peekLast() = 0;
    virtual jxx::lang::jbool removeFirstOccurrence(const jxx::Ptr<jxx::lang::Object>& object) = 0;
    virtual jxx::lang::jbool removeLastOccurrence(const jxx::Ptr<jxx::lang::Object>& object) = 0;
    virtual void push(const jxx::Ptr<E>& element) = 0;
    virtual jxx::Ptr<E> pop() = 0;
    virtual jxx::Ptr<Iterator<E>> descendingIterator() = 0;
};
} // namespace jxx::util
