#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "util/jxx.util.AbstractList.h"
namespace jxx::util {
template <typename E>
class AbstractSequentialList : public AbstractList<E> {
public:
    using Super = AbstractList<E>;
    using JxxClassInfoMarker =
        ::jxx::lang::ClassInfo<AbstractSequentialList<E>, AbstractList<E>>;
    static ::jxx::Ptr<::jxx::lang::ClassAny> Class() {
        return JxxClassInfoMarker::Class();
    }
    virtual ~AbstractSequentialList() = default;
    jxx::Ptr<E> get(jxx::lang::jint i) const override { auto self=const_cast<AbstractSequentialList<E>*>(this); auto it=self->listIterator(i); if(!it->hasNext()) throw jxx::lang::IndexOutOfBoundsException(); return it->next(); }
    jxx::Ptr<E> set(jxx::lang::jint i,const jxx::Ptr<E>& e) override { auto it=this->listIterator(i); if(!it->hasNext()) throw jxx::lang::IndexOutOfBoundsException(); auto old=it->next(); it->set(e); return old; }
    void add(jxx::lang::jint i,const jxx::Ptr<E>& e) override { this->listIterator(i)->add(e); }
    jxx::Ptr<E> remove(jxx::lang::jint i) override { auto it=this->listIterator(i); if(!it->hasNext()) throw jxx::lang::IndexOutOfBoundsException(); auto old=it->next(); it->remove(); return old; }
};
} // namespace jxx::util
