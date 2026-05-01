#pragma once

#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.String.h"
#include "jxx.util.Collection.h"
#include "jxx.util.ElementEquals.h"

#include <stdexcept>

namespace jxx::util {

template <class E>
class AbstractCollection : public jxx::lang::Object, public virtual Collection<E> {
public:
    virtual ~AbstractCollection() = default;

    // Required by subclasses
    virtual jint size() const override = 0;
    virtual jxx::Ptr<Iterator<E>> iterator() override = 0;

    // Default implementations
    jbool isEmpty() const override { return size() == 0; }

    jbool contains(const E& e) const override {
        auto it = const_cast<AbstractCollection*>(this)->iterator();
        while (it->hasNext()) {
            E cur = it->next();
            if (ElementEquals<E>::eq(cur, e)) return true;
        }
        return false;
    }

    jbool add(const E& /*e*/) override {
        throw std::logic_error("UnsupportedOperationException: Collection.add()");
    }

    jbool remove(const E& e) override {
        auto it = iterator();
        while (it->hasNext()) {
            E cur = it->next();
            if (ElementEquals<E>::eq(cur, e)) {
                it->remove();
                return true;
            }
        }
        return false;
    }

    void clear() override {
        auto it = iterator();
        while (it->hasNext()) {
            it->next();
            it->remove();
        }
    }

    jbool addAll(jxx::Ptr<Collection<E>> c) override {
        if (!c) throw std::invalid_argument("NullPointerException: addAll(null)");
        jbool modified = false;
        auto it = c->iterator();
        while (it->hasNext()) {
            if (add(it->next())) modified = true;
        }
        return modified;
    }

    jbool containsAll(jxx::Ptr<Collection<E>> c) const override {
        if (!c) throw std::invalid_argument("NullPointerException: containsAll(null)");
        auto it = const_cast<Collection<E>*>(c.get())->iterator();
        while (it->hasNext()) {
            if (!contains(it->next())) return false;
        }
        return true;
    }

    jbool removeAll(jxx::Ptr<Collection<E>> c) override {
        if (!c) throw std::invalid_argument("NullPointerException: removeAll(null)");
        jbool modified = false;
        auto it = iterator();
        while (it->hasNext()) {
            E cur = it->next();
            if (c->contains(cur)) {
                it->remove();
                modified = true;
            }
        }
        return modified;
    }

    jbool retainAll(jxx::Ptr<Collection<E>> c) override {
        if (!c) throw std::invalid_argument("NullPointerException: retainAll(null)");
        jbool modified = false;
        auto it = iterator();
        while (it->hasNext()) {
            E cur = it->next();
            if (!c->contains(cur)) {
                it->remove();
                modified = true;
            }
        }
        return modified;
    }

    jxx::Ptr<jxx::lang::String> toString() const override {
        return std::make_shared<jxx::lang::String>("[AbstractCollection]");
    }
};

} // namespace jxx::util
