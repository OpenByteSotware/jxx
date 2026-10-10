#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

template <typename T>
class ObjLongConsumer : public ::jxx::lang::InterfaceBase<ObjLongConsumer<T>> {
public:
    ~ObjLongConsumer() override = default;
    virtual void accept(const ::jxx::Ptr<T>& object, ::jxx::lang::jlong value) = 0;
};

} // namespace jxx::util::function
