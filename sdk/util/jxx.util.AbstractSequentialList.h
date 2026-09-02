#pragma once
#include "util/jxx.util.AbstractList.h"
namespace jxx::util {
template <typename E>
class AbstractSequentialList : public AbstractList<E> {
public:
    virtual ~AbstractSequentialList() = default;
    jxx::Ptr<E> get(jxx::lang::jint i) const override { auto self=const_cast<AbstractSequentialList<E>*>(this); auto it=self->listIterator(i); if(!it->hasNext()) throw jxx::lang::IndexOutOfBoundsException(); return it->next(); }
    jxx::Ptr<E> set(jxx::lang::jint i,const jxx::Ptr<E>& e) override { auto it=listIterator(i); if(!it->hasNext()) throw jxx::lang::IndexOutOfBoundsException(); auto old=it->next(); it->set(e); return old; }
    void add(jxx::lang::jint i,const jxx::Ptr<E>& e) override { listIterator(i)->add(e); }
    jxx::Ptr<E> remove(jxx::lang::jint i) override { auto it=listIterator(i); if(!it->hasNext()) throw jxx::lang::IndexOutOfBoundsException(); auto old=it->next(); it->remove(); return old; }
};
} // namespace jxx::util
