#pragma once

#include "jxx_types.h"
#include "util/jxx.util.Iterator.h"

namespace jxx::util
{
    template<typename E>
    class Collection
    {
    protected:
        Collection() = default;

    public:
        virtual ~Collection() = default;

    public:
        virtual jxx::lang::jint size() const = 0;
        virtual jxx::lang::jbool isEmpty() const = 0;
        virtual jxx::lang::jbool contains(jxx::Ptr<E> e) const = 0;
        virtual jxx::Ptr<Iterator<E>> iterator() = 0;
        virtual jxx::Ptr<E> get(jxx::lang::jint index) const = 0;
        virtual void add(const jxx::Ptr<E>& value) = 0;
        virtual jxx::lang::jbool addAll(jxx::Ptr<Collection<E>> c) = 0;
        virtual jxx::lang::jbool removeElement(jxx::Ptr<E> e) = 0;
        virtual void clear() = 0;
    };
}
