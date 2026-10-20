#pragma once

#include "util/jxx.util.AbstractCollection.h"
#include "util/jxx.util.List.h"

#include <cstdint>

namespace jxx::util
{

    template <typename E>
    class AbstractList
        : public jxx::lang::ClassBase<
        AbstractList<E>,
        AbstractCollection<E>,
        List<E>> {
    protected:
        jxx::lang::jint modCount = 0;

    public:
        virtual ~AbstractList() = default;

        jxx::lang::jbool equals(
            const jxx::Ptr<jxx::lang::Object>& object) const override
        {
            if (object.get() == this) return true;
            auto other = jxx::CAST<List<E>>(object);
            auto self = const_cast<AbstractList<E>*>(this);
            const auto selfSize = self->size();
            if (other == nullptr || selfSize != other->size()) return false;
            for (jxx::lang::jint index = 0; index < selfSize; ++index) {
                const auto left = self->get(index);
                const auto right = other->get(index);
                if (left == right) continue;
                if (left == nullptr || right == nullptr) return false;
                const auto leftObject = jxx::CAST<jxx::lang::Object>(left);
                const auto rightObject = jxx::CAST<jxx::lang::Object>(right);
                if (leftObject == nullptr ||
                    !leftObject->equals(rightObject)) return false;
            }
            return true;
        }

        jxx::lang::jint hashCode() const override
        {
            std::uint32_t hash = 1U;
            auto self = const_cast<AbstractList<E>*>(this);
            const auto selfSize = self->size();
            for (jxx::lang::jint index = 0; index < selfSize; ++index) {
                const auto element = self->get(index);
                const auto object = jxx::CAST<jxx::lang::Object>(element);
                hash = 31U * hash + static_cast<std::uint32_t>(
                    object == nullptr ? 0 : object->hashCode());
            }
            return static_cast<jxx::lang::jint>(hash);
        }

        /*
         * Preserve the indexed List<E>::add overload.
         *
         * Concrete list implementations must implement this operation.
         */
        virtual void add(
            jxx::lang::jint index,
            const jxx::Ptr<E>& element)
            override = 0;

        /*
         * Java AbstractList.add(E) appends at size().
         */
        jxx::lang::jbool add(
            const jxx::Ptr<E>& element)
            override
        {

            this->add(
                this->size(),
                element);

            return true;
        }

        /*
         * Preserve the indexed List<E>::listIterator overload.
         *
         * Concrete list implementations must provide the fail-fast iterator.
         */
        virtual jxx::Ptr<ListIterator<E>>
            listIterator(
                jxx::lang::jint index)
            override = 0;

        /*
         * Java AbstractList.listIterator() begins at index zero.
         */
        jxx::Ptr<ListIterator<E>>
            listIterator() override
        {

            return this->listIterator(0);
        }
    };

} // namespace jxx::util