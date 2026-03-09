
#pragma once
#include "jxx.lang.RuntimeException.h"
namespace jxx { namespace util {
class ConcurrentModificationException : public jxx::lang::RuntimeException {
public: using RuntimeException::RuntimeException; ~ConcurrentModificationException() override = default; };
}} // namespace jxx::util
