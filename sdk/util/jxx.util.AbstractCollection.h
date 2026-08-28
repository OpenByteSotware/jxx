#pragma once

#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"

#include "util/jxx.util.Collection.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.wildcard.CollectionAny.h"
#include "util/jxx.util.wildcard.CollectionExtends.h"
#include "lang/jxx.lang.ClassInfo.h"

namespace jxx {
    namespace util {

        template <typename E>
		class AbstractCollection : public jxx::lang::ClassBase<AbstractCollection<E>, 
            jxx::lang::Object, Collection<E>>
        {
        public:
            AbstractCollection() = default;
            virtual ~AbstractCollection() = default;

            virtual jxx::lang::jbool isEmpty() override {
                return static_cast<jxx::lang::jbool>(
                    this->size() == 0);
            }

            virtual jxx::lang::jbool contains(
                jxx::Ptr<jxx::lang::Object> object) override {

                auto iterator = this->iterator();

                if (object == nullptr) {
                    while (iterator->hasNext()) {
                        if (iterator->next() == nullptr) {
                            return static_cast<jxx::lang::jbool>(
                                true);
                        }
                    }
                }
                else {
                    while (iterator->hasNext()) {
                        auto element = iterator->next();

                        if (element != nullptr) {
                            auto elementObject =
                                jxx::CAST<jxx::lang::Object>(
                                    element);

                            if (elementObject != nullptr &&
                                object->equals(elementObject)) {

                                return static_cast<
                                    jxx::lang::jbool>(true);
                            }
                        }
                    }
                }

                return static_cast<jxx::lang::jbool>(false);
            }

            virtual jxx::lang::ObjectArray toArray() override {
                const jxx::lang::jint count =
                    this->size();

                auto result =
                    jxx::NEW<jxx::lang::ObjectArrayType>(
                        static_cast<
                        jxx::lang::ObjectArrayType::size_type>(
                            count));

                auto iterator = this->iterator();

                jxx::lang::jint index = 0;

                while (iterator->hasNext()) {
                    (*result)[index++] =
                        jxx::CAST<jxx::lang::Object>(
                            iterator->next());
                }

                return result;
            }

            /*
             * Java AbstractCollection.add(E) has a concrete default
             * implementation that throws UnsupportedOperationException.
             *
             * This is preferable to redeclaring it pure virtual because it
             * provides one unambiguous final overrider at this hierarchy level.
             */
            virtual jxx::lang::jbool add(
                jxx::Ptr<E> /* element */) override {

                throw jxx::lang::UnsupportedOperationException();
            }

            virtual jxx::lang::jbool remove(
                const jxx::Ptr<jxx::lang::Object>& object) override {

                auto iterator = this->iterator();

                if (object == nullptr) {
                    while (iterator->hasNext()) {
                        if (iterator->next() == nullptr) {
                            iterator->remove();

                            return static_cast<
                                jxx::lang::jbool>(true);
                        }
                    }
                }
                else {
                    while (iterator->hasNext()) {
                        auto element = iterator->next();

                        if (element != nullptr) {
                            auto elementObject =
                                jxx::CAST<jxx::lang::Object>(
                                    element);

                            if (elementObject != nullptr &&
                                object->equals(elementObject)) {

                                iterator->remove();

                                return static_cast<
                                    jxx::lang::jbool>(true);
                            }
                        }
                    }
                }

                return static_cast<jxx::lang::jbool>(false);
            }

            virtual jxx::lang::jbool containsAll(
                jxx::Ptr<wildcard::CollectionAny> collection)
                override {

                if (collection == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                auto iterator =
                    collection->iteratorObject();

                while (iterator->hasNext()) {
                    if (!this->contains(iterator->next())) {
                        return static_cast<
                            jxx::lang::jbool>(false);
                    }
                }

                return static_cast<jxx::lang::jbool>(true);
            }

            virtual jxx::lang::jbool addAll(
                jxx::Ptr<wildcard::CollectionExtends<E>>
                collection) override {

                if (collection == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                jxx::lang::jbool modified =
                    static_cast<jxx::lang::jbool>(false);

                auto iterator =
                    collection->iteratorExtends();

                while (iterator->hasNext()) {
                    if (this->add(iterator->next())) {
                        modified =
                            static_cast<jxx::lang::jbool>(
                                true);
                    }
                }

                return modified;
            }

            virtual jxx::lang::jbool removeAll(
                jxx::Ptr<wildcard::CollectionAny> collection)
                override {

                if (collection == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                jxx::lang::jbool modified =
                    static_cast<jxx::lang::jbool>(false);

                auto iterator = this->iterator();

                while (iterator->hasNext()) {
                    auto element = iterator->next();

                    auto elementObject =
                        jxx::CAST<jxx::lang::Object>(
                            element);

                    if (collection->containsObject(
                        elementObject)) {

                        iterator->remove();

                        modified =
                            static_cast<jxx::lang::jbool>(
                                true);
                    }
                }

                return modified;
            }

            virtual jxx::lang::jbool retainAll(
                jxx::Ptr<wildcard::CollectionAny> collection)
                override {

                if (collection == nullptr) {
                    throw jxx::lang::NullPointerException();
                }

                jxx::lang::jbool modified =
                    static_cast<jxx::lang::jbool>(false);

                auto iterator = this->iterator();

                while (iterator->hasNext()) {
                    auto element = iterator->next();

                    auto elementObject =
                        jxx::CAST<jxx::lang::Object>(
                            element);

                    if (!collection->containsObject(
                        elementObject)) {

                        iterator->remove();

                        modified =
                            static_cast<jxx::lang::jbool>(
                                true);
                    }
                }

                return modified;
            }

            virtual void clear() override {
                auto iterator = this->iterator();

                while (iterator->hasNext()) {
                    iterator->next();
                    iterator->remove();
                }
            }
        };

    } // namespace util
} // namespace jxx