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
    virtual jxx::lang::jint size() = 0;
    virtual jxx::lang::jbool isEmpty() override { return size() == 0; }
    virtual jxx::lang::jbool contains(const jxx::Ptr<jxx::lang::Object> o) = 0;
    virtual jxx::Ptr<Iterator<E>> iterator() = 0;
    virtual jxx::lang::ObjectArray toArray() = 0;
    virtual jxx::lang::jbool add(const jxx::Ptr<E> e) = 0;
    virtual jxx::lang::jbool remove(const jxx::Ptr<jxx::lang::Object>& o) = 0;
    virtual jxx::lang::jbool containsAll(const jxx::Ptr<wildcard::CollectionAny> c) = 0;
    virtual jxx::lang::jbool addAll(const jxx::Ptr<wildcard::CollectionExtends<E>> c) = 0;
    virtual jxx::lang::jbool retainAll(const jxx::Ptr<wildcard::CollectionAny> c) = 0;
    virtual jxx::lang::jbool removeAll(const jxx::Ptr<wildcard::CollectionAny> c) = 0;
    virtual void clear() = 0;
    virtual jxx::lang::jbool equals(const jxx::Ptr<jxx::lang::Object>& o) = 0;
    virtual jxx::lang::jint hashCode() = 0;
    virtual jxx::Ptr<Spliterator<E>> spliterator() = 0;
};

} // namespace util
} // namespace jxx
