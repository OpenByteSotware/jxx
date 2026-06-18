#pragma once

#include "lang/jxx.lang.Object.h"

namespace jxx::util
{
    template<typename E>
    class Comparator
    {
    protected:
        Comparator() = default;

    public:
        virtual ~Comparator() = default;

    public:
        virtual jxx::lang::jint compare(jxx::Ptr<E> a, jxx::Ptr<E> b) const = 0;
    };
}
