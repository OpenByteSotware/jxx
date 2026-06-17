#pragma once

#include "lang/jxx_types.h"

namespace jxx::util
{
    template<typename E>
    class Iterator
    {
    protected:
        Iterator() = default;

    public:
        virtual ~Iterator() = default;

    public:
        virtual jxx::lang::jbool hasNext() const = 0;
        virtual jxx::Ptr<E> next() = 0;
        virtual void remove() = 0;
    };
}
