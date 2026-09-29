#pragma once
#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::lang {
    class String;
}
namespace jxx::security {
class Principal : public ::jxx::lang::InterfaceBase<Principal> {
public:
    ~Principal() override = default;
    virtual ::jxx::Ptr<::jxx::lang::String> getName() const = 0;
};
}
