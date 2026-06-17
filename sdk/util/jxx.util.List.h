#pragma once

#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::util
{
    // Forward declarations for related Java 8 utility types.
    template<typename E>
    class Collection;

    template<typename E>
    class Iterator;

    template<typename E>
    class ListIterator;

    template<typename E>
    class List;

    template<typename E>
    class Spliterator;

    template<typename E>
    class Comparator;
}

namespace jxx::util::function
{
    template<typename T>
    class UnaryOperator;
}

namespace jxx::util
{
    /**
     * JXX / C++17 parity interface for Java 8 java.util.List<E>.
     *
     * Notes:
     * - This is an abstract base class used as an interface.
     * - Per JXX constraint, it does NOT derive from jxx::lang::Object.
     * - It assumes Collection<jxx::Ptr<E>> is also an interface-style abstract base.
     * - Java wildcard and default-method behavior is approximated where necessary.
     */
    template<typename E>
    class List : public Collection<jxx::Ptr<E>>
    {
    protected:
        List() = default;

    public:
        ~List() override = default;

    public:
        /**
         * Java 8:
         *   void add(int index, E element)
         */
        virtual void add(jxx::lang::jint index,
            jxx::Ptr<E> element) = 0;

        /**
         * Java 8:
         *   boolean addAll(int index, Collection<? extends E> c)
         *
         * Practical JXX approximation:
         *   Collection<jxx::Ptr<E>>
         */
        virtual jxx::lang::jbool addAll(
            jxx::lang::jint index,
            jxx::Ptr<Collection<jxx::Ptr<E>>> c) = 0;

        /**
         * Java 8:
         *   E get(int index)
         */
        virtual jxx::Ptr<E> get(jxx::lang::jint index) const = 0;

        /**
         * Java 8:
         *   int indexOf(Object o)
         */
        virtual jxx::lang::jint indexOf(
            jxx::Ptr<jxx::lang::Object> o) const = 0;

        /**
         * Java 8:
         *   int lastIndexOf(Object o)
         */
        virtual jxx::lang::jint lastIndexOf(
            jxx::Ptr<jxx::lang::Object> o) const = 0;

        /**
         * Java 8:
         *   ListIterator<E> listIterator()
         */
        virtual jxx::Ptr<ListIterator<jxx::Ptr<E>>> listIterator() = 0;

        /**
         * Java 8:
         *   ListIterator<E> listIterator(int index)
         */
        virtual jxx::Ptr<ListIterator<jxx::Ptr<E>>> listIterator(
            jxx::lang::jint index) = 0;

        /**
         * Java 8:
         *   E remove(int index)
         */
        virtual jxx::Ptr<E> remove(jxx::lang::jint index) = 0;

        /**
         * Java 8:
         *   E set(int index, E element)
         */
        virtual jxx::Ptr<E> set(jxx::lang::jint index,
            jxx::Ptr<E> element) = 0;

        /**
         * Java 8:
         *   List<E> subList(int fromIndex, int toIndex)
         */
        virtual jxx::Ptr<List<E>> subList(
            jxx::lang::jint fromIndex,
            jxx::lang::jint toIndex) = 0;

        /**
         * Java 8 default method:
         *   default void replaceAll(UnaryOperator<E> operator)
         *
         * Practical JXX parity:
         * - Preserved as a virtual member.
         * - Uses jxx::util::function::UnaryOperator<jxx::Ptr<E>>.
         */
        virtual void replaceAll(
            jxx::Ptr<jxx::util::function::UnaryOperator<jxx::Ptr<E>>> op) = 0;

        /**
         * Java 8 default method:
         *   default void sort(Comparator<? super E> c)
         *
         * Practical JXX approximation:
         * - Comparator<jxx::Ptr<E>>
         */
        virtual void sort(
            jxx::Ptr<Comparator<jxx::Ptr<E>>> c) = 0;

        /**
         * Java 8 default method:
         *   default Spliterator<E> spliterator()
         */
        virtual jxx::Ptr<Spliterator<jxx::Ptr<E>>> spliterator() = 0;
    };
}