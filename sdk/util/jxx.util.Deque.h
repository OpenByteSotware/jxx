#pragma once

#include "util/jxx.util.Queue.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Iterator.h"

namespace jxx { template <typename T> class Ptr; }

namespace jxx {
namespace util {

template <typename E>
class Deque : virtual public Queue<E> {
public:
    virtual ~Deque() = default;

    virtual void addFirst(jxx::Ptr<E> e) = 0;
    virtual void addLast(jxx::Ptr<E> e) = 0;
    virtual jxx::lang::jbool offerFirst(jxx::Ptr<E> e) = 0;
    virtual jxx::lang::jbool offerLast(jxx::Ptr<E> e) = 0;
    virtual jxx::Ptr<E> removeFirst() = 0;
    virtual jxx::Ptr<E> removeLast() = 0;
    virtual jxx::Ptr<E> pollFirst() = 0;
    virtual jxx::Ptr<E> pollLast() = 0;
    virtual jxx::Ptr<E> getFirst() = 0;
    virtual jxx::Ptr<E> getLast() = 0;
    virtual jxx::Ptr<E> peekFirst() = 0;
    virtual jxx::Ptr<E> peekLast() = 0;
    virtual jxx::lang::jbool removeFirstOccurrence(jxx::Ptr<jxx::lang::Object> o) = 0;
    virtual jxx::lang::jbool removeLastOccurrence(jxx::Ptr<jxx::lang::Object> o) = 0;

    // Queue bridge methods
    virtual jxx::lang::jbool offer(jxx::Ptr<E> e) override { return offerLast(e); }
    virtual jxx::Ptr<E> remove() override { return removeFirst(); }
    virtual jxx::Ptr<E> poll() override { return pollFirst(); }
    virtual jxx::Ptr<E> element() override { return getFirst(); }
    virtual jxx::Ptr<E> peek() override { return peekFirst(); }

    virtual void push(jxx::Ptr<E> e) { addFirst(e); }
    virtual jxx::Ptr<E> pop() { return removeFirst(); }

    virtual jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> o) override {
        return removeFirstOccurrence(o);
    }

    virtual jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> o) override = 0;
    virtual jxx::Ptr<Iterator<E>> iterator() override = 0;
    virtual jxx::Ptr<Iterator<E>> descendingIterator() = 0;
};

} // namespace util
} // namespace jxx
