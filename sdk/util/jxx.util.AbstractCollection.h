#pragma once
#include <stdexcept>
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Collection.h"
#include "util/jxx.util.ElementEquals.h"


namespace jxx::util {

template <class E>
class AbstractCollection : public jxx::lang::Object, public virtual Collection<E> {
public:
    virtual ~AbstractCollection() = default;

    // Required by subclasses
    virtual jint size() const override = 0;
    virtual jxx::Ptr<Iterator<E>> iterator() override = 0;

    // Default implementations
    jxx::lang::jbool isEmpty() const override { return size() == 0; }

    jxx::lang::jbool contains(const E& e) const override {
        auto it = const_cast<AbstractCollection*>(this)->iterator();
        while (it->hasNext()) {
            E cur = it->next();
            if (ElementEquals<E>::eq(cur, e)) return true;
        }
        return false;
    }

    jxx::lang::jbool add(const E& /*e*/) override {
        throw std::logic_error("UnsupportedOperationException: Collection.add()");
    }

    jxx::lang::jbool remove(const E& e) override {
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

    jxx::lang::jbool addAll(jxx::Ptr<Collection<E>> c) override {
        if (!c) throw std::invalid_argument("NullPointerException: addAll(null)");
        jxx::lang::jbool modified = false;
        auto it = c->iterator();
        while (it->hasNext()) {
            if (add(it->next())) modified = true;
        }
        return modified;
    }

    jxx::lang::jbool containsAll(jxx::Ptr<Collection<E>> c) const override {
        if (!c) throw std::invalid_argument("NullPointerException: containsAll(null)");
        auto it = const_cast<Collection<E>*>(c.get())->iterator();
        while (it->hasNext()) {
            if (!contains(it->next())) return false;
        }
        return true;
    }

    jxx::lang::jbool removeAll(jxx::Ptr<Collection<E>> c) override {
        if (!c) throw std::invalid_argument("NullPointerException: removeAll(null)");
        jxx::lang::jbool modified = false;
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

    jxx::lang::jbool retainAll(jxx::Ptr<Collection<E>> c) override {
        if (!c) throw std::invalid_argument("NullPointerException: retainAll(null)");
        jxx::lang::jbool modified = false;
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
        return jxx::NEW<jxx::lang::String>("[AbstractCollection]");
    }
};

} // namespace jxx::util
