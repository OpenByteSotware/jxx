
#pragma once
#include "lang/jxx.lang.h"

namespace jxx { namespace util {
class NoSuchElementException : public jxx::lang::RuntimeException {
public: 
	using RuntimeException::RuntimeException;
	  ~NoSuchElementException() override = default; };
}} // namespace jxx::util
