#pragma once

#include "util/jxx.util.Collection.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.Spliterator.h"
#include "util/jxx.util.wildcard.CollectionAny.h"
#include "util/jxx.util.wildcard.CollectionExtends.h"

namespace jxx {
namespace util {

template <typename E>
class Set : virtual public Collection<E> {
public:
    virtual ~Set() = default;
    virtual jint size() override = 0;
    virtual jbool isEmpty() override { return size() == 0; }
    virtual jbool contains(jxx::Ptr<jxx::lang::Object> o) override = 0;
    virtual jxx::Ptr<Iterator<E>> iterator() override = 0;
    virtual jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>>> toArray() override = 0;
    virtual jbool add(jxx::Ptr<E> e) override = 0;
    virtual jbool remove(jxx::Ptr<jxx::lang::Object> o) override = 0;
    virtual jbool containsAll(jxx::Ptr<wildcard::CollectionAny> c) override = 0;
    virtual jbool addAll(jxx::Ptr<wildcard::CollectionExtends<E>> c) override = 0;
    virtual jbool retainAll(jxx::Ptr<wildcard::CollectionAny> c) override = 0;
    virtual jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) override = 0;
    virtual void clear() override = 0;
    virtual jbool equals(jxx::Ptr<jxx::lang::Object> o) override = 0;
    virtual jint hashCode() override = 0;
    virtual jxx::Ptr<Spliterator<E>> spliterator() = 0;
};

} // namespace util
} // namespace jxx
