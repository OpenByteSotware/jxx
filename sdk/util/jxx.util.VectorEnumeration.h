#pragma once

#include <stdexcept>
#include <vector>

#include "lang/jxx_types.h"
#include "util/jxx.util.Enumeration.h"

namespace jxx::util
{
    template<typename T>
    class VectorEnumeration final : public Enumeration<T>
    {
    public:
        explicit VectorEnumeration(std::vector<T> items)
            : items_(std::move(items))
        {
        }

        ~VectorEnumeration() override = default;

    public:
        jxx::lang::jbool hasMoreElements() override
        {
            return index_ < items_.size();
        }

        T nextElement() override
        {
            if (index_ >= items_.size())
                throw std::runtime_error("no more elements");
            return items_[index_++];
        }

    private:
        std::vector<T> items_;
        std::size_t index_ = 0;
    };
}
