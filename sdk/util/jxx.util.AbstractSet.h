#pragma once
#include "util/jxx.util.Spliterator.h"
#include "lang/jxx.lang.ClassCastException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "util/jxx.util.AbstractCollection.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.Set.h"
#include "util/jxx.util.wildcard.CollectionAny.h"

namespace jxx::util {

    template <typename E>
    class AbstractSet
        : public AbstractCollection<E>
        , public virtual Set<E> {
    public:
        virtual ~AbstractSet() = default;

        /*
         * Explicit final overriders resolve the C++ multiple-inheritance
         * convergence while preserving the public Java Set interface.
         */

        virtual jxx::lang::jbool isEmpty() override {
            return AbstractCollection<E>::isEmpty();
        }

        virtual jxx::lang::jbool contains(
            const jxx::Ptr<jxx::lang::Object>& object) override {

            return AbstractCollection<E>::contains(object);
        }

        virtual jxx::lang::ObjectArray
            toArray() override {

            return AbstractCollection<E>::toArray();
        }

        virtual jxx::lang::jbool add(const jxx::Ptr<E>& element) override {

            return AbstractCollection<E>::add(element);
        }

        virtual jxx::lang::jbool remove(const jxx::Ptr<jxx::lang::Object>& object) override {

            return AbstractCollection<E>::remove(object);
        }

        virtual jxx::lang::jbool containsAll(
            const jxx::Ptr<wildcard::CollectionAny>& collection)
            override {

            return AbstractCollection<E>::containsAll(
                collection);
        }

        virtual jxx::lang::jbool addAll(const jxx::Ptr<wildcard::CollectionExtends<E>>& collection) override {

            return AbstractCollection<E>::addAll(
                collection);
        }

        virtual jxx::lang::jbool retainAll(const jxx::Ptr<wildcard::CollectionAny>& collection)
            override {

            return AbstractCollection<E>::retainAll(
                collection);
        }

        virtual void clear() override {
            AbstractCollection<E>::clear();
        }

        jxx::Ptr<Spliterator<E>>
            spliterator() override
        {
            throw jxx::lang::UnsupportedOperationException();
        }


        virtual jxx::lang::jbool equals(const jxx::Ptr<jxx::lang::Object>& object) const override {

            if (object == nullptr) {
                return static_cast<jxx::lang::jbool>(false);
            }

            auto other =
                jxx::CAST<wildcard::CollectionAny>(object);

            if (other == nullptr) {
                return static_cast<jxx::lang::jbool>(false);
            }

            auto self =
                const_cast<AbstractSet<E>*>(this);

            if (other->size() != self->size()) {
                return static_cast<jxx::lang::jbool>(false);
            }

            try {
                return self->containsAll(other);
            }
            catch (const jxx::lang::ClassCastException&) {
                return static_cast<jxx::lang::jbool>(false);
            }
            catch (const jxx::lang::NullPointerException&) {
                return static_cast<jxx::lang::jbool>(false);
            }
        }

        virtual jxx::lang::jint hashCode()
            const override {

            jxx::lang::jint hash = 0;

            auto self =
                const_cast<AbstractSet<E>*>(this);

            auto iterator = self->iterator();

            while (iterator->hasNext()) {
                auto element = iterator->next();

                if (element == nullptr) {
                    continue;
                }

                auto object =
                    jxx::CAST<jxx::lang::Object>(
                        element);

                if (object != nullptr) {
                    hash += object->hashCode();
                }
            }

            return hash;
        }

        virtual jxx::lang::jbool removeAll(const jxx::Ptr<wildcard::CollectionAny>& collection)
            override {

            if (collection == nullptr) {
                throw jxx::lang::NullPointerException();
            }

            jxx::lang::jbool modified =
                static_cast<jxx::lang::jbool>(false);

            if (this->size() > collection->size()) {
                auto iterator =
                    collection->iteratorObject();

                while (iterator->hasNext()) {
                    if (this->remove(iterator->next())) {
                        modified =
                            static_cast<jxx::lang::jbool>(true);
                    }
                }
            }
            else {
                auto iterator = this->iterator();

                while (iterator->hasNext()) {
                    auto element = iterator->next();

                    auto object =
                        jxx::CAST<jxx::lang::Object>(
                            element);

                    if (collection->containsObject(object)) {
                        iterator->remove();

                        modified =
                            static_cast<jxx::lang::jbool>(true);
                    }
                }
            }

            return modified;
        }
    };

} // namespace jxx::util