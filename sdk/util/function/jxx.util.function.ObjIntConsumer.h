#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

template <typename T>
class ObjIntConsumer : public ::jxx::lang::InterfaceBase<ObjIntConsumer<T>> {
public:
    ~ObjIntConsumer() override = default;
    virtual void accept(const ::jxx::Ptr<T>& object, ::jxx::lang::jint value) = 0;
};

} // namespace jxx::util::function
