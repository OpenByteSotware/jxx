#pragma once

#include <cstdint>
#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "lang/jxx.lang.buildin_array.h"
#include "util/jxx.util.Collection.h"

namespace jxx::util
{

    template <typename E>
    class AbstractCollection
        : public jxx::lang::ClassBase<
        AbstractCollection<E>,
        jxx::lang::Object,
        Collection<E>> {
    public:
        virtual ~AbstractCollection() = default;

        jxx::lang::jbool isEmpty() override
        {
            return this->size() == 0;
        }

        jxx::lang::jbool add(
            const jxx::Ptr<E>& /* element */)
            override
        {

            throw jxx::lang::
                UnsupportedOperationException();
        }

        jxx::lang::ObjectArray toArray() override
        {
            std::vector<jxx::Ptr<jxx::lang::Object>> snapshot;

            const auto expectedSize = this->size();
            if (expectedSize > 0) {
                snapshot.reserve(
                    static_cast<std::size_t>(expectedSize));
            }

            auto iteratorValue = this->iterator();
            while (iteratorValue->hasNext()) {
                snapshot.push_back(
                    jxx::CAST<jxx::lang::Object>(
                        iteratorValue->next()));
            }

            auto result =
                jxx::NEW<jxx::lang::ObjectArrayType>(
                    static_cast<std::uint32_t>(snapshot.size()));

            for (std::size_t index = 0;
                 index < snapshot.size();
                 ++index) {
                (*result)[static_cast<jxx::lang::jint>(index)] =
                    snapshot[index];
            }

            return result;
        }

        jxx::lang::jbool contains(
            const jxx::Ptr<
                jxx::lang::Object>& object)
            override
        {

            auto iteratorValue =
                this->iterator();

            while (iteratorValue->hasNext()) {
                auto elementObject =
                    jxx::CAST<
                    jxx::lang::Object>(
                        iteratorValue->next());

                if (object == nullptr
                        ? elementObject == nullptr
                        : object->equals(elementObject)) {

                    return true;
                }
            }

            return false;
        }

        jxx::lang::jbool remove(
            const jxx::Ptr<
                jxx::lang::Object>& object)
            override
        {

            auto iteratorValue =
                this->iterator();

            while (iteratorValue->hasNext()) {
                auto elementObject =
                    jxx::CAST<
                    jxx::lang::Object>(
                        iteratorValue->next());

                if (object == nullptr
                        ? elementObject == nullptr
                        : object->equals(elementObject)) {

                    iteratorValue->remove();
                    return true;
                }
            }

            return false;
        }

        jxx::lang::jbool containsAll(
            const jxx::Ptr<
                wildcard::CollectionAny>& collection)
            override
        {

            if (collection == nullptr) {
                throw jxx::lang::
                    NullPointerException();
            }

            auto iteratorValue =
                collection->iteratorObject();

            while (iteratorValue->hasNext()) {
                if (!this->contains(
                    iteratorValue->next())) {

                    return false;
                }
            }

            return true;
        }

        jxx::lang::jbool addAll(
            const jxx::Ptr<
                wildcard::CollectionExtends<E>>&collection)
            override
        {

            if (collection == nullptr) {
                throw jxx::lang::
                    NullPointerException();
            }

            jxx::lang::jbool modified = false;

            auto iteratorValue =
                collection->iteratorExtends();

            while (iteratorValue->hasNext()) {
                if (this->add(
                    iteratorValue->next())) {

                    modified = true;
                }
            }

            return modified;
        }

        jxx::lang::jbool removeAll(
            const jxx::Ptr<
                wildcard::CollectionAny>& collection)
            override
        {

            if (collection == nullptr) {
                throw jxx::lang::
                    NullPointerException();
            }

            jxx::lang::jbool modified = false;

            auto iteratorValue =
                this->iterator();

            while (iteratorValue->hasNext()) {
                auto elementObject =
                    jxx::CAST<
                    jxx::lang::Object>(
                        iteratorValue->next());

                if (collection->containsObject(
                    elementObject)) {

                    iteratorValue->remove();
                    modified = true;
                }
            }

            return modified;
        }

        jxx::lang::jbool retainAll(
            const jxx::Ptr<
                wildcard::CollectionAny>& collection)
            override
        {

            if (collection == nullptr) {
                throw jxx::lang::
                    NullPointerException();
            }

            jxx::lang::jbool modified = false;

            auto iteratorValue =
                this->iterator();

            while (iteratorValue->hasNext()) {
                auto elementObject =
                    jxx::CAST<
                    jxx::lang::Object>(
                        iteratorValue->next());

                if (!collection->containsObject(
                    elementObject)) {

                    iteratorValue->remove();
                    modified = true;
                }
            }

            return modified;
        }

        void clear() override
        {
            auto iteratorValue =
                this->iterator();

            while (iteratorValue->hasNext()) {
                iteratorValue->next();
                iteratorValue->remove();
            }
        }
    };

} // namespace jxx::util