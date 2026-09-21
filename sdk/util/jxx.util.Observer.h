#pragma once

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::lang { class Object; }
namespace jxx::util { class Observable; }

namespace jxx::util {

class Observer
    : public ::jxx::lang::InterfaceBase<Observer> {
public:
    ~Observer() override = default;

    virtual void update(
        const ::jxx::Ptr<Observable>& observable,
        const ::jxx::Ptr<::jxx::lang::Object>& argument) = 0;
};

} // namespace jxx::util
