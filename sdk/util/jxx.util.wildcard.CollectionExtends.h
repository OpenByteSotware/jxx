#pragma once

#include "io/jxx.util.wildcard.CollectionAny.h"

namespace jxx { template <typename T> class Ptr; }

namespace jxx {
namespace util {
template <typename E> class Iterator;
namespace wildcard {

template <typename E>
class CollectionExtends : public virtual CollectionAny {
public:
    virtual ~CollectionExtends() = default;
    virtual jxx::Ptr<Iterator<E>> iteratorExtends() = 0;
};

} // namespace wildcard
} // namespace util
} // namespace jxx
