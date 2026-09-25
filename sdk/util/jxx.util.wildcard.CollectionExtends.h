#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "util/jxx.util.wildcard.CollectionAny.h"

namespace jxx {
namespace util {
template <typename E> class Iterator;
namespace wildcard {

template <typename E>
class CollectionExtends : public ::jxx::lang::InterfaceBase<CollectionExtends<E>, CollectionAny> {
public:
    virtual ~CollectionExtends() = default;
    virtual ::jxx::Ptr<Iterator<E>> iteratorExtends() = 0;
};

} // namespace wildcard
} // namespace util
} // namespace jxx
