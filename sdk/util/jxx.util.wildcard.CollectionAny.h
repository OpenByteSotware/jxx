#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"


namespace jxx {
namespace util {
template <typename E> class Iterator;
namespace wildcard {

class CollectionAny : public ::jxx::lang::InterfaceBase<CollectionAny> {
public:
    virtual ~CollectionAny() = default;
    virtual ::jxx::lang::jint size() = 0;
    virtual ::jxx::lang::jbool containsObject(const ::jxx::Ptr<::jxx::lang::Object>& o) = 0;
    virtual ::jxx::Ptr<Iterator<::jxx::lang::Object>> iteratorObject() = 0;
};

} // namespace wildcard
} // namespace util
} // namespace jxx
