#pragma once

#include "util/jxx.util.AbstractCollection.h"
#include "util/jxx.util.Set.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.wildcard.CollectionAny.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx {
namespace util {

template <typename E>
class AbstractSet : public AbstractCollection<E>, public virtual Set<E> {
public:
    virtual ~AbstractSet() = default;

    virtual jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> o) override {
        if (o == nullptr) return false;
        auto other = jxx::CAST<wildcard::CollectionAny, jxx::lang::Object>(o);
        if (other == nullptr) return false;
        if (other->size() != this->size()) return false;
        try {
            return this->containsAll(other);
        } catch (...) {
            return false;
        }
    }

    virtual jxx::lang::jint hashCode() override {
        jxx::lang::jint h = 0;
        auto it = this->iterator();
        while (it->hasNext()) {
            auto e = it->next();
            if (e != nullptr) h += e->hashCode();
        }
        return h;
    }

    virtual jxx::lang::jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) override {
        if (c == nullptr) throw jxx::lang::NullPointerException();
        jxx::lang::jbool modified = false;
        if (this->size() > c->size()) {
            auto it = c->iteratorObject();
            while (it->hasNext()) {
                if (this->remove(it->next())) modified = true;
            }
        } else {
            auto it = this->iterator();
            while (it->hasNext()) {
                auto e = it->next();
                if (c->containsObject(jxx::lang::ptr_static_cast<jxx::lang::Object>(e))) {
                    it->remove();
                    modified = true;
                }
            }
        }
        return modified;
    }
};

} // namespace util
} // namespace jxx
