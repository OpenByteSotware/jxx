#pragma once

#include <cstdint>

#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.AbstractCollection.h"
#include "util/jxx.util.ConcurrentModificationException.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.ListIterator.h"
#include "util/jxx.util.NoSuchElementException.h"

namespace jxx::util
{

    /**
     * Java 8 compatible skeletal implementation of java.util.AbstractList<E>.
     *
     * JXX constraints:
     *
     * - AbstractList is a Java class and inherits Object through
     *   AbstractCollection<E>.
     * - List, Iterator, and ListIterator are Java interfaces represented as
     *   abstract C++17 base classes and do not inherit Object.
     * - Every jxx::Ptr<T> parameter is passed as const jxx::Ptr<T>&.
     * - STL types are not exposed through the public API.
     * - Template implementation remains header-only.
     */
    template <typename E>
    class AbstractList
        : public AbstractCollection<E>
        , public virtual List<E>
    {
    protected:
        /**
         * Number of structural modifications made to this list.
         *
         * Concrete subclasses must increment modCount whenever the size of the
         * list changes or another structural modification occurs.
         */
        jxx::lang::jint modCount = 0;

    private:
        /**
         * Compares an element with an Object using Java Object.equals semantics.
         *
         * E may itself be a Java interface, such as Runnable. Therefore Object
         * methods cannot be called directly through Ptr<E>. The concrete object
         * implementing E is cast to Object first.
         */
        static jxx::lang::jbool elementEqualsObject_(
            const jxx::Ptr<E>& element,
            const jxx::Ptr<jxx::lang::Object> object)
        {

            if (element == nullptr || object == nullptr) {
                return static_cast<jxx::lang::jbool>(
                    element == nullptr && object == nullptr);
            }

            auto elementObject =
                jxx::CAST<jxx::lang::Object>(element);

            if (elementObject == nullptr) {
                return static_cast<jxx::lang::jbool>(false);
            }

            return elementObject->equals(object);
        }

        /**
         * Returns an element's Java Object hash code.
         *
         * Interface-typed elements are first cast to Object through their concrete
         * Java class implementation.
         */
        static jxx::lang::jint elementHashCode_(
            const jxx::Ptr<E>& element)
        {

            if (element == nullptr) {
                return 0;
            }

            auto elementObject =
                jxx::CAST<jxx::lang::Object>(element);

            return elementObject == nullptr
                ? 0
                : elementObject->hashCode();
        }

    public:
        virtual ~AbstractList() = default;

        // ---------------------------------------------------------------------
        // Collection methods redeclared by List
        // ---------------------------------------------------------------------

        jxx::lang::jbool contains(
            const jxx::Ptr<jxx::lang::Object> object)
            override
        {

            return AbstractCollection<E>::contains(object);
        }

        jxx::lang::jbool containsAll(
            const jxx::Ptr<wildcard::CollectionAny> collection)
            override
        {

            return AbstractCollection<E>::containsAll(collection);
        }

        jxx::lang::jbool isEmpty() override
        {
            return AbstractCollection<E>::isEmpty();
        }

        jxx::lang::ObjectArray toArray() override
        {
            return AbstractCollection<E>::toArray();
        }

        jxx::lang::jbool removeAll(
            const jxx::Ptr<wildcard::CollectionAny> collection)
            override
        {

            return AbstractCollection<E>::removeAll(collection);
        }

        jxx::lang::jbool retainAll(
            const jxx::Ptr<wildcard::CollectionAny> collection)
            override
        {

            return AbstractCollection<E>::retainAll(collection);
        }

        // ---------------------------------------------------------------------
        // Positional list operations
        // ---------------------------------------------------------------------

        /**
         * Appends an element to the end of the list.
         *
         * The default implementation delegates to add(size(), element). A list
         * that does not support insertion may leave indexed add unsupported.
         */
        jxx::lang::jbool add(
            const jxx::Ptr<E> element)
            override
        {

            add(element);
            return static_cast<jxx::lang::jbool>(true);
        }

        /**
         * Inserts every element from collection at the end of this list.
         */
        jxx::lang::jbool addAll(
            const jxx::Ptr<wildcard::CollectionExtends<E>> collection)
            override
        {

            return addAll(this->size(), collection);
        }

        /**
         * Inserts the collection's elements beginning at index, preserving order.
         */
        jxx::lang::jbool addAll(
            jxx::lang::jint index,
            const jxx::Ptr<wildcard::CollectionExtends<E>> collection)
            override
        {

            if (collection == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            rangeCheckForAdd(index);

            auto iterator = collection->iteratorExtends();
            jxx::lang::jint insertionIndex = index;
            jxx::lang::jbool modified = false;

            while (iterator->hasNext()) {
                add(iterator->next());
                ++insertionIndex;
                modified = true;
            }

            return modified;
        }

        jxx::lang::jint indexOf(
            const jxx::Ptr<jxx::lang::Object> object)
            override
        {

            auto iterator = listIterator();

            while (iterator->hasNext()) {
                auto element = iterator->next();

                if (elementEqualsObject_(element, object)) {
                    return iterator->previousIndex();
                }
            }

            return -1;
        }

        jxx::lang::jint lastIndexOf(
            const jxx::Ptr<jxx::lang::Object> object)
            override
        {

            auto iterator = listIterator(this->size());

            while (iterator->hasPrevious()) {
                auto element = iterator->previous();

                if (elementEqualsObject_(element, object)) {
                    return iterator->nextIndex();
                }
            }

            return -1;
        }

        /**
         * Removes and returns the element at index through the list iterator.
         */
        jxx::Ptr<E> remove(jxx::lang::jint index)
            override
        {

            auto iterator = listIterator(index);
            auto oldValue = iterator->next();
            iterator->remove();
            return oldValue;
        }

        jxx::lang::jbool remove(const jxx::Ptr<jxx::lang::Object>& object)
            override
        {

            const jxx::lang::jint index = indexOf(object);

            if (index < 0) {
                return static_cast<jxx::lang::jbool>(false);
            }

            this->remove(index);
            return static_cast<jxx::lang::jbool>(true);
        }

        void clear() override
        {
            removeRange(0, this->size());
        }

        // ---------------------------------------------------------------------
        // Object contract
        // ---------------------------------------------------------------------

        /**
         * Java List equality contract: ordered element-wise equality.
         */
        jxx::lang::jbool equals(const jxx::Ptr<jxx::lang::Object>& other)
            override
        {

            if (this->same(other)) {
                return static_cast<jxx::lang::jbool>(true);
            }

            auto collection =
                jxx::CAST<wildcard::CollectionAny>(other);

            if (collection == nullptr) {
                return static_cast<jxx::lang::jbool>(false);
            }

            if (collection->size() != this->size()) {
                return static_cast<jxx::lang::jbool>(false);
            }

            auto leftIterator = this->iterator();
            auto rightIterator = collection->iteratorObject();

            while (leftIterator->hasNext() &&
                   rightIterator->hasNext()) {

                auto leftElement = leftIterator->next();
                auto rightObject = rightIterator->next();

                if (!elementEqualsObject_(leftElement, rightObject)) {
                    return static_cast<jxx::lang::jbool>(false);
                }
            }

            return static_cast<jxx::lang::jbool>(
                !leftIterator->hasNext() &&
                !rightIterator->hasNext());
        }

        /**
         * Java List hashCode contract with defined 32-bit wraparound.
         */
        jxx::lang::jint hashCode() override
        {
            std::uint32_t hash = 1U;

            auto iterator = this->iterator();

            while (iterator->hasNext()) {
                const auto elementHash =
                    elementHashCode_(iterator->next());

                hash =
                    hash * 31U +
                    static_cast<std::uint32_t>(elementHash);
            }

            return static_cast<jxx::lang::jint>(
                static_cast<std::int32_t>(hash));
        }

        // ---------------------------------------------------------------------
        // Iterators
        // ---------------------------------------------------------------------

        class Itr : public virtual Iterator<E>
        {
        protected:
            AbstractList<E>* list;
            jxx::lang::jint cursor;
            jxx::lang::jint lastRet;
            jxx::lang::jint expectedModCount;

        public:
            explicit Itr(AbstractList<E>* owner)
                : list(owner)
                , cursor(0)
                , lastRet(-1)
                , expectedModCount(owner->modCount)
            {
            }

            virtual ~Itr() = default;

            jxx::lang::jbool hasNext() override
            {
                return static_cast<jxx::lang::jbool>(
                    cursor != list->size());
            }

            jxx::Ptr<E> next() override
            {
                checkForComodification();

                const jxx::lang::jint index = cursor;

                if (index >= list->size()) {
                    throw jxx::util::NoSuchElementException();
                }

                auto value = list->get(index);
                cursor = index + 1;
                lastRet = index;
                return value;
            }

            void remove() override
            {
                if (lastRet < 0) {
                    throw jxx::lang::IllegalStateException();
                }

                checkForComodification();

                list->remove(lastRet);

                if (lastRet < cursor) {
                    --cursor;
                }

                lastRet = -1;
                expectedModCount = list->modCount;
            }

        protected:
            void checkForComodification() const
            {
                if (list->modCount != expectedModCount) {
                    throw jxx::util::ConcurrentModificationException();
                }
            }
        };

        class ListItr
            : public Itr
            , public virtual ListIterator<E>
        {
        public:
            ListItr(
                AbstractList<E>* owner,
                jxx::lang::jint index)
                : Itr(owner)
            {

                this->cursor = index;
            }

            virtual ~ListItr() = default;

            jxx::lang::jbool hasPrevious() override
            {
                return static_cast<jxx::lang::jbool>(
                    this->cursor != 0);
            }

            jxx::Ptr<E> previous() override
            {
                this->checkForComodification();

                const jxx::lang::jint index =
                    this->cursor - 1;

                if (index < 0) {
                    throw jxx::util::NoSuchElementException();
                }

                auto value = this->list->get(index);
                this->cursor = index;
                this->lastRet = index;
                return value;
            }

            jxx::lang::jint nextIndex() override
            {
                return this->cursor;
            }

            jxx::lang::jint previousIndex() override
            {
                return this->cursor - 1;
            }

            void set(
                const jxx::Ptr<E> element)
                override
            {

                if (this->lastRet < 0) {
                    throw jxx::lang::IllegalStateException();
                }

                this->checkForComodification();
                this->list->set(this->lastRet, element);
                this->expectedModCount = this->list->modCount;
            }

            void add(
                const jxx::Ptr<E> element)
                override
            {

                this->checkForComodification();

                const jxx::lang::jint index =
                    this->cursor;

                this->list->add(element);
                this->cursor = index + 1;
                this->lastRet = -1;
                this->expectedModCount = this->list->modCount;
            }
        };

        jxx::Ptr<Iterator<E>> iterator() override
        {
            return jxx::Ptr<Iterator<E>>(
                new Itr(this));
        }

        jxx::Ptr<ListIterator<E>> listIterator() override
        {
            return listIterator(0);
        }

        jxx::Ptr<ListIterator<E>> listIterator(
            jxx::lang::jint index)
            override
        {

            rangeCheckForAdd(index);

            return jxx::Ptr<ListIterator<E>>(
                new ListItr(this, index));
        }

        /**
         * A backed SubList implementation can be added when the repository's
         * SubList class has completed the same const-reference migration.
         */
        jxx::Ptr<List<E>> subList(
            jxx::lang::jint /* fromIndex */,
            jxx::lang::jint /* toIndex */)
            override
        {

            throw jxx::lang::UnsupportedOperationException();
        }

    protected:
        /**
         * Removes the half-open range [fromIndex, toIndex).
         */
        virtual void removeRange(
            jxx::lang::jint fromIndex,
            jxx::lang::jint toIndex)
        {

            if (fromIndex > toIndex) {
                throw jxx::lang::IndexOutOfBoundsException();
            }

            rangeCheckForAdd(fromIndex);
            rangeCheckForAdd(toIndex);

            auto iterator = listIterator(fromIndex);
            const jxx::lang::jint count =
                toIndex - fromIndex;

            for (jxx::lang::jint i = 0;
                 i < count;
                 ++i) {

                iterator->next();
                iterator->remove();
            }
        }

        void rangeCheckForAdd(jxx::lang::jint index)
        {

            if (index < 0 || index > this->size()) {

                throw jxx::lang::IndexOutOfBoundsException();
            }
        }
    };

} // namespace jxx::util
