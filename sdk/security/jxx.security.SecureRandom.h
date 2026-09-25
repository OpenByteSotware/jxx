#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::security { class SecureRandom : public ::jxx::lang::ClassBase<SecureRandom,::jxx::lang::Object> { public: SecureRandom(); void nextBytes(const ::jxx::lang::ByteArray& bytes); }; }
