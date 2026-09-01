#pragma once

#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"

#include "util/jxx.util.AbstractCollection.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/jxx.util.Queue.h"
#include "util/jxx.util.wildcard.CollectionExtends.h"

namespace jxx {
namespace util {

/**
 * JXX/C++17 implementation of java.util.AbstractQueue<E>.
 *
 * Java 8 hierarchy:
 *
 *   Object
 *     |
 *   AbstractCollection<E>
 *     |
 *   AbstractQueue<E>
 *
 * Implements:
 *   Queue<E>
 *
 * A concrete subclass must minimally implement:
 *
 *   offer(E)
 *   poll()
 *   peek()
 *   size()
 *   iterator()
 *
 * The subclass's offer(E) implementation must reject nullptr
 * when the queue does not permit null elements.
 */
template <typename E>
class AbstractQueue
    : public virtual AbstractCollection<E>
    , public virtual Queue<E> {
protected:
    /**
     * Java:
     *
     * protected AbstractQueue()
     */
    AbstractQueue() = default;

public:
    virtual ~AbstractQueue() = default;

    /**
     * Java:
     *
     * public boolean add(E e)
     *
     * Inserts the element using offer(). If offer() returns false,
     * add() throws IllegalStateException.
     *
     * Null validation is delegated to offer(), matching Java's
     * AbstractQueue behavior.
     */
    virtual jxx::lang::jbool add(const jxx::Ptr<E>& element) override {

        if (this->offer(element)) {
            return static_cast<jxx::lang::jbool>(true);
        }

        throw jxx::lang::IllegalStateException();
    }

    /**
     * Java:
     *
     * public E remove()
     *
     * Retrieves and removes the head. If poll() returns null,
     * the queue is empty and NoSuchElementException is thrown.
     */
    virtual jxx::Ptr<E> remove() override {
        auto element = this->poll();

        if (element == nullptr) {
            throw jxx::util::NoSuchElementException();
        }

        return element;
    }

    /**
     * Java:
     *
     * public E element()
     *
     * Retrieves without removing the head. If peek() returns null,
     * the queue is empty and NoSuchElementException is thrown.
     */
    virtual jxx::Ptr<E> element() override {
        auto element = this->peek();

        if (element == nullptr) {
            throw jxx::util::NoSuchElementException();
        }

        return element;
    }

    /**
     * Java:
     *
     * public void clear()
     *
     * Repeatedly calls poll() until the queue is empty.
     */
    virtual void clear() override {
        while (this->poll() != nullptr) {
            // Continue until poll() indicates an empty queue.
        }
    }

    /**
     * Java:
     *
     * public boolean addAll(Collection<? extends E> c)
     *
     * Adds every element using add(), not offer(). This means a
     * capacity failure is reported through IllegalStateException.
     *
     * Throws:
     *   NullPointerException if collection is null
     *   IllegalArgumentException if collection is this queue
     *
     * The operation is not atomic. If an exception occurs after some
     * elements have been added, those preceding elements remain added,
     * matching Java's AbstractQueue contract.
     */
    virtual jxx::lang::jbool addAll(
        jxx::Ptr<wildcard::CollectionExtends<E>>
            collection) override {

        if (collection == nullptr) {
            throw jxx::lang::NullPointerException();
        }

        if (isSameCollection_(collection)) {
            throw jxx::lang::IllegalArgumentException();
        }

        jxx::lang::jbool modified =
            static_cast<jxx::lang::jbool>(false);

        auto iterator =
            collection->iteratorExtends();

        while (iterator->hasNext()) {
            /*
             * Use add(), not offer().
             *
             * Java AbstractQueue.addAll delegates each element to
             * add(), so capacity failure throws IllegalStateException.
             */
            if (this->add(iterator->next())) {
                modified =
                    static_cast<jxx::lang::jbool>(true);
            }
        }

        return modified;
    }

    /*
     * The following methods remain abstract through Queue<E> and
     * AbstractCollection<E>. Concrete queue classes must implement them.
     */

    virtual jxx::lang::jbool offer(
        jxx::Ptr<E> element) override = 0;

    virtual jxx::Ptr<E> poll() override = 0;

    virtual jxx::Ptr<E> peek() override = 0;

    virtual jxx::lang::jint size() override = 0;

    virtual jxx::Ptr<Iterator<E>>
    iterator() override = 0;

private:
    /**
     * Detect AbstractQueue.addAll(this).
     *
     * The wildcard adapter is expected to derive from Object. The
     * comparison is performed through the shared Object identity.
     */
    jxx::lang::jbool isSameCollection_(
        jxx::Ptr<wildcard::CollectionExtends<E>>
            collection) const {

        auto collectionObject =
            jxx::CAST<jxx::lang::Object>(
                collection);

        auto thisObject =
            static_cast<const jxx::lang::Object*>(
                this);

        return static_cast<jxx::lang::jbool>(
            collectionObject != nullptr &&
            collectionObject.get() == thisObject);
    }
};

} // namespace util
} // namespace jxx