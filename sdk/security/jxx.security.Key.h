#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::security {

class Key : public ::jxx::lang::InterfaceBase<Key> {
public:
    ~Key() override = default;

    virtual ::jxx::Ptr<::jxx::lang::String>
    getAlgorithm() const = 0;

    virtual ::jxx::Ptr<::jxx::lang::String>
    getFormat() const = 0;

    virtual ::jxx::lang::ByteArray
    getEncoded() const = 0;
};

} // namespace jxx::security
