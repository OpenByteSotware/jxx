
#pragma once
#include "lang/jxx.lang.h"
#include "lang/jxx.lang.Iterable.h"
#include "util/jxx.util.wildcard.CollectionAny.h"
#include "util/jxx.util.wildcard.CollectionExtends.h"

namespace jxx::util {
    template <typename E>
    class Collection
        : public virtual jxx::lang::Iterable<E> {
    public:
        virtual ~Collection() = default;

        virtual jxx::lang::jint size() = 0;

        virtual jxx::lang::jbool isEmpty() = 0;

        virtual jxx::lang::jbool contains(
            jxx::Ptr<jxx::lang::Object> object) = 0;

        virtual jxx::Ptr<Iterator<E>> iterator() override = 0;

        virtual jxx::lang::ObjectArray toArray() = 0;

        virtual jxx::lang::jbool add(
            jxx::Ptr<E> element) = 0;

        virtual jxx::lang::jbool remove(
            jxx::Ptr<jxx::lang::Object> object) = 0;

        virtual jxx::lang::jbool containsAll(
            jxx::Ptr<wildcard::CollectionAny> collection) = 0;

        virtual jxx::lang::jbool addAll(
            jxx::Ptr<wildcard::CollectionExtends<E>> collection) = 0;

        virtual jxx::lang::jbool removeAll(
            jxx::Ptr<wildcard::CollectionAny> collection) = 0;

        virtual jxx::lang::jbool retainAll(
            jxx::Ptr<wildcard::CollectionAny> collection) = 0;

        virtual void clear() = 0;
    };
}