#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::ext::xml::bind::metadata {

class ObjectFactory
    : public ::jxx::lang::InterfaceBase<ObjectFactory> {
public:
    ~ObjectFactory() override = default;
    virtual ::jxx::Ptr<::jxx::lang::Object> create() = 0;
};

} // namespace jxx::ext::xml::bind::metadata
