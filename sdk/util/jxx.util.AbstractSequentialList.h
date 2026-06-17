#pragma once

#include <stdexcept>

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.AbstractList.h"
#include "util/jxx.util.Collection.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.ListIterator.h"

namespace jxx::util
{
    /**
     * JXX / C++17 parity implementation for Java 8 java.util.AbstractSequentialList<E>.
     *
     * Design notes:
     * - This class is implemented in terms of listIterator(int), matching the Java design.
     * - Generic element type E is treated as an object type in public APIs: jxx::Ptr<E>.
     * - addAll(int, Collection<? extends E>) is approximated as:
     *     addAll(jint, jxx::Ptr<Collection<jxx::Ptr<E>>>)
     *   because Java wildcard generics do not map directly to C++ templates.
     */
    template<typename E>
    class AbstractSequentialList : public AbstractList<jxx::Ptr<E>>
    {
    protected:
        AbstractSequentialList() = default;

    public:
        ~AbstractSequentialList() override = default;

    public:
        /**
         * Java 8:
         *   public E get(int index)
         */
        jxx::Ptr<E> get(jxx::lang::jint index) const override
        {
            auto it = this->listIterator(index);
            if (!it || !it->hasNext())
                throw std::out_of_range("AbstractSequentialList::get index out of bounds");
            return it->next();
        }

        /**
         * Java 8:
         *   public E set(int index, E element)
         */
        jxx::Ptr<E> set(jxx::lang::jint index,
                        jxx::Ptr<E> element) override
        {
            auto it = this->listIterator(index);
            if (!it || !it->hasNext())
                throw std::out_of_range("AbstractSequentialList::set index out of bounds");

            auto oldValue = it->next();
            it->set(std::move(element));
            return oldValue;
        }

        /**
         * Java 8:
         *   public void add(int index, E element)
         */
        void add(jxx::lang::jint index,
                 jxx::Ptr<E> element) override
        {
            auto it = this->listIterator(index);
            if (!it)
                throw std::out_of_range("AbstractSequentialList::add index out of bounds");

            it->add(std::move(element));
        }

        /**
         * Java 8:
         *   public E remove(int index)
         */
        jxx::Ptr<E> remove(jxx::lang::jint index) override
        {
            auto it = this->listIterator(index);
            if (!it || !it->hasNext())
                throw std::out_of_range("AbstractSequentialList::remove index out of bounds");

            auto oldValue = it->next();
            it->remove();
            return oldValue;
        }

        /**
         * Java 8:
         *   public boolean addAll(int index, Collection<? extends E> c)
         *
         * Practical JXX/C++ approximation:
         *   jxx::Ptr<Collection<jxx::Ptr<E>>>
         *
         * This implementation assumes your JXX Collection<T> supports:
         *   - size()
         *   - get(int)
         *
         * If your actual Collection abstraction is iterator-based instead, you can
         * swap this loop to use c->iterator().
         */
        jxx::lang::jbool addAll(jxx::lang::jint index,
                                jxx::Ptr<Collection<jxx::Ptr<E>>> c) override
        {
            if (!c)
                throw std::invalid_argument("AbstractSequentialList::addAll null collection");

            auto it = this->listIterator(index);
            if (!it)
                throw std::out_of_range("AbstractSequentialList::addAll index out of bounds");

            jxx::lang::jbool modified = false;
            for (jxx::lang::jint i = 0; i < c->size(); ++i)
            {
                it->add(c->get(i));
                modified = true;
            }
            return modified;
        }

        /**
         * Java 8:
         *   public Iterator<E> iterator()
         *
         * Since ListIterator<E> is-a Iterator<E>, this delegates to listIterator(0).
         */
        jxx::Ptr<Iterator<jxx::Ptr<E>>> iterator() override
        {
            return std::static_pointer_cast<Iterator<jxx::Ptr<E>>>(this->listIterator(0));
        }

        /**
         * Java 8:
         *   public abstract ListIterator<E> listIterator(int index)
         *
         * This is the key abstract hook subclasses must implement.
         */
        virtual jxx::Ptr<ListIterator<jxx::Ptr<E>>> listIterator(jxx::lang::jint index) const = 0;
    };
}
