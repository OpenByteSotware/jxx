#pragma once
#include <exception>
#include <string>
#include <memory>
#include "jxx.lang.RuntimeException.h"

namespace jxx::lang {
	class UnsupportedOperationException : public jxx::lang::RuntimeException {
	public: using RuntimeException::RuntimeException; ~UnsupportedOperationException() override = default;
	};
}