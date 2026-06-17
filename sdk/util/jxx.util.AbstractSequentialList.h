#pragma once

#include <stdexcept>

#include "util/jxx.util.AbstractList.h"

namespace jxx::util
{
    template<typename E>
    class AbstractSequentialList : public AbstractList<E>
    {
    protected:
        AbstractSequentialList() = default;

    public:
        ~AbstractSequentialList() override = default;

    public:
        jxx::Ptr<E> get(jxx::lang::jint index) const override
        {
            auto self = const_cast<AbstractSequentialList<E>*>(this);
            auto it = self->listIterator(index);
            if (!it || !it->hasNext())
                throw std::out_of_range("AbstractSequentialList::get index out of bounds");
            return it->next();
        }

        jxx::Ptr<E> set(jxx::lang::jint index,
                        jxx::Ptr<E> element) override
        {
            auto it = this->listIterator(index);
            if (!it || !it->hasNext())
                throw std::out_of_range("AbstractSequentialList::set index out of bounds");
            auto old = it->next();
            it->set(std::move(element));
            return old;
        }

        void add(jxx::lang::jint index,
                 jxx::Ptr<E> element) override
        {
            auto it = this->listIterator(index);
            if (!it)
                throw std::out_of_range("AbstractSequentialList::add index out of bounds");
            it->add(std::move(element));
        }

        jxx::Ptr<E> remove(jxx::lang::jint index) override
        {
            auto it = this->listIterator(index);
            if (!it || !it->hasNext())
                throw std::out_of_range("AbstractSequentialList::remove index out of bounds");
            auto old = it->next();
            it->remove();
            return old;
        }

        jxx::lang::jbool addAll(jxx::lang::jint index,
                                jxx::Ptr<Collection<E>> c) override
        {
            if (!c)
                return false;
            auto it = this->listIterator(index);
            if (!it)
                throw std::out_of_range("AbstractSequentialList::addAll index out of bounds");
            auto src = c->iterator();
            jxx::lang::jbool modified = false;
            while (src && src->hasNext())
            {
                it->add(src->next());
                modified = true;
            }
            return modified;
        }
    };
}
