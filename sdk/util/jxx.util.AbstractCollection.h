#pragma once

#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Collection.h"
#include "util/jxx.util.Iterator.h"

namespace jxx {
namespace util {

template <typename E>
class AbstractCollection : public Object, public virtual Collection<E> {
public:
    virtual ~AbstractCollection() = default;
    virtual jxx::lang::jbool isEmpty() override { return this->size() == 0; }

    virtual jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> o) override {
        auto it = this->iterator();
        if (o == nullptr) {
            while (it->hasNext()) if (it->next() == nullptr) return true;
        } else {
            while (it->hasNext()) {
                auto e = it->next();
                if (e != nullptr && o->equals(jxx::lang::ptr_static_cast<jxx::lang::Object>(e))) return true;
            }
        }
        return false;
    }

    virtual jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>> toArray() override {
        const jxx::lang::jint sz = this->size();
        auto result = jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>, 1U>>(new JxxArray<jxx::Ptr<jxx::lang::Object>>(sz));
        auto it = this->iterator();
        jxx::lang::jint i = 0;
        while (it->hasNext()) result->set(i++, jxx::lang::ptr_static_cast<jxx::lang::Object>(it->next()));
        return result;
    }

    virtual jxx::lang::jbool add(jxx::Ptr<E> e) = 0;

    virtual jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> o) override {
        auto it = this->iterator();
        if (o == nullptr) {
            while (it->hasNext()) {
                if (it->next() == nullptr) { it->remove(); return true; }
            }
        } else {
            while (it->hasNext()) {
                auto e = it->next();
                if (e != nullptr && o->equals(jxx::lang::ptr_static_cast<jxx::lang::Object>(e))) { it->remove(); return true; }
            }
        }
        return false;
    }

    virtual jxx::lang::jbool containsAll(jxx::Ptr<wildcard::CollectionAny> c) override {
        auto it = c->iteratorObject();
        while (it->hasNext()) if (!this->contains(it->next())) return false;
        return true;
    }

    virtual jxx::lang::jbool addAll(jxx::Ptr<wildcard::CollectionExtends<E>> c) override {
        jxx::lang::jbool modified = false;
        auto it = c->iteratorExtends();
        while (it->hasNext()) if (this->add(it->next())) modified = true;
        return modified;
    }

    virtual jxx::lang::jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) override {
        jxx::lang::jbool modified = false;
        auto it = this->iterator();
        while (it->hasNext()) {
            if (c->containsObject(jxx::lang::ptr_static_cast<jxx::lang::Object>(it->next()))) { it->remove(); modified = true; }
        }
        return modified;
    }

    virtual jxx::lang::jbool retainAll(jxx::Ptr<wildcard::CollectionAny> c) override {
        jxx::lang::jbool modified = false;
        auto it = this->iterator();
        while (it->hasNext()) {
            if (!c->containsObject(jxx::lang::ptr_static_cast<jxx::lang::Object>(it->next()))) { it->remove(); modified = true; }
        }
        return modified;
    }

    virtual void clear() override {
        auto it = this->iterator();
        while (it->hasNext()) { it->next(); it->remove(); }
    }
};

} // namespace util
} // namespace jxx
