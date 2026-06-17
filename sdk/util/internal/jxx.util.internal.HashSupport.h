#pragma once

#include <cstddef>

#include "jxx_types.h"

namespace jxx::util::internal
{
    template<typename T>
    inline jxx::lang::jbool ptrEquals(jxx::Ptr<T> a,
                                      jxx::Ptr<T> b)
    {
        return (!a && !b) || (a && b && a->equals(b));
    }

    template<typename T>
    struct PtrHash
    {
        std::size_t operator()(const jxx::Ptr<T>& p) const noexcept
        {
            if (!p)
                return 0U;
            return static_cast<std::size_t>(p->hashCode());
        }
    };

    template<typename T>
    struct PtrEq
    {
        bool operator()(const jxx::Ptr<T>& a,
                        const jxx::Ptr<T>& b) const noexcept
        {
            return ptrEquals(a, b);
        }
    };
}
