#pragma once

#include "util/jxx.util.AbstractCollection.h"
#include "util/jxx.util.List.h"

namespace jxx::util
{

    template <typename E>
    class AbstractList
        : public jxx::lang::ClassBase<
        AbstractList<E>,
        AbstractCollection<E>,
        List<E>> {
    protected:
        jxx::lang::jint modCount = 0;

    public:
        virtual ~AbstractList() = default;

        /*
         * Preserve the indexed List<E>::add overload.
         *
         * Concrete list implementations must implement this operation.
         */
        virtual void add(
            jxx::lang::jint index,
            const jxx::Ptr<E>& element)
            override = 0;

        /*
         * Java AbstractList.add(E) appends at size().
         */
        jxx::lang::jbool add(
            const jxx::Ptr<E>& element)
            override
        {

            this->add(
                this->size(),
                element);

            return true;
        }

        /*
         * Preserve the indexed List<E>::listIterator overload.
         *
         * Concrete list implementations must provide the fail-fast iterator.
         */
        virtual jxx::Ptr<ListIterator<E>>
            listIterator(
                jxx::lang::jint index)
            override = 0;

        /*
         * Java AbstractList.listIterator() begins at index zero.
         */
        jxx::Ptr<ListIterator<E>>
            listIterator() override
        {

            return this->listIterator(0);
        }
    };

} // namespace jxx::util