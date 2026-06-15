#pragma once

#include "lang/jxx_types.h"

namespace jxx::util
{
    template<typename T>
    class List
    {
    public:
        virtual ~List() = default;

    public:
        virtual jxx::lang::jint size() const = 0;
        virtual jxx::lang::jbool isEmpty() const = 0;
        virtual T get(jxx::lang::jint index) const = 0;
        virtual void add(const T& value) = 0;
    };
}
