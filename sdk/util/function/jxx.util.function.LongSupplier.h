#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
namespace jxx::util::function { class LongSupplier : public ::jxx::lang::InterfaceBase<LongSupplier> { public: ~LongSupplier() override=default; virtual ::jxx::lang::jlong getAsLong()=0; }; }
