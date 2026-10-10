#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
namespace jxx::util::function { class DoubleSupplier : public ::jxx::lang::InterfaceBase<DoubleSupplier> { public: ~DoubleSupplier() override=default; virtual ::jxx::lang::jdouble getAsDouble()=0; }; }
