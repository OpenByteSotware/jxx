#pragma once

#include "util/jxx.util.Collection.h"

namespace jxx::util
{
    template<typename E>
    class Set : public Collection<E>
    {
    protected:
        Set() = default;

    public:
        ~Set() override = default;

    public:
        virtual jxx::lang::jbool addElement(jxx::Ptr<E> e) = 0;
    };
}
