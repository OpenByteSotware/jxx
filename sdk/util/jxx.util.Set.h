#pragma once

#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Collection.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.Spliterator.h"
#include "util/jxx.util.wildcard.CollectionAny.h"
#include "util/jxx.util.wildcard.CollectionExtends.h"

namespace jxx::util
{

    template <typename E>
    class Set
        : public virtual Collection<E>
    {
    public:
        virtual ~Set() = default;

        virtual jxx::lang::jint size() = 0;

        virtual jxx::lang::jbool isEmpty() override
        {
            return size() == 0;
        }

        virtual jxx::lang::jbool contains(
            const jxx::Ptr<jxx::lang::Object>& object) = 0;

        virtual jxx::Ptr<Iterator<E>> iterator() = 0;

        virtual jxx::lang::ObjectArray toArray() = 0;

        virtual jxx::lang::jbool add(
            const jxx::Ptr<E>& element) = 0;

        virtual jxx::lang::jbool remove(
            const jxx::Ptr<jxx::lang::Object>& object) = 0;

        virtual jxx::lang::jbool containsAll(
            const jxx::Ptr<
                wildcard::CollectionAny>& collection) = 0;

        virtual jxx::lang::jbool addAll(
            const jxx::Ptr<
                wildcard::CollectionExtends<E>>&collection) = 0;

        virtual jxx::lang::jbool retainAll(
            const jxx::Ptr<
                wildcard::CollectionAny>& collection) = 0;

        virtual jxx::lang::jbool removeAll(
            const jxx::Ptr<
                wildcard::CollectionAny>& collection) = 0;

        virtual void clear() = 0;

        /*
         * Must exactly match Object::equals().
         */
        virtual jxx::lang::jbool equals(
            const jxx::Ptr<jxx::lang::Object>& object)
            const = 0;

        /*
         * Must exactly match Object::hashCode().
         */
        virtual jxx::lang::jint hashCode()
            const = 0;

        virtual jxx::Ptr<Spliterator<E>>
            spliterator() = 0;
    };

} // namespace jxx::util