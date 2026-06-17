#pragma once

#include "util/jxx.util.Collection.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.wildcard.CollectionAny.h"
#include "util/jxx.util.wildcard.CollectionExtends.h"

namespace jxx {
namespace util {
namespace wildcard {

template <typename T>
class CollectionAnyView : public virtual CollectionAny {
private:
    jxx::Ptr<Collection<T>> inner;

    class ObjectIteratorView : public virtual Iterator<jxx::lang::Object> {
    private:
        jxx::Ptr<Iterator<T>> innerIterator;

    public:
        explicit ObjectIteratorView(jxx::Ptr<Iterator<T>> it)
            : innerIterator(it) {}

        virtual ~ObjectIteratorView() = default;

        virtual jbool hasNext() override {
            return innerIterator->hasNext();
        }

        virtual jxx::Ptr<jxx::lang::Object> next() override {
            return innerIterator->next();
        }

        virtual void remove() override {
            innerIterator->remove();
        }
    };

public:
    explicit CollectionAnyView(jxx::Ptr<Collection<T>> c)
        : inner(c) {
        if (inner == nullptr) {
            throw NullPointerException();
        }
    }

    virtual ~CollectionAnyView() = default;

    virtual jint size() override {
        return inner->size();
    }

    virtual jbool containsObject(jxx::Ptr<jxx::lang::Object> o) override {
        return inner->contains(jxx::Ptr<T>(o));
    }

    virtual jxx::Ptr<Iterator<jxx::lang::Object>> iteratorObject() override {
        return jxx::Ptr<Iterator<jxx::lang::Object>>(new ObjectIteratorView(inner->iterator()));
    }
};

template <typename Base, typename Derived>
class CollectionExtendsView : public virtual CollectionExtends<Base> {
private:
    jxx::Ptr<Collection<Derived>> inner;

    class BaseIteratorView : public virtual Iterator<Base> {
    private:
        jxx::Ptr<Iterator<Derived>> innerIterator;

    public:
        explicit BaseIteratorView(jxx::Ptr<Iterator<Derived>> it)
            : innerIterator(it) {}

        virtual ~BaseIteratorView() = default;

        virtual jbool hasNext() override {
            return innerIterator->hasNext();
        }

        virtual jxx::Ptr<Base> next() override {
            return innerIterator->next();
        }

        virtual void remove() override {
            innerIterator->remove();
        }
    };

    class ObjectIteratorView : public virtual Iterator<jxx::lang::Object> {
    private:
        jxx::Ptr<Iterator<Derived>> innerIterator;

    public:
        explicit ObjectIteratorView(jxx::Ptr<Iterator<Derived>> it)
            : innerIterator(it) {}

        virtual ~ObjectIteratorView() = default;

        virtual jbool hasNext() override {
            return innerIterator->hasNext();
        }

        virtual jxx::Ptr<jxx::lang::Object> next() override {
            return innerIterator->next();
        }

        virtual void remove() override {
            innerIterator->remove();
        }
    };

public:
    explicit CollectionExtendsView(jxx::Ptr<Collection<Derived>> c)
        : inner(c) {
        if (inner == nullptr) {
            throw NullPointerException();
        }
    }

    virtual ~CollectionExtendsView() = default;

    virtual jint size() override {
        return inner->size();
    }

    virtual jbool containsObject(jxx::Ptr<jxx::lang::Object> o) override {
        return inner->contains(jxx::Ptr<Derived>(o));
    }

    virtual jxx::Ptr<Iterator<jxx::lang::Object>> iteratorObject() override {
        return jxx::Ptr<Iterator<jxx::lang::Object>>(new ObjectIteratorView(inner->iterator()));
    }

    virtual jxx::Ptr<Iterator<Base>> iteratorExtends() override {
        return jxx::Ptr<Iterator<Base>>(new BaseIteratorView(inner->iterator()));
    }
};

} // namespace wildcard
} // namespace util
} // namespace jxx
