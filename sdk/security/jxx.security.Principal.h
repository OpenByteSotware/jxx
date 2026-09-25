#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
namespace jxx::security {
class Principal : public ::jxx::lang::InterfaceBase<Principal> {
public:
    ~Principal() override = default;
    virtual ::jxx::Ptr<::jxx::lang::String> getName() const = 0;
};
}
