#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.JxxArray.h"
namespace jxx::security { class SecureRandom : public ::jxx::lang::ClassBase<SecureRandom,::jxx::lang::Object> { public: SecureRandom(); void nextBytes(const ::jxx::lang::ByteArray& bytes); }; }
