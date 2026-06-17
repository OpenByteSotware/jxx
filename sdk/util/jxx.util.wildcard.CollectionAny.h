#pragma once

#include "io/jxx.lang.Object.h"

namespace jxx {
namespace util {

template <typename E> class Iterator;

namespace wildcard {

class CollectionAny : virtual public jxx::lang::Object {
public:
    virtual ~CollectionAny() = default;

    virtual jint size() = 0;
    virtual jbool containsObject(jxx::Ptr<jxx::lang::Object> o) = 0;
    virtual jxx::Ptr<Iterator<jxx::lang::Object>> iteratorObject() = 0;
};

} // namespace wildcard
} // namespace util
} // namespace jxx
