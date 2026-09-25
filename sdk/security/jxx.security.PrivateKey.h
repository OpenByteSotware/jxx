#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::security {
class PrivateKey : public ::jxx::lang::InterfaceBase<PrivateKey> {
public:
    ~PrivateKey() override = default;
    virtual ::jxx::Ptr<::jxx::lang::String> getAlgorithm() const = 0;
    virtual ::jxx::Ptr<::jxx::lang::String> getFormat() const = 0;
    virtual ::jxx::lang::ByteArray getEncoded() const = 0;
};
}
