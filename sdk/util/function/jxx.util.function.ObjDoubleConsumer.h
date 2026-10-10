#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

template <typename T>
class ObjDoubleConsumer : public ::jxx::lang::InterfaceBase<ObjDoubleConsumer<T>> {
public:
    ~ObjDoubleConsumer() override = default;
    virtual void accept(const ::jxx::Ptr<T>& object, ::jxx::lang::jdouble value) = 0;
};

} // namespace jxx::util::function
