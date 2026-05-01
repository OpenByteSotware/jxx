#pragma once

#include "jxx_types.h"
#include "jxx.util.AbstractCollection.h"
#include "jxx.util.Set.h"
#include "jxx.lang.Object.h"

#include <type_traits>

namespace jxx::util {

template <class E>
class AbstractSet : public AbstractCollection<E>, public virtual Set<E> {
public:
    virtual ~AbstractSet() = default;

    // Java Set equality semantics: same size + containsAll
    jbool equals(jxx::Ptr<jxx::lang::Object> o) const override {
        if (!o) return false;
        if (o.get() == static_cast<const jxx::lang::Object*>(this)) return true;

        // Best-effort: attempt cast to Collection<E>
        auto other = std::dynamic_pointer_cast<jxx::util::Collection<E>>(o);
        if (!other) return false;

        if (this->size() != other->size()) return false;
        return this->containsAll(other);
    }

    // Java Set hashCode: sum of element hash codes (null => 0)
    jint hashCode() const override {
        jint h = 0;
        auto it = const_cast<AbstractSet*>(this)->iterator();
        while (it->hasNext()) {
            auto e = it->next();
            if constexpr (std::is_same_v<E, jxx::Ptr<jxx::lang::Object>>) {
                h += (e ? e->hashCode() : 0);
            }
        }
        return h;
    }
};

} // namespace jxx::util
