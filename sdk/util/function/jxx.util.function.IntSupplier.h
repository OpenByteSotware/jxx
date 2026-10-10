#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
namespace jxx::util::function { class IntSupplier : public ::jxx::lang::InterfaceBase<IntSupplier> { public: ~IntSupplier() override=default; virtual ::jxx::lang::jint getAsInt()=0; }; }
