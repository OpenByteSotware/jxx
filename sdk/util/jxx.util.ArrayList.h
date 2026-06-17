#pragma once

#include <stdexcept>
#include <vector>

#include "util/jxx.util.Collection.h"

namespace jxx::util
{
    template<typename E>
    class ArrayList final : public jxx::lang::Object, public Collection<E>
    {
    public:
        ArrayList() = default;
        ~ArrayList() override = default;

    public:
        jxx::lang::jint size() const override
        {
            return static_cast<jxx::lang::jint>(items_.size());
        }

        jxx::lang::jbool isEmpty() const override
        {
            return items_.empty();
        }

        jxx::Ptr<E> get(jxx::lang::jint index) const override
        {
            if (index < 0 || static_cast<std::size_t>(index) >= items_.size())
                throw std::out_of_range("ArrayList index out of bounds");
            return items_[static_cast<std::size_t>(index)];
        }

        void add(const jxx::Ptr<E>& value) override
        {
            items_.push_back(value);
        }

        void clear() override
        {
            items_.clear();
        }

    private:
        std::vector<jxx::Ptr<E>> items_;
    };
}
