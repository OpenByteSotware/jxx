#pragma once

#include <stdexcept>

#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Collection.h"

namespace jxx::util
{
    namespace internal
    {
        template<typename T>
        inline bool ptrEquals(jxx::Ptr<T> a, jxx::Ptr<T> b)
        {
            return (!a && !b) || (a && b && a->equals(b));
        }
    }

    template<typename E>
    class AbstractCollection
        : public jxx::lang::Object
        , public Collection<E>
    {
    protected:
        AbstractCollection() = default;

    public:
        ~AbstractCollection() override = default;

    public:
        jxx::lang::jbool isEmpty() const override
        {
            return this->size() == 0;
        }

        jxx::lang::jbool contains(jxx::Ptr<E> e) const override
        {
            for (jxx::lang::jint i = 0; i < this->size(); ++i)
                if (internal::ptrEquals(this->get(i), e))
                    return true;
            return false;
        }

        jxx::lang::jbool addAll(jxx::Ptr<Collection<E>> c) override
        {
            if (!c)
                return false;
            jxx::lang::jbool modified = false;
            for (jxx::lang::jint i = 0; i < c->size(); ++i)
            {
                this->add(c->get(i));
                modified = true;
            }
            return modified;
        }

        void add(const jxx::Ptr<E>&) override
        {
            throw std::runtime_error("AbstractCollection::add unsupported by default");
        }

        jxx::lang::jbool removeElement(jxx::Ptr<E>) override
        {
            throw std::runtime_error("AbstractCollection::removeElement unsupported by default");
        }
    };
}
