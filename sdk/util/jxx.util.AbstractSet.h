#pragma once

#include "util/jxx.util.AbstractCollection.h"
#include "util/jxx.util.Set.h"

namespace jxx::util
{
    template<typename E>
    class AbstractSet
        : public AbstractCollection<E>
        , public Set<E>
    {
    protected:
        AbstractSet() = default;

    public:
        ~AbstractSet() override = default;
    };
}
