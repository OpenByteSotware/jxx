
#pragma once
#include "jxx.lang.RuntimeException.h"
namespace jxx { namespace util {
class NoSuchElementException : public jxx::lang::RuntimeException {
public: using RuntimeException::RuntimeException; ~NoSuchElementException() override = default; };
}} // namespace jxx::util
