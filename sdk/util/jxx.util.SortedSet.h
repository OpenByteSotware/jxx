#pragma once

#include "util/jxx.util.Comparator.h"
#include "util/jxx.util.Set.h"

namespace jxx::util
{
    template<typename E>
    class SortedSet : public Set<E>
    {
    protected:
        SortedSet() = default;

    public:
        ~SortedSet() override = default;

    public:
        virtual jxx::Ptr<Comparator<E>> comparator() const = 0;
        virtual jxx::Ptr<E> first() const = 0;
        virtual jxx::Ptr<E> last() const = 0;
        virtual jxx::Ptr<SortedSet<E>> subSet(jxx::Ptr<E> fromElement,
                                              jxx::Ptr<E> toElement) const = 0;
        virtual jxx::Ptr<SortedSet<E>> headSet(jxx::Ptr<E> toElement) const = 0;
        virtual jxx::Ptr<SortedSet<E>> tailSet(jxx::Ptr<E> fromElement) const = 0;
    };
}
