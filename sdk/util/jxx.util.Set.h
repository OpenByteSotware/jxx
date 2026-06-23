#pragma once

#include "lang/jxx.lang.Object.h"
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
    virtual jxx::lang::jint size() override = 0;
    virtual jxx::lang::jbool isEmpty() override { return size() == 0; }
    virtual jxx::lang::jbool contains(jxx::Ptr<jxx::lang::Object> o) override = 0;
    virtual jxx::Ptr<Iterator<E>> iterator() override = 0;
    virtual jxx::Ptr<JxxArray<jxx::Ptr<jxx::lang::Object>>> toArray() override = 0;
    virtual jxx::lang::jbool add(jxx::Ptr<E> e) override = 0;
    virtual jxx::lang::jbool remove(jxx::Ptr<jxx::lang::Object> o) override = 0;
    virtual jxx::lang::jbool containsAll(jxx::Ptr<wildcard::CollectionAny> c) override = 0;
    virtual jxx::lang::jbool addAll(jxx::Ptr<wildcard::CollectionExtends<E>> c) override = 0;
    virtual jxx::lang::jbool retainAll(jxx::Ptr<wildcard::CollectionAny> c) override = 0;
    virtual jxx::lang::jbool removeAll(jxx::Ptr<wildcard::CollectionAny> c) override = 0;
    virtual void clear() override = 0;
    virtual jxx::lang::jbool equals(jxx::Ptr<jxx::lang::Object> o) override = 0;
    virtual jxx::lang::jint hashCode() override = 0;
    virtual jxx::Ptr<Spliterator<E>> spliterator() = 0;
};

} // namespace util
} // namespace jxx
