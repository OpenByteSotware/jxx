#pragma once

#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Iterator.h"

namespace jxx {
namespace util {

namespace wildcard {

class CollectionAny {
public:
    virtual ~CollectionAny() = default;

    virtual jxx::lang::jint size() = 0;
    virtual jxx::lang::jbool containsObject(jxx::Ptr<jxx::lang::Object> o) = 0;
    virtual jxx::Ptr<Iterator<jxx::lang::Object>> iteratorObject() = 0;
};

} // namespace wildcard
} // namespace util
} // namespace jxx
